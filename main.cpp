#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "vision.h"
#include "renderer.h"


const int WAIT_MILLIS = 10;

GLFWwindow* initialiseWindow()
{
	// Initialises GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialise GLFW" << std::endl;
		return NULL;
	}
	
	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Opens window and creates its OpenGL context
	GLFWwindow* window = glfwCreateWindow(640, 480, "Trophy Gallery", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	
	glfwMakeContextCurrent(window);
	
	// Initialises GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return NULL;
	}
	return window;
}

int main()
{	
	// Initialises camera
	Vision* vision = new Vision();
	bool camera_initialisation_success = vision->initialise("resources/feature_grid.jpg", 0);
	if (camera_initialisation_success == false)
		return 1;
	int feed_width, feed_height;
	vision->getDimensions(feed_width, feed_height);
		
	// Initialises window
	GLFWwindow* window = initialiseWindow();
	if (window == NULL)
		return 1;
	
	// Initialises OpenGL renderer
	Renderer* renderer = new Renderer();
	bool renderer_initialisation_success = renderer->initialise();
	if (renderer_initialisation_success == false)
		return 1;
	
	// Update and draw loop
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{	
		bool orientation_found = true;//vision->calculateOrientation();
		unsigned char* frame_data = vision->getFrameData();
		if (orientation_found)
		{
			//renderer->update(vision->getOrientation());
			renderer->draw(window, frame_data, feed_width, feed_height);
		}
	}
	
	delete renderer;
	delete vision;
	
	return 0;
}
