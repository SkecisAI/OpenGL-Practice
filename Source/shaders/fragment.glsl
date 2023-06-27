#version 330 core

// requires only one output variable
out vec4 FragColor;

// same name in vertex shader
in vec3 vertexColor;
in vec2 TexCoord;

// uniform vec4 ourColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	// FragColor = texture(texture1, TexCoord) * vec4(vertexColor, 1.0);
	FragColor = mix(texture(texture1, TexCoord), 
	                texture(texture2, TexCoord), 0.2);
}