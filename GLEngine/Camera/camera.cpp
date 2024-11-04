#include "pch.h"
#include "camera.h"
#include "../Shaders/shader.h"

void camera::initCamera()
{
    viewLoc = glGetUniformLocation(Shader::ShaderProgram, "view");
    projectionLoc = glGetUniformLocation(Shader::ShaderProgram, "projection");
}

glm::mat4 camera::getView()
{
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    return view;
}

glm::mat4 camera::getProjection(float width, float height)
{
    glm::mat4 proj = glm::mat4(1.f);
    proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    return proj;
}

void camera::setPlayerPos(glm::vec3 _position)
{
	PlayerPos = glm::vec3(_position.x + 0.375f, _position.y + 0.65f, _position.z - 0.375f);
}

glm::vec3 camera::orbitCamera(glm::vec3 direction)
{
    cameraPos = PlayerPos;
	cameraFront = glm::normalize(direction);
	cameraPos = glm::vec3(PlayerPos) + cameraFront * -3.f;
    return cameraPos;
}

void camera::updatePos(glm::vec3 _newpos)
{
	cameraPos += _newpos;
    cameraFront += _newpos;
}
