#pragma once
#include <unordered_map>

template <typename T>

class ComponentManager
{
private:
	std::unordered_map<int, T> components;

public:

void addComponent(int entityID, const T& component) 
{
	components[entityID] = component;
}


bool hasComponent(int entityID)
{
	return components.contains(entityID) != components.end();
}

void removeComponent(int entityID)
{
	components.erase(entityID);
	std::cout << "Component Removed" << std::endl;
}

T getComponent(int entityID)
{
	return components[entityID];
}

std::unordered_map<int, T> getComponents()
{
	return components;
}

};

