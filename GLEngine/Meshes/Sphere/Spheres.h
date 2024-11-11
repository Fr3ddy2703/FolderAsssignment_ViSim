#pragma once
#include "../Mesh.h"
#include "../../Collision/Collision.h"

enum class ECollisionType;

class Spheres : public Mesh
{
public:
	glm::vec3 mPosition = glm::vec3(0.f);
	glm::vec3 mSize = glm::vec3(1.f);
	float mMass = 0.f;
	glm::vec3 mColor = Color::Gold;
	glm::vec3 mVelocity = glm::vec3(1.f, 0.f, 0.f);

	Spheres() = default;
	~Spheres() = default;

	glm::vec3& GetPosition();
	glm::vec3& GetScale();

	Collision mCollider;

	void CreateSphere(glm::vec3 _size, float _segment, glm::vec3 _pos, float _mass,glm::vec3 _speed, glm::vec3 _color);
	void SubDivide(int _index1, int _index2, int _index3, int _n);

	void DrawSphere();

	void UpdatePos(float _dt);

	void AddCollider(glm::vec3 _scale,ECollisionType _collisionType, glm::vec3 _offset = glm::vec3(0.f));

	void BindBuffer();

private:
	unsigned int mVBO;
	unsigned int mEBO;
	unsigned int mVAO;

};

