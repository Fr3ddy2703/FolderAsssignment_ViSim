#include "pch.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(int _maxParticles) : mMaxParticles(_maxParticles)
{	mPosition.reserve(mMaxParticles);
	mVelocity.reserve(mMaxParticles);
	mLifeSpan.reserve(mMaxParticles);
}

void ParticleSystem::update(float _deltaTime)
{
	for (int i = 0; i < mLifeSpan.size(); ++i)
	{
		mVelocity[i].y -= mGravity / 8.f * _deltaTime;
		mPosition[i] -= mVelocity[i] * _deltaTime;
		mLifeSpan[i] -= _deltaTime;

		if (mLifeSpan[i] <= 0.0f)
		{
			int last = mLifeSpan.size() - 1;
			mPosition[i] = mPosition[last];
			mVelocity[i] = mVelocity[last];
			mLifeSpan[i] = mLifeSpan[last];
			mPosition.pop_back();
			mVelocity.pop_back();
			mLifeSpan.pop_back();
			--i;
		}
	}
	std::cout << mLifeSpan.size() << std::endl;
	BindBuffer();
}

void ParticleSystem::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mPosition.size() * sizeof(glm::vec3), mPosition.data());

	glBindVertexArray(mVAO);
	glDrawArrays(GL_POINTS, 0, mPosition.size());
	glBindVertexArray(0);
}

void ParticleSystem::emit(const glm::vec3& _position, float _lifeSpan)
{
	if (mPosition.size() < mMaxParticles)
	{
			mPosition.emplace_back(_position);
			mVelocity.emplace_back(glm::sphericalRand(1.f));
			mLifeSpan.emplace_back(_lifeSpan);
	}
}

void ParticleSystem::BindBuffer()
{
    // VAO
    glGenBuffers(1, &mVBO);

    // VAO
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);


    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
}


