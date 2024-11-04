#include "pch.h"
#include "window.h"
#include "../Input/input.h"

#include "../Shaders/shader.h"

float window::width = 1920.f;
float window::height = 1080.f;

GLFWwindow* window::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	
	window = glfwCreateWindow(width, height, "Compulsory 2", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to initialize window" << std::endl;
		glfwTerminate();
		return nullptr;
	}	
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
	}
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	Shader::CreateProgram();
	glfwSetFramebufferSizeCallback(window, input::framebuffer_size_callback);
	glfwSetCursorPosCallback(window, MouseInput::mouse_callback);
	return window;
}


