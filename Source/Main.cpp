#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <string>

#include "Shader.h"

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
void BindTexture(unsigned int& texture);
void LoadTexture()
{
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	int width2, height2, nrChannels2;
	unsigned char* data1 = stbi_load("Source/images/container.jpg", &width, &height, &nrChannels, 0);
	unsigned char* data2 = stbi_load("Source/images/awesomeface.png", &width2, &height2, &nrChannels2, 0);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data1)
	{
		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);

		// generate all required mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data1);

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data2)
	{
		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);

		// generate all required mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	// free the image memory
	stbi_image_free(data2);
}

void BindTexture(unsigned int& texture)
{
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

/**
 * generate a vertex buffer object(VBO)
 */
void GenerateVBO(unsigned int& vbo, float vertices[], GLsizeiptr bytes_num)
{
	// generate one with ID
	glGenBuffers(1, &vbo);

	// bind target type, the buffer type of VBO is GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// copies vertex data into the buffer's memory
	glBufferData(GL_ARRAY_BUFFER, bytes_num, vertices, GL_STATIC_DRAW);

	/**
	 *	how to interpret the vertex data:, here is positions
	 *	0                  (location = 0)
	 *	3                  the size of vertex attribute (vec3)
	 *	GL_FLOAT           the type of the data
	 *	GL_FALSE           if want data to be normalized
	 *	3 * sizeof(float)  stride bytes, the space between consecutive vertex attributes
	 *	(void*)0           the offset of where the position data begins in the buffer
	 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (3 + 3 + 2) * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); 	// enable vertex attribute and give the vertex atrribute loaction

	// here is color
	glVertexAttribPointer(1, 3, GL_FLOAT, GLFW_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// here is texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GLFW_FALSE, (3 + 3 + 2) * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void GenerateVAO(unsigned int& vao)
{
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
}

void GenerateEBO(unsigned int& ebo, unsigned int indices[], GLsizeiptr bytes_num)
{
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes_num, indices, GL_STATIC_DRAW);
}

void DrawPipeline(unsigned int& vao)
{
	// vertex buffer data
	float vertices[] = {
		// positions        // colors         // texture coords
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 3, 2
	};

	unsigned int vbo, ebo;

	GenerateVAO(vao);
	GenerateVBO(vbo, vertices, sizeof(vertices)); 
	GenerateEBO(ebo, indices, sizeof(indices));
}

int main()
{
	// test
	

	// initialize glfw
	glfwInit();

	// configure glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// initialize glad
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// size of rendering window in pixels
	glViewport(0, 0, 800, 600);

	// setting pipeline
	unsigned int vao;
	DrawPipeline(vao);
	Shader shader("Source/shaders/vertex.glsl", "Source/shaders/fragment.glsl");
	shader.Use();

	// GL_LINE-wireframe, GL_FILL-default
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	LoadTexture();
	glUniform1i(glGetUniformLocation(shader.ProgramID, "texture1"), 0);
	shader.SetInt("texture2", 1);

	// transform：ratate 90 degrees around z-axis, and scale 0.5
	


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // full color of interface
		glClear(GL_COLOR_BUFFER_BIT);

		// to show two images
		glActiveTexture(GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);

		// rotate around z-axis
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		glUniformMatrix4fv(glGetUniformLocation(shader.ProgramID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		// render container
		shader.Use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean/delete glfw resources
	glfwTerminate();
	return 0;
}