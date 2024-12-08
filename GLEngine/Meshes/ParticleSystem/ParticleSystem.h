#pragma once
#include "../Mesh.h"

class ParticleSystem : public Mesh
{

public:
	ParticleSystem(int _maxParticles);
	void update(float _deltaTime);
	void draw();
	void emit(const glm::vec3& _position, float _lifeSpan);
	void BindBuffer();

private:
	std::vector<glm::vec3> mPosition;
	std::vector<glm::vec3> mVelocity;
	std::vector<float> mLifeSpan;
	glm::vec3 mColor;
	int mMaxParticles;
	float mGravity = -0.5f;
	unsigned int mVAO, mVBO;

};
