#version 330 core

uniform mat4 u_MVPMatrix;

layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec2 i_TextureCoordinates;

out vec2 v_TextureCoordinates;

void main()
{
	gl_Position = u_MVPMatrix * vec4(i_Position, 1);
	
	// Vertical axis inverted for DDS textures
	v_TextureCoordinates = vec2(i_TextureCoordinates.x, -i_TextureCoordinates.y);
}
