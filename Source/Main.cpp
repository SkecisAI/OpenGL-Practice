#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

std::string ReadShaderFile(const char* path)
{
	std::string content;
	std::ifstream file_stream(path, std::ios::in);

	if (!file_stream.is_open())
	{
		std::cerr << "could not read file: " << path << ". file does not exist.\n";
		return "";
	}

	std::string line;
	while (!file_stream.eof())
	{
		std::getline(file_stream, line);
		content.append(line + "\n");
	}

	file_stream.close();
	return content;
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
	 *	how to interpret the vertex data:
	 *	0                  (location = 0)
	 *	3                  the size of vertex attribute (vec3)
	 *	GL_FLOAT           the type of the data
	 *	GL_FALSE           if want data to be normalized
	 *	3 * sizeof(float)  stride, the space between consecutive vertex attributes
	 *	(void*)0           the offset of where the position data begins in the buffer
	 */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));

	// enable vertex attribute and give the vertex atrribute loaction
	glEnableVertexAttribArray(0);
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

void GenerateVertexShader(unsigned int& vertex_shader)
{
	// read shader source
	const std::string source = ReadShaderFile("Source/shaders/vertex.glsl");
	const char* vertex_shader_source = source.c_str();

	// create a shader object
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	// attach shader source to the shader object
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);

	// compile shader
	glCompileShader(vertex_shader);

	// check if compilation was successful
	int success;
	char log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, nullptr, log);
		std::cout << "error while compiling vertex shader: \n" << log << std::endl;
	}
}

void GenerateFragmentShader(unsigned int& fragment_shader)
{
	// read shader source
	const std::string source = ReadShaderFile("Source/shaders/fragment.glsl");
	const char* fragment_shader_source = source.c_str();

	// create a shader object
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// attach shader source
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

	// compile shader
	glCompileShader(fragment_shader);

	// check if compilation was successful
	int success;
	char log[512];
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, nullptr, log);
		std::cout << "error while compiling fragment shader: \n" << log << std::endl;
	}
}

void GenerateShaders(unsigned int& shader_program)
{
	// shader's ID
	unsigned int vertex_shader;
	unsigned int fragment_shader;

	// generate shaders
	GenerateVertexShader(vertex_shader);
	GenerateFragmentShader(fragment_shader);

	// create a program
	shader_program = glCreateProgram();

	// attach compiled shaders to the program
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	// link them all
	glLinkProgram(shader_program);

	// check if linking was successful
	int success;
	char log[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, nullptr, log);
		std::cout << "error while linking shader program: \n" << log << std::endl;
	}

	// delete shaders after linking, no longer need them anymore
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void DrawPipeline()
{
	// vertex buffer data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // stride = 3 * sizeof(float)
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	float rectangles[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int vbo;
	unsigned int vao;
	unsigned int ebo;
	unsigned int shader_program;

	GenerateVAO(vao);
	glBindVertexArray(vao);

	//GenerateVBO(vbo, vertices, sizeof(vertices));      // draw triangle
	GenerateVBO(vbo, rectangles, sizeof(rectangles));  // draw rectangle

	GenerateEBO(ebo, indices, sizeof(indices));

	GenerateShaders(shader_program);

	glUseProgram(shader_program);
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

	// GL_LINE-wireframe, GL_FILL-default
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean/delete glfw resources
	glfwTerminate();
	return 0;
}
