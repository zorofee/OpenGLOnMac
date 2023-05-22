#version 330 core

// shader inputs
in vec2 texture_coords;

// shader outputs
out vec4 frag;

// screen image
uniform sampler2D screen;

void main()
{
	frag = vec4(texture(screen, texture_coords).rgb, 1.0f);
}