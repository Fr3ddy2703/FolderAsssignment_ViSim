#pragma once

class Player;
class Camera;
struct GLFWwindow;

class input
{
public:
	static void framebuffer_size_callback(GLFWwindow* _window, int _width,int _height);
};

class KeyBoardInput
{
public:
	static void processInput(GLFWwindow* _window, std::shared_ptr<Player> _player);
};

namespace MouseInput
{
	extern bool firstMouse;

	extern double lastX;
	extern double lastY;

	extern float yaw;
	extern float pitch;

	extern float fov;

	void mouse_callback(GLFWwindow* _window, double _posX, double _posY);
}

