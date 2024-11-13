#include "pch.h"
#include "initializer.h"
#include "../Input/input.h"
#include "../MathFunctions/MathFunctions.h"
#include "../Shaders/shader.h"

float initializer::mDeltaTime = 0.f;
camera initializer::mUseCamera = camera();

/* Systems */


void initializer::Initialize()
{
	mWindow = window::initWindow();
	mUseCamera.initCamera();

	Run();
}

void initializer::Create()
{

	/* Scene */
	mFloor.CreateCube(glm::vec3(18.f, 0.5f, 9.f), glm::vec3(1.f, 0.f, 9.f),Color::Green);

	/* Left wall */
	mWall.CreateCube(glm::vec3(20.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f),Color::Brown);
	mWall.AddCollider(mWall.GetScale(), ECollisionType::Boxes);

	/* Right wall */
	mWall2.CreateCube(glm::vec3(20.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 10.f),Color::Brown);
	mWall2.AddCollider(mWall2.GetScale(), ECollisionType::Boxes);

	/* Front wall */
	mWall3.CreateCube(glm::vec3(-1.f, 1.f, -9.f), glm::vec3(1.f, 0.f, 0.f),Color::Brown);
	mWall3.AddCollider(mWall3.GetScale(), ECollisionType::Boxes);

	/* Back wall */
	mWall4.CreateCube(glm::vec3(1.f, 1.f, -9.f), glm::vec3(19.f, 0.f, 0.f),Color::Brown);
	mWall4.AddCollider(mWall4.GetScale(), ECollisionType::Boxes);

	/* Player */
	mPlayer = std::make_shared<Player>();
	mPlayer->createPlayer(glm::vec3(0.5f), glm::vec3(10.f, 0.75f, 5.f), Color::Blue);

	/* Enemy */
	mEnemy = std::make_shared<Enemy>();
	mEnemy->createEnemy(glm::vec3(0.5f), glm::vec3(10.f, 0.75f, 10.f), Color::Red);

	/* Item */
	mItem = std::make_shared<Item>();
	mItem->createItem(glm::vec3(0.25f), 4.f, glm::vec3(15.f, 0.75f, 5.f), 1.f, glm::vec3(0.f, 0.f, 0.f), Color::Gold);

	mBSplines = std::make_shared<BSplineSurface>();
	mBSplines->CreateBSplineSurface(mUResolution, mVResolution, mDu, mDv, mUKnot, mVKnot, mControlPoints);

	mPCloud = std::make_shared<PointCloud>();
	std::string directory = "ReadableFiles/Budor/32-2-519-158-01.txt";
	mPCloud->CreatePointCloudFromFile(directory.c_str(), 0.01f);
	mPCloud->mPosition;
	mPCloud->mIndices;
	glm::vec3 minVertices = glm::vec3(0);
	glm::vec3 maxVertices = glm::vec3(0);
	for (Vertex& vertex : mPCloud->mVertices)
	{
		if (vertex.mPosition.x < minVertices.x)
		{
			minVertices.x = vertex.mPosition.x;
		}
		if (vertex.mPosition.y < minVertices.y)
		{
			minVertices.y = vertex.mPosition.y;
		}
		if (vertex.mPosition.z < minVertices.z )
		{
			minVertices.z = vertex.mPosition.z;
		}
		if (vertex.mPosition.x > maxVertices.x)
		{
			maxVertices.x = vertex.mPosition.x;
		}
		if (vertex.mPosition.y > maxVertices.y)
		{
			maxVertices.y = vertex.mPosition.y;
		}
		if (vertex.mPosition.z > maxVertices.z )
		{
			maxVertices.z = vertex.mPosition.z;
		}
	}

	std::vector<Vertex> vertices;
	std::vector<Triangle> index;
	int resolution = 10;
	float distX = maxVertices.x - minVertices.x;
	float distZ = maxVertices.z - minVertices.z;
	float distcapX = distX / resolution;
	float distcapZ = distZ / resolution;
	int numpoints = 0;
	float avgposY = 0;
	
	for (int i = 0; i <= resolution; i++)
	{
		for (int j = 0; j <= resolution; j++)
		{
			for (Vertex& vert : mPCloud->mVertices)
			{
				avgposY += vert.mPosition.y;
				avgposY /= numpoints;
			}
			vertices.push_back(Vertex(glm::vec3(minVertices.x + (distcapX * i), 0, minVertices.z + (distcapZ * j)), glm::vec3(0, i /resolution , j / resolution)));
			numpoints++;
		}
	}

	for (int i = 0; i < resolution; i++)
{
    for (int j = 0; j < resolution; j++)
    {
		/* For  */
        int topLeft = i * (resolution + 1) + j;
        int topRight = topLeft + 1;
        int bottomLeft = topLeft + (resolution + 1);
        int bottomRight = bottomLeft + 1;

        // First triangle of the cell
        index.push_back(Triangle(topLeft, bottomLeft, bottomRight));

        // Second triangle of the cell
        index.push_back(Triangle(topLeft, bottomRight, topRight));
    }
}


	mSurface.CreateSurfaceFromPointCLoud(vertices, index);

	Spheres Sphere;
	Sphere.CreateSphere(glm::vec3(0.25f),4.f, glm::vec3(15.25f, 0.75f, 2.5f), 1.f,glm::vec3(0.f, 0.f, 0.f),Color::Gold);
	Sphere.AddCollider(Sphere.GetScale(), ECollisionType::ball);

	/* Creating the balls */
	Spheres kule;
	kule.CreateSphere(glm::vec3(0.25f),4.f, glm::vec3(15.f, 0.75f, 7.5f), 1.f,glm::vec3(0.f, 0.f, 0.f),Color::Gold);
	kule.AddCollider(kule.GetScale(), ECollisionType::ball);
	
	mCubes.emplace_back(mFloor);
	mCubes.emplace_back(mWall);
	mCubes.emplace_back(mWall2);
	mCubes.emplace_back(mWall3);
	mCubes.emplace_back(mWall4);
	mBalls.push_back(kule);
	mBalls.push_back(Sphere);

}

