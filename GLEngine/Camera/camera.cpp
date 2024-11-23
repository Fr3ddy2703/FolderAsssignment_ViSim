#include "pch.h"
#include "camera.h"
#include "../Shaders/shader.h"

void camera::initCamera()
{
    mViewLoc = glGetUniformLocation(Shader::ShaderProgram, "view");
    mProjectionLoc = glGetUniformLocation(Shader::ShaderProgram, "projection");
}

glm::mat4 camera::getView()
{
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(mCameraPos, mCameraPos+mCameraFront, mCameraUp);
    return view;
}

glm::mat4 camera::getProjection(float _width, float _height)
{
    glm::mat4 proj = glm::mat4(1.f);
    proj = glm::perspective(glm::radians(45.0f), _width / _height, 0.1f, 1000.0f);
    return proj;
}

void camera::setPlayerPos(glm::vec3 _position)
{
	mPlayerPos = glm::vec3(_position.x + 0.375f, _position.y + 0.65f, _position.z - 0.375f);
}

glm::vec3 camera::orbitCamera(glm::vec3 _direction)
{
    mCameraPos = mPlayerPos;
	mCameraFront = glm::normalize(_direction);
	mCameraPos = glm::vec3(mPlayerPos) + mCameraFront * -3.f;
    return mCameraPos;
}

void camera::updatePos(glm::vec3 _newpos)
{
	mCameraPos += _newpos;
    mCameraFront += _newpos;
}
