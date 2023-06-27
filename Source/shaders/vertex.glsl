#version 330 core

// define a INPUT var named aPos, its LOCATION = 0, its TYPE is vec3
layout (location = 0) in vec3 aPos;

// input color, its location = 1, 
layout (location = 1) in vec3 aColor;

// input text coord
layout (location = 2) in vec2 aTexCoord;

// The fragment shader only requires one output variable and that is a vector of size 4 that defines
// the final color output that we should calculate ourselves.
// same name in fragment shader
out vec3 vertexColor;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);

	vertexColor = aColor;
	TexCoord = aTexCoord;
}