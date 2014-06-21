/**
* Description	A 3D model
**/

#include "model.h"

#include "texture_helper.h"
#include "model_helper.h"

const int ATTRIB_POSITION_INDEX = 0;
const int ATTRIB_UV_INDEX = 1;
const int ATTRIB_NORMAL_INDEX = 2;

Model::Model(const char* path)
{
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);
	
	// Loads 3D model
	bool result = loadOBJ(path, vertices, uvs, normals);
	/*if (result == false)
		return false;
	*/
		
	texture_id = TextureHelper::loadBMP("marble.bmp");
	
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	
	position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Model::~Model()
{
	// Cleans up OpenGL allocations
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &uv_buffer);
	glDeleteBuffers(1, &normal_buffer);
	glDeleteTextures(1, &texture_id);
	glDeleteVertexArrays(1, &vertex_array_id);	
}

glm::vec3 Model::getPosition()
{
	return position;
}

GLuint Model::getTextureID()
{
	return texture_id;
}

void Model::draw()
{
	glBindVertexArray(vertex_array_id);
	
	// Position attribute
	glEnableVertexAttribArray(ATTRIB_POSITION_INDEX);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(ATTRIB_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	// UV attribute
	glEnableVertexAttribArray(ATTRIB_UV_INDEX);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(ATTRIB_UV_INDEX, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	// Normal attribute
	glEnableVertexAttribArray(ATTRIB_NORMAL_INDEX);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(ATTRIB_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));	
	
	// Draws model
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
	glDisableVertexAttribArray(ATTRIB_POSITION_INDEX);
	glDisableVertexAttribArray(ATTRIB_UV_INDEX);
	glDisableVertexAttribArray(ATTRIB_NORMAL_INDEX);
	
	glBindVertexArray(0);
}
