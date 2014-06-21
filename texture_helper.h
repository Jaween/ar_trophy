#ifndef TEXTURE_HELPER_H
#define TEXTURE_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

class TextureHelper
{
	public:
		static GLuint loadBMP(const char* path);
		//static GLuint loadTGA(const char* path);
		static GLuint loadDDS(const char* path);
};
#endif 
