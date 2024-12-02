#include "pch.h"
#include "Collision.h"
#include "../Initializer/initializer.h"


std::vector<std::shared_ptr<Collision>> Collision::mAllCollision;

Collision::Collision(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _offset, ECollisionType _collision_type) : mScale(_scale), mOffset(_offset), mCollisionType(_collision_type)
{
    mMin = _position;
    mMax = _position + _scale;
    mMax.z = _position.z - _scale.z;

    mAllCollision.push_back(std::make_shared<Collision>(*this));
}

void Collision::UpdatePosition(glm::vec3 _position)
{
    mMin = _position;
    mMax = _position + mScale;
    mMax.z = _position.z - mScale.z;
}

bool Collision::checkBallBoxCollision(Spheres _ball, Cube _wall, ECollisionType _ballType, ECollisionType _wallType)
{
    if (_ballType == ECollisionType::ball && _wallType == ECollisionType::Boxes)
    {
        if (((_wall.mPosition.x <= _ball.mPosition.x + _ball.mSize.x) && 
             (_wall.mPosition.x >= _ball.mPosition.x - _ball.mSize.x)) ||
            ((_wall.mPosition.z - _wall.mSize.z <= _ball.mPosition.z + _ball.mSize.z) && 
             (_wall.mPosition.z >= _ball.mPosition.z - _ball.mSize.z)))
        {
          
             std::cout << "Collision detected" << "\n";
        	return true; 
        }
    }
    return false;
}

bool Collision::checkPlayerItemCollision(std::shared_ptr<Player> _player, std::shared_ptr<Item> _item)
{
	if (((_player->mPosition.x <= _item->mPosition.x + _item->mSize.x) &&
             (_player->mPosition.x >= _item->mPosition.x - _item->mSize.x)) ||
            ((_player->mPosition.z - _player->mSize.z <= _item->mPosition.z + _item->mSize.z) &&
             (_player->mPosition.z >= _item->mPosition.z - _item->mSize.z))){
		{
          
             //std::cout << "Collision detected" << "\n";
			
        	return true; 
        }
    }
	return false;
}

bool Collision::checkBoxBoxCollision(std::shared_ptr<Player> _player, std::shared_ptr<Enemy> _enemy)
{

		if (((_player->mPosition.x <= _enemy->mPosition.x + _enemy->mSize.x) &&
			(_player->mPosition.x >= _enemy->mPosition.x - _enemy->mSize.x)) &&
			((_player->mPosition.z - _player->mSize.z <= _enemy->mPosition.z + _enemy->mSize.z) &&
				(_player->mPosition.z >= _enemy->mPosition.z - _enemy->mSize.z)))
		{
			//std::cout << "Collision detected" << "\n";
			return true;
		}
    return false;
}




bool Collision::checkBallBallCollision(Spheres& _ball1, Spheres& _ball2)
{
	float totald = std::sqrtf(std::powf(_ball2.mPosition.x - _ball1.mPosition.x, 2.f) + std::powf(_ball2.mPosition.y - _ball1.mPosition.y, 2.f) + std::powf(_ball2.mPosition.z - _ball1.mPosition.z, 2.f));
	float radius = (_ball1.GetScale().x + _ball2.GetScale().x);
	if(totald > radius)
            return false;
        ballphysics(_ball1, _ball2);
		//	std::cout << "Collision detected" << std::endl;
		//std::cout << "Ball1 position: " << _ball1.mPosition.x << " " << _ball1.mPosition.y << " " << _ball1.mPosition.z << std::endl;
		//std::cout << "Ball2 position: " << _ball2.mPosition.x << " " << _ball2.mPosition.y << " " << _ball2.mPosition.z << std::endl;

		glm::vec3 ClampedNormal = _ball1.mPosition - _ball2.mPosition;
        float massv1 = (2 * _ball2.mMass) / (_ball1.mMass + _ball2.mMass);
        float massv2 = (2 * _ball1.mMass) / (_ball1.mMass + _ball2.mMass);
		float dotv1 = glm::dot(_ball1.mVelocity - _ball2.mVelocity, ClampedNormal);
		float magnitudev1 = sqrt(pow(ClampedNormal.x, 2) + pow(ClampedNormal.y, 2) + pow(ClampedNormal.z, 2));
		dotv1 /= magnitudev1;
		float dotv2 = glm::dot(_ball2.mVelocity - _ball1.mVelocity, -ClampedNormal);
		float magnitudev2 = sqrt(pow(-ClampedNormal.x, 2) + pow(-ClampedNormal.y, 2) + pow(-ClampedNormal.z, 2));
		dotv2 /= magnitudev2;
        glm::vec3 tempv1 = massv1 * dotv1 * ClampedNormal;
		glm::vec3 tempv2 = massv2 * dotv2 * -ClampedNormal;
		_ball1.mVelocity = _ball1.mVelocity - tempv1;
		_ball2.mVelocity = _ball2.mVelocity - tempv2;
		return true;
	}

void Collision::ballphysics(Spheres& _b1, Spheres& _b2)
{
	float totalr = std::sqrtf(std::powf(_b2.mPosition.x - _b1.mPosition.x, 2.f) + std::powf(_b2.mPosition.y - _b1.mPosition.y, 2.f) + std::powf(_b2.mPosition.z - _b1.mPosition.z, 2.f));
	float d = (_b1.GetScale().x + _b2.GetScale().x);
	glm::vec3 impactNormal = glm::normalize(_b2.mPosition - _b1.mPosition);
    _b1.mPosition = _b1.mPosition + impactNormal * (totalr - d) / 2.f;
    _b2.mPosition = _b2.mPosition - impactNormal * (totalr - d) / 2.f;
}

void Collision::enemyAI(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _speed, float _deltaTime)
{
	glm::vec3 direction = _player->mPosition - _enemy->mPosition;
	direction = glm::normalize(direction);
	_enemy->mPosition += direction * _speed * _deltaTime;
}

void Collision::bounceBack(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _bounceDistance)
{
	glm::vec3 direction = _player->mPosition - _enemy->mPosition;
	direction = glm::normalize(direction);
	_enemy->mPosition -= direction * _bounceDistance;
}

