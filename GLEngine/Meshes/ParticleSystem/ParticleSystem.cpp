#include "pch.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(int _maxParticles) : mMaxParticles(_maxParticles)
{
	mPosition.reserve(mMaxParticles);
	mVelocity.reserve(mMaxParticles);
	mLifeSpan.reserve(mMaxParticles);
}

void ParticleSystem::update(float _deltaTime)
{
	for (int i = 0; i < mLifeSpan.size(); ++i)
	{
        mVelocity[i] += glm::vec3(glm::linearRand(-0.01f, 0.01f), -mGravity * _deltaTime, glm::linearRand(-0.01f, 0.01f));
		mPosition[i] -= mVelocity[i] * _deltaTime;
		mLifeSpan[i] -= _deltaTime;

		if (mLifeSpan[i] <= 0.0f)
		{
			mPosition.erase(mPosition.begin() + i);
			mVelocity.erase(mVelocity.begin() + i);
			mLifeSpan.erase(mLifeSpan.begin() + i);
			--i;
		}
	}
	/*std::cout << mLifeSpan.size() << std::endl;*/
}

void ParticleSystem::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mPosition.size() * sizeof(glm::vec3), mPosition.data());

	glBindVertexArray(mVAO);
	glDrawArrays(GL_POINTS, 0, mPosition.size());
	glBindVertexArray(0);
	glPointSize(5.f);

}

void ParticleSystem::emit(const glm::vec3& _position, float _lifeSpan)
{
	if (mPosition.size() < mMaxParticles)
	{
		glm::vec3 randomOffset = glm::linearRand(glm::vec3(-5.0f, 0.0f, -5.0f), glm::vec3(5.0f, 0.0f, 5.0f));
        glm::vec3 startPosition = _position + randomOffset;

		glm::vec3 randomVelocity = glm::vec3(glm::linearRand(-0.1f, 0.1f), glm::linearRand(0.5f, 1.5f), glm::linearRand(-0.1f, 0.1f));
			mPosition.emplace_back(startPosition);
			mVelocity.emplace_back(randomVelocity);
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

void ParticleSystem::UpdateBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
}
