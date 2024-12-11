#pragma once
#include "../ComponentManager.h"
#include "../PositionComponents/PositionComponent.h"
#include "../VelocityComponents/VelocityComponent.h"

class ComponentHandler
{
public:
	static 	ComponentHandler& getInstance()
	{
		static ComponentHandler componentHandler;
		return componentHandler;
	}

	ComponentManager<PositionComponent> positionManager;
	ComponentManager<VelocityComponent> VelocityManager;
};

