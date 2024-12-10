#pragma once
#include "../Systems.h"
#include "../../Components/PositionComponents/PositionComponent.h"
#include "../../Components/ComponentManager.h"

class MovementSystem : Systems
{
public:

	MovementSystem(ComponentManager<PositionComponent>& _positionManager) :
	positionManager(_positionManager){}

   void Update(const std::vector<Entity>& _entities, float _deltatime) override {
		for (const auto& entity : _entities)
		{
			if (positionManager.hasComponent(entity.EntityId))
			{
				PositionComponent& position = positionManager.getComponent(entity.EntityId);
				std::cout << "Before update: " << position.mPosition.x << ", " << position.mPosition.y << ", " << position.mPosition.z << std::endl;
				position.mPosition += glm::vec3(1.f) * _deltatime;
				std::cout << "After update: " << position.mPosition.x << ", " << position.mPosition.y << ", " << position.mPosition.z << std::endl;
			}
		}

    }


private:
	ComponentManager<PositionComponent>& positionManager;
};
