#include "pch.h"
#include "Actor.h"


void Player::createPlayer(glm::vec3 size, glm::vec3 pos, glm ::vec3 color)
{
	CreateCube(size, pos, color);
}

glm::vec3& Player::GetPlayerPos()
{
	 return mPosition;
}

 glm::vec3& Player::GetPlayerSize()
 {
	 return mSize;
 }

void Player::drawPlayer()
{
	Draw();	
}


Enemy::Enemy()
{
	
	
}

void Enemy::createEnemy(glm::vec3 _size, glm::vec3 _pos, glm::vec3 _color)
{
	CreateCube(_size, _pos, _color);
}

void Enemy::drawEnemy()
{
	Draw();
}

glm::vec3& Enemy::GetEnemyPos()
{
	return mPosition;
}

glm::vec3& Enemy::GetEnemySize()
{
	return mSize;
}


Item::Item()
{

}

void Item::createItem(glm::vec3 _size, float _segment, glm::vec3 _pos, float _mass, glm::vec3 _speed, glm::vec3 _color)
{
	CreateSphere(_size, _segment, _pos, _mass, _speed, _color);
}

void Item::drawItem()
{
	DrawSphere();
}

glm::vec3& Item::GetItemPos()
{
	return mPosition;
}

glm::vec3& Item::GetItemSize()
{
	return mSize;
}

