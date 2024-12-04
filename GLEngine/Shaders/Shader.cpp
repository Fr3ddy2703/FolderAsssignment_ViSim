#include "pch.h"
#include "Shader.h"

ShaderSource Shader::ReadShaderFromSource(const char* _filepathVert, const char* _filepathFrag)
{
	ShaderSource currentSource;
	std::ifstream InFileVert(_filepathVert);
	std::ifstream InFileFrag(_filepathFrag);
	std::stringstream ssVert;
	std::stringstream ssFrag;
	if (InFileVert.is_open()&&InFileFrag.is_open())
	{
		ssVert << InFileVert.rdbuf();
		ssFrag << InFileFrag.rdbuf();
		currentSource.mVertSource = ssVert.str();
		currentSource.mFragSource = ssFrag.str();
		return currentSource;
	}
	return {};
}

GLuint Shader::ShaderProgram;

void Shader::CreateProgram()
{
	ShaderSource source = ReadShaderFromSource("Shaders/VertexShader.vert", "Shaders/FragmentShader.frag");
	ShaderProgram = glCreateProgram();
	const char* VertexSource = source.mVertSource.c_str();
	const char* FragmentSource = source.mFragSource.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	int good;
	char infoLog[512];
	glShaderSource(vs, 1, &VertexSource, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &good);
	if (!good)
	{
		glGetShaderInfoLog(vs, 512, nullptr, infoLog);
		std::cout << "Vertex Compilation failed\n" << infoLog << "\n";
	}
	glShaderSource(fs, 1, &FragmentSource, nullptr);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &good);
	if (!good)
	{
		glGetShaderInfoLog(fs, 512, nullptr, infoLog);
		std::cout << "Fragment Compilation failed\n" << infoLog << "\n";
	}
	glAttachShader(ShaderProgram, vs);
	glAttachShader(ShaderProgram, fs);
	glLinkProgram(ShaderProgram);
	glDeleteShader(vs);
	glDeleteShader(fs);
}