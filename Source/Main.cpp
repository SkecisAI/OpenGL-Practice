#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) + 3 * sizeof(float), (void*)0);

	// enable vertex attribute and give the vertex atrribute loaction
	glEnableVertexAttribArray(0);

	// here is color
	glVertexAttribPointer(1, 3, GL_FLOAT, GLFW_FALSE, 3 * sizeof(float) + 3 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void GenerateVAO(unsigned int& vao)
{
	glGenVertexArrays(1, &vao);
}

void GenerateEBO(unsigned int& ebo, unsigned int indices[], GLsizeiptr bytes_num)
{
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytes_num, indices, GL_STATIC_DRAW);
}

void DrawPipeline()
{
	// vertex buffer data
	float vertices[] = {
		// positions        // colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	//float rectangles[] = {
	//	 0.5f,  0.5f, 0.0f,
	//	 0.5f, -0.5f, 0.0f,
	//	-0.5f, -0.5f, 0.0f,
	//	-0.5f,  0.5f, 0.0f
	//};
	unsigned int indices[] = {
		//0, 1, 3,
		//1, 2, 3
		0, 1, 2 // triangle
	};

	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;

	GenerateVAO(vao);
	glBindVertexArray(vao);

	GenerateVBO(vbo, vertices, sizeof(vertices));      // draw triangle
	//GenerateVBO(vbo, rectangles, sizeof(rectangles));  // draw rectangle

	GenerateEBO(ebo, indices, sizeof(indices));
}

int main()
{
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
	DrawPipeline();
	Shader shader("Source/shaders/vertex.glsl", "Source/shaders/fragment.glsl");
	shader.Use();

	// GL_LINE-wireframe, GL_FILL-default
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // full color of interface
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//float TimeValue = glfwGetTime();
		//float GreenColor = (sin(TimeValue) / 2.0f) + 0.5f;
		//int VertexColorLocation = glGetUniformLocation(shader_program, "ourColor");
		//glUniform4f(VertexColorLocation, 0.0f, GreenColor, 0.0f, 1.0f);

		// specified 6 indices
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean/delete glfw resources
	glfwTerminate();
	return 0;
}
