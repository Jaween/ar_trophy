/**
* Author		Jaween Ediriweera
* Student ID	a1647468
* Filename		controls.cpp
* Description	Sells your dog, collects your clip, buys your animal and straightens out your bird
**/

#include "controls.h"

glm::mat4 view_matrix;
glm::mat4 projection_matrix;
glm::vec3 position = glm::vec3(0, 0, 5);

float horizontal_angle = 3.14f;
float vertical_angle = 0.0f;
const float MAX_VERTICAL_ANGLE = 70.0 * (3.14f / 180.0f);
const float INITIAL_FOV = 45.0f;

float speed = 3.0f;
float mouse_speed = 0.1f;


void computeMatricesFromInputs(GLFWwindow* window)
{
	static double last_time = glfwGetTime();
	double current_time = glfwGetTime();
	float delta_time = float(current_time - last_time);
	
	// Gets mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	
	// Resets mouse position
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	//glfwSetCursorPos(window, width/2, height/2);
	
	// Computes new orientation
	horizontal_angle += 0;//mouse_speed * delta_time * float(width/2 - xpos);
	vertical_angle += 0;//mouse_speed * delta_time * float(height/2 - ypos);
	
	if (vertical_angle >= MAX_VERTICAL_ANGLE)
		vertical_angle = MAX_VERTICAL_ANGLE;
	else if (vertical_angle <= -MAX_VERTICAL_ANGLE)
		vertical_angle = -MAX_VERTICAL_ANGLE;
	
	// Spherical coords to cartesion coords conversion
	glm::vec3 direction(
		cos(vertical_angle) * sin(horizontal_angle),
		sin(vertical_angle),
		cos(vertical_angle) * cos(horizontal_angle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontal_angle - 3.14f/2.0f),
		0,
		cos(horizontal_angle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross(right, direction);
	
	
	// Keyboard
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		position -= right * delta_time * speed;
		
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		position += direction * delta_time * speed;
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		position += right * delta_time * speed;
		
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		position -= direction * delta_time * speed;
	
	// Field of view
	float fov = INITIAL_FOV;// - 5 * glfwGetMouseWheel();
	
	// Projection matrix
	projection_matrix = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
	
	// View matrix
	view_matrix = glm::lookAt(
		position,
		position + direction,
		up
	);
	
	last_time = current_time;
}

glm::mat4 getProjectionMatrix()
{
	return projection_matrix;
}

glm::mat4 getViewMatrix()
{
	return view_matrix;
}
