#pragma once
#define CAMERA_H


class Mesh;
class Player;

class camera
{
public:

    glm::vec3 mCameraFront = glm::vec3 (0.0f, 0.0f,-1.0f);
	glm::vec3 mCameraUp = glm::vec3 (0.0f, 1.0f, 0.0f);
	glm::vec3 mPlayerPos = glm::vec3(0.375f, 1.15f, -0.375f);
	glm::vec3 mPCamerPos = glm::vec3(mPlayerPos) + mCameraFront;
	glm::vec3 mCameraPos = glm::vec3(10.f, 3.f, 5.f) + mCameraFront * -3.f;
    float mCameraSpeed = 3.f;


    int mViewLoc;
    int mProjectionLoc;
    bool mCameraLock = false;
    bool mFirstPersCam = false;
    
    void initCamera();

    glm::mat4 mView;
    glm::mat4 mProjection;
    
    glm::mat4 getView();
	glm::mat4 getProjection(float _width, float _height);
    void setPlayerPos(glm::vec3 _position);
	glm::vec3 orbitCamera(glm::vec3 _direction);

    void updatePos(glm::vec3 _newpos);
};




