#pragma once
#include "../Components.h"

class HealthComponent : public Components
{
public:
	/* Public variables */
	int mCurrentHealth;
	int mMaxHealth;

	HealthComponent(int _health, int _maxHealth) : mCurrentHealth(_health), mMaxHealth(_maxHealth) {};

};
