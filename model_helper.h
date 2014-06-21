#ifndef MODEL_HELPER_H
#define MODEL_HELPER_H

#include <vector>

#include <GL/gl.h>
#include <glm/glm.hpp>

bool loadOBJ(
	const char* path, 
	std::vector<glm::vec3> &out_vertices, 
	std::vector<glm::vec2> &out_uvs, 
	std::vector<glm::vec3> &out_normals
);

#endif 
