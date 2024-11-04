#pragma once

struct ShaderSource
{
	std::string VertSource;
	std::string FragSource;
};

namespace Shader
{
	extern GLuint ShaderProgram;
	ShaderSource ReadShaderFromSource(const char* filepathVert, const char* filepathFrag);
	void CreateProgram();
}
