#pragma once
#include "../Systems.h"
#include "../../Components/PositionComponents/PositionComponent.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/VelocityComponents/VelocityComponent.h"

class MovementSystem : Systems
{
public:

	MovementSystem(ComponentManager<PositionComponent>& _positionManager, ComponentManager<VelocityComponent>& _velocityManager) :
	positionManager(_positionManager), velocityManager(_velocityManager){}


   void Update(const std::vector<Entity>& _entities, float _deltatime) override {
		for (const auto& entity : _entities)
		{
			if (positionManager.hasComponent(entity.EntityId))
			{
				PositionComponent& position = positionManager.getComponent(entity.EntityId);
				VelocityComponent& velocity = velocityManager.getComponent(entity.EntityId);
				/*std::cout << "Before update: " << position.mPosition.x << ", " << position.mPosition.y << ", " << position.mPosition.z << std::endl;*/
				position.mPosition += velocity.mVelocity * _deltatime;
				/*std::cout << "After update: " << position.mPosition.x << ", " << position.mPosition.y << ", " << position.mPosition.z << std::endl;*/
			}
		}

    }


private:
	ComponentManager<PositionComponent>& positionManager;
	ComponentManager<VelocityComponent>& velocityManager;
};
