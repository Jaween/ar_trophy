#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <GL/glew.h>

GLuint createProgram(const char* vertex_filepath, const char* fragment_filepath);
std::string readShaderFile(const char* filepath);
void compileShader(std::string shader_code, GLuint shader_id);
GLuint linkShaders(GLuint vertex_shader_id, GLuint fragment_shader_id);

#endif 
