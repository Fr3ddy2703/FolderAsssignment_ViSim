#pragma once
#include "../Components/ComponentManager.h"
#include "../Components/EntityComponent/EntityComponent.h"

class Systems
{
public:
	virtual void Update(std::vector<EntityComponent> _entities);
};

