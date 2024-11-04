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
    glm::vec3 min = glm::vec3(0.f);
    glm::vec3 max= glm::vec3(0.f);
    glm::vec3 scale;
    glm::vec3 offset = glm::vec3(0.f);
    ECollisionType collisionType;

	Collision() = default;
    
    Collision(glm::vec3 position, glm::vec3 scale, glm::vec3 offset = glm::vec3(0.f),ECollisionType collision_type = ECollisionType::Boxes);
    void UpdatePosition(glm::vec3 position);
    bool checkBallBoxCollision(Spheres ball, Cube wall, ECollisionType ballType, ECollisionType wallType);
	bool checkPlayerItemCollision(std::shared_ptr<Player> _player, std::shared_ptr<Item> _item);
	bool checkBoxBoxCollision(std::shared_ptr<Player> _player, std::shared_ptr<Enemy> _enemy);
	bool checkBallBallCollision(Spheres& ball1, Spheres& ball2, ECollisionType type);
    void ballphysics(Spheres& b1, Spheres& b2);
	void enemyAI( std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _speed, float _deltaTime);
	void bounceBack(std::shared_ptr<Enemy> _enemy, std::shared_ptr<Player> _player, float _bounceDistance);
private:
    static std::vector<std::shared_ptr<Collision>> AllCollision;
};
