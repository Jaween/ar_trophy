#ifndef TEXTURE_HELPER_H
#define TEXTURE_HELPER_H

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

GLuint loadBMP(const char* path);
//GLuint loadTGA(const char* path);
GLuint loadDDS(const char* path);

void updateTexture(GLuint texture_id, int width, int height, const unsigned char* data);


#endif 
