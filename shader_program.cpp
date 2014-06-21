/**
* Description	Shader wrapper
**/

#include "shader_program.h"

#include "shader_helper.h"

#include <GL/glew.h>


const GLchar* U_MVP_MATRIX = "u_MVPMatrix";
const GLchar* U_TEXTURE = "u_TextureUnit";

ShaderProgram::ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path)
{
	program_id = createProgram(vertex_shader_path, fragment_shader_path);
	
	mvp_matrix_location = glGetUniformLocation(program_id, U_MVP_MATRIX);
	texture_location = glGetUniformLocation(program_id, U_TEXTURE);
}

ShaderProgram::~ShaderProgram()
{
	// Cleans up OpenGL allocations
	glDeleteProgram(program_id);
}

void ShaderProgram::setUniforms(glm::mat4 mvp_matrix, GLuint texture_id)
{
	
	// Updates uniforms
	glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &mvp_matrix[0][0]);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(texture_location, 0);
}

void ShaderProgram::useProgram()
{
	glUseProgram(program_id);
}
