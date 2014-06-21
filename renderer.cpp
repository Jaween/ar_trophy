/**
* Description	Manages the rendering of graphics by OpenGL
**/

#include "renderer.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.h"
#include "model.h"

bool Renderer::initialise()
{
	// Rules for drawing
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	// Creates a simple shader program
	shader_program = new ShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");
	
	
	Model* marble = new Model("marble.obj");
	models.push_back(marble);
	
	return true;
}

Renderer::~Renderer()
{
	delete shader_program;
	
	for (int i = 0; i < models.size(); i++)
	{
		Model* model = models.at(i);
		delete model;
	}
	
	// Closes GLFW window and exits GLFW
	glfwTerminate();
}

glm::mat4 Renderer::position_object_in_scene(glm::vec3 position)
{
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
	return model_matrix;
}

void Renderer::draw(GLFWwindow* window)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Camera 
	computeMatricesFromInputs(window);
	glm::mat4 projection_matrix = getProjectionMatrix();
	glm::mat4 view_matrix = getViewMatrix();
	
	// Simple shader
	shader_program->useProgram();
	
	for (int i = 0; i < models.size(); i++)
	{
		// Updates data and draws
		glm::mat4 model_matrix = position_object_in_scene(models.at(i)->getPosition());
		glm::mat4 model_view_projection_matrix = projection_matrix * view_matrix * model_matrix; 
		shader_program->setUniforms(model_view_projection_matrix, models.at(i)->getTextureID());
		models.at(i)->draw();
	}
	
	// Swaps drawing canvas
	glfwSwapBuffers(window);
	glfwPollEvents();
}
