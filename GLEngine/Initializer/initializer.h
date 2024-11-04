#pragma once
#include "../Camera/camera.h"
#include "../Meshes/Sphere/Spheres.h"
#include "../Meshes/Cubes/Cubes.h"
#include "../Actor/Actor.h"
#include "../Meshes/BSplineSurface/BSplineSurface.h"
#include "../Meshes/PointCloud/PointCloud.h"

class ElementBuffer;
class VertexBuffer;
struct GLFWwindow;

class initializer
{
public:

	/* Public variables*/
	GLFWwindow *window;
	static camera UseCamera;
	static float DeltaTime;
	Cube Floor;
	Cube Wall;
	Cube Wall2;
	Cube Wall3;
	Cube Wall4;

	/* Degrees for the Bi-Quadratic */
	int Du = 2;
	int Dv = 2;

	int UResolution = 20;
	int VResolution = 20;

	std::vector<float> uKnot = {0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0};
	std::vector<float> vKnot = {0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0}; 

	std::vector<std::vector<glm::vec3>> controlPoints = {
		{glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(2.0, 0.0, 0.0)},
		{glm::vec3(0.0, 1.0, 1.0), glm::vec3(1.0, 2.0, 1.0), glm::vec3(2.0, 1.0, 1.0)},
		{glm::vec3(0.0, 0.0, 2.0), glm::vec3(1.0, 1.0, 2.0), glm::vec3(2.0, 0.0, 2.0)}
	};

	
	/* Public Functions */
	initializer(){};

	virtual void Initialize();
	virtual void Create();
	virtual void Run();
	virtual void Update(float deltaTime);

	virtual  ~initializer();

private:
	/* private Shared Pointers */
	std::vector<Cube> Cubes; 
	std::vector<Spheres> Balls;
	std::shared_ptr<BSplineSurface> bSplines;
	std::shared_ptr<PointCloud> pCloud;
	std::shared_ptr<Player> player;
	std::shared_ptr<Enemy> enemy;
	std::shared_ptr<Item> item;

	/* private variables */
	bool render = true;
	bool start = false;
};

