#include "shader_helper.h"

#include <iostream>
#include <vector>
#include <fstream>

GLuint createProgram(const char* vertex_filepath, const char* fragment_filepath)
{
		// Creates the shaders
		GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
		
		// Reads in the shader code from their files
		std::string vertex_shader_code = readShaderFile(vertex_filepath);
		std::string fragment_shader_code = readShaderFile(fragment_filepath);
		
		// Compiles each shader
		compileShader(vertex_shader_code, vertex_shader_id);
		compileShader(fragment_shader_code, fragment_shader_id);
		
		// Links the shaders into a single program
		GLuint program_id = linkShaders(vertex_shader_id, fragment_shader_id);
		
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);
		
		return program_id;
}

std::string readShaderFile(const char* filepath)
{
	// Reads in the shader code from the file
	std::string shader_code;
	std::ifstream shader_stream(filepath, std::ios::in);
	if (shader_stream.is_open())
	{
		std::string line = "";
		while (getline(shader_stream, line))
			shader_code += line + "\n";
		shader_stream.close();
	}
	return shader_code;
}

void compileShader(std::string shader_code, GLuint shader_id)
{
	// Compiles the shader
	std::cout << "Compiling shader" << std::endl;
	char const* source_pointer = shader_code.c_str();
	glShaderSource(shader_id, 1, &source_pointer, NULL);
	glCompileShader(shader_id);
	
	// Prints any error messages
	GLint result = GL_FALSE;
	int info_log_length;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	
	if (info_log_length > 0)
	{
		std::vector<char> shader_error_message(info_log_length + 1);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, &shader_error_message[0]);
		std::cout << shader_error_message[0] << std::endl;
	}
}

GLuint linkShaders(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
	// Links the shaders together
	std::cout << "Linking shaders" << std::endl;
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);
	
	// Prints any error messages
	GLint result = GL_FALSE;
	int info_log_length;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	
	if (info_log_length > 0)
	{
		std::vector<char> program_error_message(info_log_length + 1);
		glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
		std::cout << program_error_message[0] << std::endl;
	}
	
	return program_id;
}
