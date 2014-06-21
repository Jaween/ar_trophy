#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>	
#include <glm/glm.hpp>

class ShaderProgram
{
	public:
		ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path);
		~ShaderProgram();
		void setUniforms(glm::mat4 mvp_matrix, GLuint texture_id);
		void useProgram();
		
	private:
		GLuint program_id;
		
		GLuint texture_location;
		GLuint mvp_matrix_location;
};
#endif 
