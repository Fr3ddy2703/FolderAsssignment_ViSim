#include "pch.h"
#include "initializer.h"
#include "../Input/input.h"
#include "../MathFunctions/MathFunctions.h"
#include "../Shaders/shader.h"

float initializer::mDeltaTime = 0.f;
camera initializer::mUseCamera = camera();


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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*Folder exam task 1.3 */
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

	/* The surface' variables */
	std::vector<Vertex> vertices;
	std::vector<Triangle> index;
	int resolution = 10;
	float distX = maxVertices.x - minVertices.x;
	float distZ = maxVertices.z - minVertices.z;
	float distcapX = distX / resolution;
	float distcapZ = distZ / resolution;

	for (int i = 0; i <= resolution; i++)
	{
		for (int j = 0; j <= resolution; j++)
		{
			/* For each coordinates in the grid */
			glm::vec3 gridPos(
				minVertices.x + distcapX * i, 
				0.0f,                         
				minVertices.z + distcapZ * j   
			);

			/* Variables for the height */
			float heightSum = 0.0f;
			int heightCount = 0;

			/* The threshold of how detailed the surface should be, lower value give more detail on the terrain */
			float thresholdDistance = 0.01f;

			for (const Vertex& vert : mPCloud->mVertices)
			{
				float distance = glm::distance(glm::vec2(gridPos.x, gridPos.z), glm::vec2(vert.mPosition.x, vert.mPosition.z));

				if (distance < thresholdDistance)
				{
					heightSum += vert.mPosition.y;
					heightCount++;
				}
			}

			// Calculate the average height based on nearby points
			if (heightCount > 0) {
				gridPos.y = heightSum / heightCount;
			}

			vertices.push_back(Vertex(gridPos, Color::Blue));
		}
	}

	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			/* For each corner in a square */
			int topLeft = i * (resolution + 1) + j;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + (resolution + 1);
			int bottomRight = bottomLeft + 1;

			// First triangle of the square
			index.push_back(Triangle(topLeft, bottomLeft, bottomRight));

			// Second triangle of the square
			index.push_back(Triangle(topLeft, bottomRight, topRight));
		}
	}

	/* Calculations for normals */
	for (const Triangle& triangle : index)
    {
		glm::vec3 normal = glm::cross(vertices[triangle.mIndex2].mPosition - vertices[triangle.mIndex1].mPosition,
                                      vertices[triangle.mIndex3].mPosition - vertices[triangle.mIndex1].mPosition);
        vertices[triangle.mIndex1].mNormal += glm::normalize(normal);
        vertices[triangle.mIndex2].mNormal += glm::normalize(normal);
        vertices[triangle.mIndex3].mNormal += glm::normalize(normal);
    }
	

	mSurface.CreateSurfaceFromPointCLoud(vertices, index, glm::vec3(20));

	Spheres Sphere;
	Sphere.CreateSphere(glm::vec3(1.f),4.f, glm::vec3(15.25f, 100.f, 2.5f), 1.f,glm::vec3(0.f, 0.f, 0.f),Color::Gold);
	Sphere.AddCollider(Sphere.GetScale(), ECollisionType::ball);

	mCubes.emplace_back(mFloor);
	mCubes.emplace_back(mWall);
	mCubes.emplace_back(mWall2);
	mCubes.emplace_back(mWall3);
	mCubes.emplace_back(mWall4);
	mBalls.push_back(mKule);
	mBalls.push_back(Sphere);

}

void initializer::Run()
{
	Create();
 	float FirstFrame = 0.0f;
	glm::vec3 color(Color::Grey);
	
	while (!glfwWindowShouldClose(mWindow))
	{
		glEnable(GL_DEPTH_TEST);
		const auto CurrentFrame = static_cast<float>(glfwGetTime());
		mDeltaTime = CurrentFrame - FirstFrame;
		FirstFrame = CurrentFrame;
		const float spawnCooldown = 0.5f;
		static float lastSpawnTime = 0.0f;

		if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			float currentTime = glfwGetTime();
			if (currentTime - lastSpawnTime >= spawnCooldown)
			{
				Spheres newBall;
				newBall.CreateSphere(glm::vec3(1.f), 4.f, mUseCamera.mCameraPos + mUseCamera.mCameraFront,1.f, glm::vec3(-1.f, 0.f, 0.f), Color::Gold);
				mBalls.push_back(newBall);
				lastSpawnTime = currentTime;
			}
		}


		Barycentric mBarycentric;
		for (auto& ball : mBalls)
		{
			float ballheight = 0.f;
			glm::vec3 faceNormal = glm::vec3(0);
			ball.mAcceleration = glm::vec3(0, -9.81, 0);
			if (mBarycentric.BarycentricCord(ball, mSurface, ballheight, faceNormal))
			{
				ball.mVelocity.y = 0;
				glm::vec3 faceAcceleration = 9.81f * glm::vec3(faceNormal.x * faceNormal.y, (faceNormal.y * faceNormal.y) - 1, faceNormal.z * faceNormal.y);
				ball.mAcceleration = faceAcceleration;
				ball.mPosition.y = ballheight;
			}
		}
		
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
		for (auto& sphere : mBalls)
		{
			sphere.DrawSphere();
		}
		//mPlayer->drawPlayer();
		//mEnemy->drawEnemy();
		//mItem->drawItem();
		/*mBSplines->Draw();*/
		//mPCloud->Draw();
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

	for (auto& ball : mBalls)
	{

		//if(collision.checkBallBallCollision(ball, ball, ECollisionType::ball))
		//{
		//	
		//}
		//ball.UpdatePos(_deltaTime);
		//ball.UpdatePos(_deltaTime);
		ball.UpdatePos(_deltaTime);
	}

	//player->takeDamage();
}
