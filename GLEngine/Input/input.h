#pragma once

class Player;
class Camera;
struct GLFWwindow;

class input
{
public:
	static void framebuffer_size_callback(GLFWwindow* window, int width,int height);
};

class KeyBoardInput
{
public:
	static void processInput(GLFWwindow* window, std::shared_ptr<Player> player);
};

namespace MouseInput
{
	extern bool firstMouse;

	extern double lastX;
	extern double lastY;

	extern float yaw;
	extern float pitch;

	extern float fov;

	void mouse_callback(GLFWwindow* window, double posX, double posY);
}

