/**
* Author		Jaween Ediriweera
* Student ID	a1647468
* Filename		model_helper.cpp
* Description	Loads 3D models
**/

#include "model_helper.h"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

bool loadOBJ(const char* path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
{
	std::vector<unsigned int> vertex_indices;
	std::vector<unsigned int> uv_indices;
	std::vector<unsigned int> normal_indices;
	
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	
	// Opens file
	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		std::cout << "Couldn't open " << path << std::endl;
		return false;
	}
	
	// Reads file and changes it's shape
	while(true)
	{
		// Reads until End Of File
		char line_header[128];
		int line = fscanf(file, "%s", line_header);
		if (line == EOF)
			break;
		
		if (strcmp(line_header, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		} else if (strcmp(line_header, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		} else if (strcmp(line_header, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		} else if (strcmp(line_header, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertex_index[3], uv_index[3], normal_index[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex_index[0], &uv_index[0], &normal_index[0], &vertex_index[1], &uv_index[1], &normal_index[1], &vertex_index[2], &uv_index[2], &normal_index[2]);
			if (matches != 9)
			{
				std::cout << "File wasn't standard" << std::endl;
				return false;
			}
			vertex_indices.push_back(vertex_index[0]);
			vertex_indices.push_back(vertex_index[1]);
			vertex_indices.push_back(vertex_index[2]);
			uv_indices.push_back(uv_index[0]);
			uv_indices.push_back(uv_index[1]);
			uv_indices.push_back(uv_index[2]);
			normal_indices.push_back(normal_index[0]);
			normal_indices.push_back(normal_index[1]);
			normal_indices.push_back(normal_index[2]);
		}
	}
	
	for(unsigned int i = 0; i < vertex_indices.size(); i++)
	{
		unsigned int vertex_index = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertex_index - 1];
		out_vertices.push_back(vertex);
		
		unsigned int uv_index = uv_indices[i];
		glm::vec2 uv = temp_uvs[uv_index - 1];
		out_uvs.push_back(uv);
		
		unsigned int normal_index = normal_indices[i];
		glm::vec3 normal = temp_normals[normal_index - 1];
		out_normals.push_back(normal);
	}
	
	return true;	
}
