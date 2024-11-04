#pragma once
#include "../Components.h"


struct EntityComponent : public Components
{
	/* Public Functions */

	EntityComponent() = default;

	/* Public Variables */
	int EntityId;
	std::string EntityName;
};

