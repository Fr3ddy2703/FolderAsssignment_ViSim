#pragma once

class ParticleSystem
{

public:
	ParticleSystem(int _maxParticles);
	void update(float _deltaTime);
	void draw();
	void emit(const glm::vec3& _position, float _lifeSpan);
	void BindBuffer();
	void UpdateBuffer();

private:
	std::vector<glm::vec3> mPosition;
	std::vector<glm::vec3> mVelocity;
	std::vector<float> mLifeSpan;
	int mMaxParticles;
	float mGravity = -0.5f;
	unsigned int mVAO, mVBO;

};
