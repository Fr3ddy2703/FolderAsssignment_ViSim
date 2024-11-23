#include "pch.h"
#include "input.h"
#include "../Initializer//initializer.h"

void input::framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
	glViewport(0, 0, _width, _height);
}

void KeyBoardInput::processInput(GLFWwindow* _window, std::shared_ptr<Player> _player)
{
    if(glfwGetKey(_window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
    }
	if(glfwGetKey(_window, GLFW_KEY_W)==GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraPos.x += initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.x * initializer::mDeltaTime;
		initializer::mUseCamera.mCameraPos.y += initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.y * initializer::mDeltaTime;
		initializer::mUseCamera.mCameraPos.z += initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.z * initializer::mDeltaTime;
	}
	if(glfwGetKey(_window, GLFW_KEY_S)==GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraPos.x -= initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.x * initializer::mDeltaTime;
		initializer::mUseCamera.mCameraPos.y -= initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.y * initializer::mDeltaTime;
		initializer::mUseCamera.mCameraPos.z -= initializer::mUseCamera.mCameraSpeed * initializer::mUseCamera.mCameraFront.z * initializer::mDeltaTime;
	}
	if(glfwGetKey(_window, GLFW_KEY_A)==GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraPos -= (initializer::mUseCamera.mCameraSpeed * glm::normalize(glm::cross(initializer::mUseCamera.mCameraFront, initializer::mUseCamera.mCameraUp))) * initializer::mDeltaTime;
	}
	if(glfwGetKey(_window, GLFW_KEY_D)==GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraPos += (initializer::mUseCamera.mCameraSpeed * glm::normalize(glm::cross(initializer::mUseCamera.mCameraFront, initializer::mUseCamera.mCameraUp))) * initializer::mDeltaTime;
	}


	if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraSpeed = glm::clamp(initializer::mUseCamera.mCameraSpeed + 1.0f, initializer::mUseCamera.mMinSpeed, initializer::mUseCamera.mMaxSpeed);
	}

	if (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS)
	{
		initializer::mUseCamera.mCameraSpeed = initializer::mUseCamera.mDefaultSpeed;
	}
	if (glfwGetKey(_window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};
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


void MouseInput::mouse_callback(GLFWwindow* _window, double _xpos, double _ypos)
{
	if(!initializer::mUseCamera.mCameraLock)
	{
		if (firstMouse)
		{
			lastX = _xpos;
			lastY = _ypos;
			firstMouse = false;
		}
		auto xoffset = static_cast<float>(_xpos - lastX);
		auto yoffset = static_cast<float>(lastY - _ypos);
		lastX = _xpos;
		lastY = _ypos;
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
		initializer::mUseCamera.mCameraFront = direction;
	}
}