void initializer::Run()
{
	Create();
 	float FirstFrame = 0.0f;
	glm::vec3 color(Color::Grey);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(mWindow))
	{
		const auto CurrentFrame = static_cast<float>(glfwGetTime());
		mDeltaTime = CurrentFrame - FirstFrame;
		FirstFrame = CurrentFrame;

		glClearColor(color.x, color.y, color.z, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(Shader::ShaderProgram);

		KeyBoardInput::processInput(mWindow, mPlayer);
		Update(mDeltaTime);


		glUniformMatrix4fv(mUseCamera.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mUseCamera.getProjection(window::mWidth, window::mHeight)));
		glUniformMatrix4fv(mUseCamera.mViewLoc, 1, GL_FALSE, glm::value_ptr(mUseCamera.getView()));
		glUniform3fv(glGetUniformLocation(Shader::ShaderProgram, "viewPos"), 1, glm::value_ptr(mUseCamera.mCameraPos));
		//for (auto& cube : mCubes)
		//{
		//	cube.Draw();
		//}
		//for (auto& sphere : mBalls)
		//{
		//	sphere.DrawSphere();
		//}
		//mPlayer->drawPlayer();
		//mEnemy->drawEnemy();
		//mItem->drawItem();
		/*mBSplines->Draw();*/
		mPCloud->Draw();
		mSurface.Draw();
	
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void initializer::Update(float _deltaTime)
{
	Collision collision;
	if(collision.checkBoxBoxCollision(mPlayer, mEnemy))
	{
		collision.bounceBack(mEnemy, mPlayer, 1.f);
	}

	collision.enemyAI(mEnemy, mPlayer, 1, _deltaTime);

	//player->takeDamage();
}

initializer::~initializer()
{
}
