#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "shader_program.h"

class Renderer
{
	public:
		~Renderer();
		bool initialise();
		glm::mat4 position_object_in_scene(glm::vec3 position);
		void draw(GLFWwindow* window, unsigned char* frame_data, int width, int height);
		
	private:
		Model* trophy;
		Model* environment;
		
		ShaderProgram* shader_program;
		
};
#endif 
