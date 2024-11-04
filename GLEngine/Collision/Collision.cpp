#include "pch.h"
#include "Collision.h"
#include "../Initializer/initializer.h"


std::vector<std::shared_ptr<Collision>> Collision::AllCollision;

Collision::Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset, ECollisionType collision_type) : scale(scale), offset(offset), collisionType(collision_type)
{
    min = position;
    max = position + scale;
    max.z = position.z - scale.z;

    AllCollision.push_back(std::make_shared<Collision>(*this));
}

void Collision::UpdatePosition(glm::vec3 position)
{
    min = position;
    max = position + scale;
    max.z = position.z - scale.z;
}

bool Collision::checkBallBoxCollision(Spheres ball, Cube wall, ECollisionType ballType, ECollisionType wallType)
{
    if (ballType == ECollisionType::ball && wallType == ECollisionType::Boxes)
    {
        if (((wall.position.x <= ball.position.x + ball.size.x) && 
             (wall.position.x >= ball.position.x - ball.size.x)) ||
            ((wall.position.z - wall.size.z <= ball.position.z + ball.size.z) && 
             (wall.position.z >= ball.position.z - ball.size.z)))
        {
          
             std::cout << "Collision detected" << "\n";
        	return true; 
        }
    }
    return false;
}

bool Collision::checkPlayerItemCollision(std::shared_ptr<Player> _player, std::shared_ptr<Item> _item)
{
	if (((_player->position.x <= _item->position.x + _item->size.x) &&
             (_player->position.x >= _item->position.x - _item->size.x)) ||
            ((_player->position.z - _player->size.z <= _item->position.z + _item->size.z) &&
             (_player->position.z >= _item->position.z - _item->size.z))){
		{
          
             //std::cout << "Collision detected" << "\n";
			
        	return true; 
        }
    }
	return false;
}

bool Collision::checkBoxBoxCollision(std::shared_ptr<Player> _player, std::shared_ptr<Enemy> _enemy)
{

		if (((_player->position.x <= _enemy->position.x + _enemy->size.x) &&
			(_player->position.x >= _enemy->position.x - _enemy->size.x)) &&
			((_player->position.z - _player->size.z <= _enemy->position.z + _enemy->size.z) &&
				(_player->position.z >= _enemy->position.z - _enemy->size.z)))
		{
			//std::cout << "Collision detected" << "\n";
			return true;
		}
    return false;
}




bool Collision::checkBallBallCollision(Spheres& ball1, Spheres& ball2, ECollisionType type)
{
	float totald = std::sqrtf(std::powf(ball2.position.x - ball1.position.x, 2.f) + std::powf(ball2.position.y - ball1.position.y, 2.f) + std::powf(ball2.position.z - ball1.position.z, 2.f));
	float radius = (ball1.GetScale().x + ball2.GetScale().x);
	if (type == ECollisionType::ball)
	{
		if(totald > radius)
            return false;
        ballphysics(ball1, ball2);
			std::cout << "Collision detected" << std::endl;
		std::cout << "Ball1 position: " << ball1.position.x << " " << ball1.position.y << " " << ball1.position.z << std::endl;
		std::cout << "Ball2 position: " << ball2.position.x << " " << ball2.position.y << " " << ball2.position.z << std::endl;

		glm::vec3 ClampedNormal = ball1.position - ball2.position;
		glm::vec3 ClampedNormal2 = ball2.position - ball1.position;
        float massv1 = (2 * ball2.m) / (ball1.m + ball2.m);
        float massv2 = (2 * ball1.m) / (ball1.m + ball2.m);
		float dotv1 = glm::dot(ball1.velocity - ball2.velocity, ClampedNormal);
		float magnitudev1 = sqrt(pow(ClampedNormal.x, 2) + pow(ClampedNormal.y, 2) + pow(ClampedNormal.z, 2));
		dotv1 /= magnitudev1;
		float dotv2 = glm::dot(ball2.velocity - ball1.velocity, -ClampedNormal);
		float magnitudev2 = sqrt(pow(-ClampedNormal.x, 2) + pow(-ClampedNormal.y, 2) + pow(-ClampedNormal.z, 2));
		dotv2 /= magnitudev2;
        glm::vec3 tempv1 = massv1 * dotv1 * ClampedNormal;
		glm::vec3 tempv2 = massv2 * dotv2 * -ClampedNormal;
		ball1.velocity = ball1.velocity - tempv1;
		ball2.velocity = ball2.velocity - tempv2;
		return true;
	}
	return false;
}

void Collision::ballphysics(Spheres& b1, Spheres& b2)
{
	float totalr = std::sqrtf(std::powf(b2.position.x - b1.position.x, 2.f) + std::powf(b2.position.y - b1.position.y, 2.f) + std::powf(b2.position.z - b1.position.z, 2.f));
	float d = (b1.GetScale().x + b2.GetScale().x);
	glm::vec3 impactNormal = glm::normalize(b2.position - b1.position);
    b1.position = b1.position + impactNormal * (totalr - d) / 2.f;
    b2.position = b2.position - impactNormal * (totalr - d) / 2.f;
}

void Collision::enemyAI(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _speed, float _deltaTime)
{
	glm::vec3 direction = _player->position - _enemy->position;
	direction = glm::normalize(direction);
	_enemy->position += direction * _speed * _deltaTime;
}

void Collision::bounceBack(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _bounceDistance)
{
	glm::vec3 direction = _player->position - _enemy->position;
	direction = glm::normalize(direction);
	_enemy->position -= direction * _bounceDistance;
}

