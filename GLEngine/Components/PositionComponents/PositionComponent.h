#pragma once
#include "../Components.h"
class Player;

class PositionComponent : Components
{
public:
	std::vector<glm::vec3> position = { glm::vec3(1.f) };
	std::vector<std::shared_ptr<Player>> player;

	PositionComponent(glm::vec3 Position);
};

