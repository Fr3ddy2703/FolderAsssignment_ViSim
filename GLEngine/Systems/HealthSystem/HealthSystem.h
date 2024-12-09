#pragma once
#include "../Systems.h"

class HealthComponent;


class HealthSystem : Systems
{

public:
	//HealthSystem(ComponentManager<HealthComponent> HealthManager);
	int GetHealth(int _entityID);

	void takeDamage(int _damageDoer, int _damageTaker, int _damage);
};

