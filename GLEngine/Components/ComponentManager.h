#pragma once
#include <unordered_map>

template <typename T>

class ComponentManager
{
private:
	std::unordered_map<int, T> components;

public:

	void addComponent(int _entityID, const T _component) 
	{
		components[_entityID] = _component;
		std::cout << "Component Added " << std::endl;
	}

	bool hasComponent(int _entityID)
	{
		return components.contains(_entityID);
	}

	void removeComponent(int _entityID)
	{
		components.erase(_entityID);
		std::cout << "Component Removed" << std::endl;
	}

	T& getComponent(int _entityID)
	{
		return components[_entityID];
	}

	std::unordered_map<int, T> getComponents()
	{
		return components;
	}

};

