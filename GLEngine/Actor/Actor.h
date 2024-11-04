#pragma once
#include "../Meshes/Cubes/Cubes.h"
#include "../Meshes/Sphere/Spheres.h"


class EntityComponent;

class Player : public Cube
{
public:

	/* Public Functions */
	Player(){};

	void createPlayer(glm::vec3 _size, glm::vec3 _pos, glm::vec3 _color);

	glm::vec3& GetPlayerPos();
	glm::vec3& GetPlayerSize();

	void drawPlayer();
};


class Enemy : public Cube
{

public:

	/* Public Functions */
	Enemy();
	void createEnemy(glm::vec3 _size, glm::vec3 _pos, glm::vec3 _color);
	void drawEnemy();

	glm::vec3& GetEnemyPos();
	glm::vec3& GetEnemySize();


};


class Item : public Spheres
{

public:

	/* Public Functions */
	Item();
	void createItem(glm::vec3 _size, float _segment, glm::vec3 _pos, float _mass, glm::vec3 _speed, glm::vec3 _color);
	void drawItem();

	glm::vec3& GetItemPos();
	glm::vec3& GetItemSize();
};

