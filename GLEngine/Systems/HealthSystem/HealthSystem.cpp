#include "pch.h"
#include "HealthSystem.h"

#include "../../Actor/Actor.h"


//HealthSystem::HealthSystem(ComponentManager<HealthComponent> HealthManager)
//{
//	this->HealthManager = HealthManager;
//}
//
//int HealthSystem::GetHealth(int _entityID)
//{
//	return HealthManager.getComponent(_entityID).mCurrentHealth;
//}

//int HealthSystem::GetHealth(int _entityID)
//{
//	return _entityID;
//}

void HealthSystem::takeDamage(int _damageDoer, int _damageTaker, int _damage)
{
	//HealthManager.getComponent(_damageTaker).mCurrentHealth -= _damage;
}

//int HealthSystem::GetHealth(std::shared_ptr<Player> actorToGetHealthFrom)
//{
//	return actorToGetHealthFrom->hp->mCurrentHealth;
//}

//void HealthSystem::takeDamage(std::shared_ptr<Player> damageDoer, std::shared_ptr<Player> damageTaker, int damage)
//{
//
//}
