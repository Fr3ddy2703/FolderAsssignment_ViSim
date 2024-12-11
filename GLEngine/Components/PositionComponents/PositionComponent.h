#pragma once
#include "../Components.h"

struct PositionComponent : Components
{
public:

	PositionComponent(glm::vec3 _position) : mPosition(_position){}
	PositionComponent(){}
	glm::vec3 mPosition;
	
};

