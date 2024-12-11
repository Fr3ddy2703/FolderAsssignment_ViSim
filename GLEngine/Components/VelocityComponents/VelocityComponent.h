#pragma once
#include "../Components.h"

class VelocityComponent : Components

{
public:
	VelocityComponent(glm::vec3 _velocity) : mVelocity(_velocity){}
	VelocityComponent(){}
	glm::vec3 mVelocity;

};

