#pragma once
#include "../Systems.h"
#include "../../Components/PositionComponents/PositionComponent.h"

class UpdateSystem : Systems
{
public:

	//UpdateSystem(ComponentManager<PositionComponent> _positionManager)
	//{
	//	this->mPositionManager = _positionManager;
	//}


	//void Update(std::vector<EntityComponent> _entities) override
	//{
	//	for (auto& entity : _entities)
	//	{
	//		if (mPositionManager.hasComponent(entity.EntityId))
	//		{
	//			//glm::vec3 position = mPositionManager.getComponent(entity.EntityId).mPosition;

	//			//for (int i = 0; i < position; i++)
	//			//{
	//			//	position.x += 1;
	//			//	position.y += 1;
	//			//	position.z += 1;
	//			//}
	//			//mPositionManager.addComponent(entity.EntityId, position);
	//		}
	//	}
	//}


private:
	ComponentManager<PositionComponent> mPositionManager;
};
