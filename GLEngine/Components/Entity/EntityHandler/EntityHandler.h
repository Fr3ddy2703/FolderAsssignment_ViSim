#pragma once
#include "../Entity.h"
#include "../EntityHandler/EntityHandler.h"
class EntityHandler
{
public:
	static EntityHandler& getInstance()
	{
		static EntityHandler entityHandler;
		return entityHandler;
	}

	std::vector<Entity> entities;
};

