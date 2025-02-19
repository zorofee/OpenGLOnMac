#version 330 core

// shader outputs
 out vec4 frag;

// material color
uniform vec3 color;

void main()
{
	frag = vec4(color, 1.0f);
}