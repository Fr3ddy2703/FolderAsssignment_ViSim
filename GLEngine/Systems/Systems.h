#pragma once
#include "../Components/ComponentManager.h"
#include "../Components/EntityComponent/EntityComponent.h"

class Systems
{
public:
	virtual void Update(std::list<EntityComponent> _entities);
};

