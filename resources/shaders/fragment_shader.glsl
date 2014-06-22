#version 330 core

uniform sampler2D u_TextureUnit;

in vec2 v_TextureCoordinates;

out vec3 o_Colour;

void main()
{
	o_Colour = texture2D(u_TextureUnit, v_TextureCoordinates).rgb;
}
