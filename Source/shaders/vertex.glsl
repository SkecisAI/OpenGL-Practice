#version 330 core

// define a INPUT var named aPos, its LOCATION = 0, its TYPE is vec3
layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
