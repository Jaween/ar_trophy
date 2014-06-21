#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
	public:
		~Renderer();
		bool initialise();
		void draw(GLFWwindow* window);
		
	private:
		int num_vertices;
		
		GLuint program_id;
		GLuint texture_id;
		
		GLuint mvp_matrix_location;
		GLuint texture_location;
		
		GLuint vertex_array_id;
		GLuint vertex_buffer;
		GLuint uv_buffer;
		GLuint normal_buffer;
};
#endif 
