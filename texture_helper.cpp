/**
* Description	Loads textures
**/

#include "texture_helper.h"

#include <iostream>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadBMP(const char* path)
{
	unsigned char header[54];
	unsigned int data_pos;
	unsigned int width, height;
	unsigned int image_size;
	unsigned char* data;
	
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened" << std::endl;
		return 1;
	}
	
	// Is there a 54 byte header?
	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << "Not a valid bitmap file" << std::endl;
		return 1;
	}
	
	// Does in begin with 'BM'?
	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << "Not a valid bitmap file" << std::endl;
		return 1;
	}
	
	data_pos = *(int*) & (header[0x0A]);
	image_size = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);
	
	if (image_size == 0)
		image_size = width * height * 3;
		
	if (data_pos = 0)
		data_pos = 54;
	
	// Creates our data buffer	
	data = new unsigned char[image_size];
	
	// Reads data from the file
	fread(data, 1, image_size, file);
	fclose(file);
	
	// Generates OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	
	// Loads it into OpenGL
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	
	// OpenGL now has it's own copy of the data
	delete [] data;
	
	// Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	return texture_id;
}

// TGA loading has been removed from GLFW
/*GLuint loadTGA(const char* path)
{
	// Generates OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	
	// Binds the texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	// Reads the file
	glfwLoadTexture2D(path, 0);
	
	// Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	// Unbinds the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return texture_id;
}*/

GLuint loadDDS(const char* path)
{
	unsigned char header[124];
	
	FILE* fp;
	
	// Trys to open the file
	fp = fopen(path, "rb");
	if (fp == NULL)
	{
		std::cout << "Could not open the file" << std::endl;
		return 1;
	}
		
	// Verifies the type of file
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0)
	{
		fclose(fp);
		std::cout << "Invalid DDS file" << std::endl;
		return 1;
	}
	
	// Gets the header data
	fread(&header, 124, 1, fp);
	unsigned int width 			= *(unsigned int*) & (header[12]);
	unsigned int height 			= *(unsigned int*) & (header[8]);
	unsigned int linear_size 	= *(unsigned int*) & (header[16]);
	unsigned int mipmap_count 	= *(unsigned int*) & (header[24]);
	unsigned int four_cc 			= *(unsigned int*) & (header[80]);
	
	// Gets the image data
	
	// Determines the size the buffer must be to include all the mipmaps
	unsigned int buffer_size;
	if (mipmap_count > 1)
		buffer_size = linear_size * 2;
	else
		buffer_size = linear_size;
	unsigned char* buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));
	
	fread(buffer, 1, buffer_size, fp);
	fclose(fp);
	
	// Deals with DXT1, DXT3, DXT5
	unsigned int components;
	if (four_cc == FOURCC_DXT1)
		components = 3;
	else
		components = 4;
	
	unsigned int format;
	switch (four_cc)
	{
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			free(buffer);
			std::cout << "Invalid DDS format: " << four_cc << std::endl;
			return 1;
	}
	
	// Generates OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	
	// Binds the texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	// Fills each mipmap
	unsigned int block_size;
	if (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
		block_size = 8;
	else
		block_size = 16;
	
	// Loads the mimaps
	unsigned int offset = 0;
	for (unsigned int level = 0; level < mipmap_count && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * block_size;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer + offset);
		offset += size;
		width /= 2;
		height /= 2;
	}
	free(buffer);
	
	// Unbinds the texture
	//glBindTexture(GL_TEXTURE_2D, 0);
	
	return texture_id;
}

void updateTexture(GLuint texture_id, int width, int height, const unsigned char* data)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
}
