#pragma once
#include "../Components.h"

class VelocityComponent : Components

{
public:
	std::vector<glm::vec3> velocity = { glm::vec3(1.f) };

	VelocityComponent(std::vector<glm::vec3> vel) : velocity(vel)
	{
		velocity = vel;
	}
};

