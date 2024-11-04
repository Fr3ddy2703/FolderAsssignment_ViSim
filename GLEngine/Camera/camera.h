#pragma once
#define CAMERA_H


class Mesh;
class Player;

class camera
{
public:

    glm::vec3 cameraFront = glm::vec3 (0.0f, 0.0f,-1.0f);
	glm::vec3 cameraUp = glm::vec3 (0.0f, 1.0f, 0.0f);
	glm::vec3 PlayerPos = glm::vec3(0.375f, 1.15f, -0.375f);
	glm::vec3 pCamerPos = glm::vec3(PlayerPos) + cameraFront;
	glm::vec3 cameraPos = glm::vec3(10.f, 3.f, 5.f) + cameraFront * -3.f;
    float cameraSpeed = 3.f;


    int viewLoc;
    int projectionLoc;
    bool CameraLock = false;
    bool FirstPersCam = false;
    
    void initCamera();

    glm::mat4 view;
    glm::mat4 projection;
    
    glm::mat4 getView();
	glm::mat4 getProjection(float width, float height);
    void setPlayerPos(glm::vec3 _position);
	glm::vec3 orbitCamera(glm::vec3 direction);

    void updatePos(glm::vec3 _newpos);
};




