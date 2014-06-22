#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Model
{
	public:
		Model(const char* path, const char* texture_path);
		~Model();
		
		void setPosition(glm::vec3 position);
		
		glm::vec3 getPosition();
		GLuint getTextureID();
		
		void draw();
		
	private:
		glm::vec3 position;
	
		GLuint vertex_array_id;
		GLuint texture_id;
		
		GLuint vertex_buffer;
		GLuint uv_buffer;
		GLuint normal_buffer;
		
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
};
#endif 
