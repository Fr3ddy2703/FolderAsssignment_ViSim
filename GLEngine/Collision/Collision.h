#pragma once
#define PrintBool(x) std::cout << ((x) ? "True" : "False") << "\n"


class Spheres;
class Cube;
class Player;
class Item;
class Enemy;

enum class ECollisionType
{
    Boxes,
    enemy,
    player,
    item,
    ball
};

class Collision
{
public:
    glm::vec3 mMin = glm::vec3(0.f);
    glm::vec3 mMax= glm::vec3(0.f);
    glm::vec3 mScale;
    glm::vec3 mOffset = glm::vec3(0.f);
    ECollisionType mCollisionType;

	Collision() = default;
    
    Collision(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _offset = glm::vec3(0.f),ECollisionType _collision_type = ECollisionType::Boxes);
    void UpdatePosition(glm::vec3 position);
    bool checkBallBoxCollision(Spheres _ball, Cube _wall, ECollisionType _ballType, ECollisionType _wallType);
	bool checkPlayerItemCollision(std::shared_ptr<Player> _player, std::shared_ptr<Item> _item);
	bool checkBoxBoxCollision(std::shared_ptr<Player> _player, std::shared_ptr<Enemy> _enemy);
	bool checkBallBallCollision(Spheres& _ball1, Spheres& ball2, ECollisionType _type);
    void ballphysics(Spheres& _b1, Spheres& _b2);
	void enemyAI( std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _speed, float _deltaTime);
	void bounceBack(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _bounceDistance);
private:
    static std::vector<std::shared_ptr<Collision>> mAllCollision;
};
