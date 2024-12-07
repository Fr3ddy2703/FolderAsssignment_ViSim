#pragma once

class ParticleSystem
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
	std::vector<glm::vec4> mColor;
	int mMaxParticles;
	float mGravity = 1.f;
	unsigned int mVAO, mVBO;

};