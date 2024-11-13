#pragma once

struct ShaderSource
{
	std::string mVertSource;
	std::string mFragSource;
};

namespace Shader
{
	extern GLuint ShaderProgram;
	ShaderSource ReadShaderFromSource(const char* _filepathVert, const char* _filepathFrag);
	void CreateProgram();
}
