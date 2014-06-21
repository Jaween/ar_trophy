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
		void draw(GLFWwindow* window);
		
	private:
		std::vector<Model*> models;
		ShaderProgram* shader_program;
		
};
#endif 
