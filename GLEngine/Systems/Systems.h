#pragma once
#include "../Components/ComponentManager.h"

class Systems
{
public:
	virtual void Update() = 0;
	Systems() = default;
};

