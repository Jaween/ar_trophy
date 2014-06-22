/**
* Description	Manages the rendering of graphics by OpenGL
**/

#include "renderer.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.h"
#include "model.h"
#include "texture_helper.h"

bool Renderer::initialise()
{
	// Rules for drawing
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	
	// Creates a simple shader program
	shader_program = new ShaderProgram("resources/shaders/vertex_shader.glsl", "resources/shaders/fragment_shader.glsl");
	
	// Loads and initialises models
	// TODO: Load model without texture (currently it's loading an unused texture)
	environment = new Model("resources/models/back.obj", "resources/textures/marble.bmp");
	environment->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	
	trophy = new Model("resources/models/marble.obj", "resources/textures/marble.bmp");
	trophy->setPosition(glm::vec3(-2.0f, 0.0f, -4.0f));
	
	return true;
}

Renderer::~Renderer()
{
	delete shader_program;
	
	/*for (int i = 0; i < models.size(); i++)
	{
		Model* model = models.at(i);
		delete model;
	}*/
	delete trophy;
	delete environment;
	
	// Closes GLFW window and exits GLFW
	glfwTerminate();
}

glm::mat4 Renderer::position_object_in_scene(glm::vec3 position)
{
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), position);
	return model_matrix;
}

void Renderer::draw(GLFWwindow* window, unsigned char* frame_data, int width, int height)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Simple shader
	shader_program->useProgram();
	
	// Background
	updateTexture(environment->getTextureID(), width, height, frame_data);
	glm::mat4 ortho_projection_matrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
	glm::mat4 env_model_matrix = position_object_in_scene(environment->getPosition());
	glm::mat4 model_projection_matrix = ortho_projection_matrix * env_model_matrix;
	shader_program->setUniforms(model_projection_matrix, environment->getTextureID());
	environment->draw();
	
	// Camera 
	computeMatricesFromInputs(window);
	glm::mat4 projection_matrix = getProjectionMatrix();
	glm::mat4 view_matrix = getViewMatrix();
	
	// Updates data and draws
	glm::mat4 model_matrix = position_object_in_scene(trophy->getPosition());
	glm::mat4 model_view_projection_matrix = projection_matrix * view_matrix * model_matrix; 
	shader_program->setUniforms(model_view_projection_matrix, trophy->getTextureID());
	trophy->draw();
	
	// Swaps drawing canvas
	glfwSwapBuffers(window);
	glfwPollEvents();
}
