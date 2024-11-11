#pragma once
#include "../Mesh.h"
#include "../../Collision/Collision.h"


enum class ECollisionType;

class Cube : Mesh
{
public:
	glm::vec3 mPosition = glm::vec3(1);
	glm::vec3 mSize = glm::vec3(1);


	Collision mCollider;
	glm::vec3& GetPosition();
	glm::vec3& GetScale();
	
	void Draw();
	//void DrawBoundingBox(unsigned int shaderProgram);
	void CreateCube(glm::vec3 _size, glm::vec3 _pos, glm::vec3 _color);
	void AddCollider(glm::vec3 _scale,ECollisionType _collisionType, glm::vec3 _offset = glm::vec3(0.f));

	void BindBuffer();

private:
	unsigned int mVBO;
	unsigned int mEBO;
	unsigned int mVAO;

};

