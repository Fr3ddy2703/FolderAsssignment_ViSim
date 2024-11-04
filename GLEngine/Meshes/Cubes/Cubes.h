#pragma once
#include "../Mesh.h"
#include "../../Collision/Collision.h"


enum class ECollisionType;

class Cube : Mesh
{
public:
	std::vector<Vertex> mVertices;
	std::vector<Triangle> mIndices;
	glm::vec3 position = glm::vec3(1);
	glm::vec3 size = glm::vec3(1);


	Collision Collider;
	glm::vec3& GetPosition();
	glm::vec3& GetScale();
	
	void Draw();
	//void DrawBoundingBox(unsigned int shaderProgram);
	void CreateCube(glm::vec3 size, glm::vec3 pos, glm::vec3 color);
	void AddCollider(glm::vec3 scale,ECollisionType collisionType, glm::vec3 offset = glm::vec3(0.f));

	void BindBuffer();

	

private:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;

};

