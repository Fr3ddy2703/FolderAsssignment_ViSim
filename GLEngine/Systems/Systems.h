#pragma once
#include "../Components/Entity/Entity.h"

class Systems
{
public:
	virtual void Update(const std::vector<Entity>& _entities, float _deltatime) = 0;
};

