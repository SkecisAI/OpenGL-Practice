#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>

class Shader
{
public:
	unsigned int ProgramID;

	Shader(const char* VertexPath, const char* FragmentPath);

	void Use();

	void SetBool(const std::string& Name, bool Value) const;
	void SetInt(const std::string& Name, int Value) const;
	void SetFloat(const std::string& Name, float Value) const;
};

