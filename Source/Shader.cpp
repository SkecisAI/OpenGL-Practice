#include "Shader.h"

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

void GenerateVertexShader(unsigned int& ShaderID, const char* ShaderSource, GLenum ShaderType)
{
	// create a shader object
	ShaderID = glCreateShader(ShaderType);

	// attach shader source to the shader object
	glShaderSource(ShaderID, 1, &ShaderSource, nullptr);

	// compile shader
	glCompileShader(ShaderID);

	// check if compilation was successful
	int success;
	char log[512];
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(ShaderID, 512, nullptr, log);
		std::cout << "error while compiling shader: \n" << log << std::endl;
	}
}

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	std::string VertexShaderStr = ReadShaderFile(VertexPath);
	std::string FragmentShaderStr = ReadShaderFile(FragmentPath);

	unsigned int VertexID, FragmentID;
	GenerateVertexShader(VertexID, VertexShaderStr.c_str(), GL_VERTEX_SHADER);
	GenerateVertexShader(FragmentID, FragmentShaderStr.c_str(), GL_FRAGMENT_SHADER);

	ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexID);
	glAttachShader(ProgramID, FragmentID);
	glLinkProgram(ProgramID);

	// check if linking was successful
	int success;
	char log[512];
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ProgramID, 512, nullptr, log);
		std::cout << "error while linking shader program: \n" << log << std::endl;
	}

	// delete shaders after linking, no longer need them anymore
	glDeleteShader(VertexID);
	glDeleteShader(FragmentID);
}

void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::SetBool(const std::string& Name, bool Value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), (int)Value);
}

void Shader::SetInt(const std::string& Name, int Value) const
{
	glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), Value);
}

void Shader::SetFloat(const std::string& Name, float Value) const
{
	glUniform1f(glGetUniformLocation(ProgramID, Name.c_str()), Value);
}
