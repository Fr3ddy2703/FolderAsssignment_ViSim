#include "pch.h"
#include "input.h"
#include "../Initializer//initializer.h"

void input::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void KeyBoardInput::processInput(GLFWwindow* window, std::shared_ptr<Player> player)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
if(glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS)
	{
			initializer::UseCamera.cameraPos.x += initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.x * initializer::DeltaTime;
			initializer::UseCamera.cameraPos.y += initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.y * initializer::DeltaTime;
			initializer::UseCamera.cameraPos.z += initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.z * initializer::DeltaTime;
	}
	if(glfwGetKey(window, GLFW_KEY_S)==GLFW_PRESS)
	{
			initializer::UseCamera.cameraPos.x -= initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.x * initializer::DeltaTime;
			initializer::UseCamera.cameraPos.y -= initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.y * initializer::DeltaTime;
			initializer::UseCamera.cameraPos.z -= initializer::UseCamera.cameraSpeed * initializer::UseCamera.cameraFront.z * initializer::DeltaTime;
	}
	if(glfwGetKey(window, GLFW_KEY_A)==GLFW_PRESS)
	{
			initializer::UseCamera.cameraPos -= (initializer::UseCamera.cameraSpeed * glm::normalize(glm::cross(initializer::UseCamera.cameraFront, initializer::UseCamera.cameraUp))) * initializer::DeltaTime;
	}
	if(glfwGetKey(window, GLFW_KEY_D)==GLFW_PRESS)
	{
			initializer::UseCamera.cameraPos += (initializer::UseCamera.cameraSpeed * glm::normalize(glm::cross(initializer::UseCamera.cameraFront, initializer::UseCamera.cameraUp))) * initializer::DeltaTime;
	}
}

namespace MouseInput
{
	bool firstMouse = true;
	double lastX = 0.f;
	double lastY = 0.f;
	float yaw = -90.f;
	float pitch = 0.f;
	float fov = 45.f;
}


void MouseInput::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(!initializer::UseCamera.CameraLock)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		auto xoffset = static_cast<float>(xpos - lastX);
		auto yoffset = static_cast<float>(lastY - ypos);
		lastX = xpos;
		lastY = ypos;
		const float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		yaw += xoffset;
		pitch += yoffset;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		initializer::UseCamera.cameraFront = direction;
	}
}


