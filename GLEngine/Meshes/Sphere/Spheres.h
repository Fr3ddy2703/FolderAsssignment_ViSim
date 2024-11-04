#pragma once
#include "../Mesh.h"
#include "../../Collision/Collision.h"

enum class ECollisionType;

class Spheres : Mesh
{
	public:
	std::vector<Vertex> bVertices;
	std::vector<Triangle> bIndices;
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 size = glm::vec3(1.f);
	float m = 0.f;
	glm::vec3 color = Color::Gold;
	glm::vec3 velocity = glm::vec3(1.f, 0.f, 0.f);

	glm::vec3& GetPosition();
	glm::vec3& GetScale();

	Collision Collider;

	void CreateSphere(glm::vec3 size, float segment, glm::vec3 pos, float mass,glm::vec3 speed, glm::vec3 color);
	void SubDivide(int index1, int index2, int index3, int n);

	void BindBuffer();

	void DrawSphere();

	void UpdatePos(float dt);

	void AddCollider(glm::vec3 scale,ECollisionType collisionType, glm::vec3 offset = glm::vec3(0.f));

private:
	unsigned int VBO;
	unsigned int EBO;
	unsigned int VAO;

};

