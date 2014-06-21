/**
* Description	Manages the rendering of graphics by OpenGL
**/

#include "renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_helper.h"
#include "texture_helper.h"
#include "model_helper.h"
#include "controls.h"

const GLchar* U_MVP_MATRIX = "u_MVPMatrix";
const GLchar* U_TEXTURE = "u_TextureUnit";

bool Renderer::initialise()
{
	// Rules for drawing
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	
	// Creates a simple shader program
	program_id = createProgram("vertex_shader.glsl", "fragment_shader.glsl");
	texture_id = TextureHelper::loadBMP("marble.bmp");
		
	// Locations of our shader uniforms
	mvp_matrix_location = glGetUniformLocation(program_id, U_MVP_MATRIX);
	texture_location = glGetUniformLocation(program_id, U_TEXTURE);
	
	// Loads model
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool result = loadOBJ("marble.obj", vertices, uvs, normals);
	if (result == false)
		return false;
		
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	
	glBindVertexArray(0);
	
	num_vertices = vertices.size();
	
	return true;
}

Renderer::~Renderer()
{
	// Cleans up OpenGL allocations
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &uv_buffer);
	glDeleteBuffers(1, &normal_buffer);
	glDeleteProgram(program_id);
	glDeleteTextures(1, &texture_id);
	glDeleteVertexArrays(1, &vertex_array_id);
	
	// Closes GLFW window and exits GLFW
	glfwTerminate();
}

void Renderer::draw(GLFWwindow* window)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Simple shader
	glUseProgram(program_id);
	
	// Camera 
	computeMatricesFromInputs(window);
	glm::mat4 projection_matrix = getProjectionMatrix();
	glm::mat4 view_matrix = getViewMatrix();
	glm::mat4 model_matrix = glm::mat4(1.0);
	glm::mat4 model_view_projection_matrix = projection_matrix * view_matrix * model_matrix; 
	glUniformMatrix4fv(mvp_matrix_location, 1, GL_FALSE, &model_view_projection_matrix[0][0]);
	
	// BEGIN managing model verticies (TODO Move model vertex code to a model class)
	glBindVertexArray(vertex_array_id);
	
	// Binds texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(texture_location, 0);
	
	// Position attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	// UV attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));
	
	// Normal attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));	
	
	// Draws model
	glDrawArrays(GL_TRIANGLES, 0, num_vertices);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	
	// END managing model verticies
	glBindVertexArray(0);
	
	// Swaps drawing canvas
	glfwSwapBuffers(window);
	glfwPollEvents();
}
