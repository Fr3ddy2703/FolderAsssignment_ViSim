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
	GLFWwindow *mWindow;
	static camera mUseCamera;
	static float mDeltaTime;
	Cube mFloor;
	Cube mWall;
	Cube mWall2;
	Cube mWall3;
	Cube mWall4;
	Cube mSurface;
	Spheres mKule;

	/* Degrees for the Bi-Quadratic */
	int mDu = 2;
	int mDv = 2;

	int mUResolution = 20;
	int mVResolution = 20;

	std::vector<float> mUKnot = {0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0};
	std::vector<float> mVKnot = {0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0}; 

	std::vector<std::vector<glm::vec3>> mControlPoints = {
		{glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(2.0, 0.0, 0.0)},
		{glm::vec3(0.0, 1.0, 1.0), glm::vec3(1.0, 2.0, 1.0), glm::vec3(2.0, 1.0, 1.0)},
		{glm::vec3(0.0, 0.0, 2.0), glm::vec3(1.0, 1.0, 2.0), glm::vec3(2.0, 0.0, 2.0)}
	};

	
	/* Public Functions */
	initializer() = default;

	virtual void Initialize();
	virtual void Create();
	virtual void Run();
	virtual void Update(float _deltaTime);

	virtual ~initializer() = default;

private:
	/* private Shared Pointers */
	std::vector<Cube> mCubes; 
	std::vector<Spheres> mBalls;
	std::shared_ptr<BSplineSurface> mBSplines;
	std::shared_ptr<PointCloud> mPCloud;
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<Enemy> mEnemy;
	std::shared_ptr<Item> mItem;

	/* private variables */
	bool mRender = true;
	bool mStart = false;
};

