#include "pch.h"
#include "initializer.h"
#include "../Input/input.h"
#include "../MathFunctions/MathFunctions.h"
#include "../Meshes/ParticleSystem/ParticleSystem.h"
#include "../Shaders/shader.h"
#include "../Components/Entity/Entity.h"
#include "../Components/ComponentManager.h"
#include "../Components/ComponentHandler/ComponentHandler.h"
#include "../Components/Entity/EntityHandler/EntityHandler.h"
#include "../Components/PositionComponents/PositionComponent.h"
#include "../Components/VelocityComponents/VelocityComponent.h"
#include "../LuaIntegration/LuaIntegration.h"
#include "../Systems/MovementSystem/MovementSystem.h"

float initializer::mDeltaTime = 0.f;
camera initializer::mUseCamera = camera();
MovementSystem movementSystem(ComponentHandler::getInstance().positionManager, ComponentHandler::getInstance().VelocityManager);
std::vector<Entity>& entities = EntityHandler::getInstance().entities;

void initializer::Initialize()
{
	mWindow = window::initWindow();
	mUseCamera.initCamera();

	Run();
}


void initializer::Create()
{
	/* Comment out lua part and uncomment the commented part to test ECS*/
	LuaIntegration lua;
	lua.lua();
	//entities.emplace_back(0);
	//entities.emplace_back(1);
	//entities.emplace_back(2);
	ComponentManager<PositionComponent>& positionManager = ComponentHandler::getInstance().positionManager;
	ComponentManager<VelocityComponent>& velocityManager = ComponentHandler::getInstance().VelocityManager;
	//positionManager.addComponent(0, PositionComponent(glm::vec3(100.f)));
	//positionManager.addComponent(1, PositionComponent(glm::vec3(50.f)));
	//positionManager.addComponent(2, PositionComponent(glm::vec3(10.f)));
	//velocityManager.addComponent(0, VelocityComponent(glm::vec3(1.f)));
	//velocityManager.addComponent(1, VelocityComponent(glm::vec3(2.f)));
	//velocityManager.addComponent(2, VelocityComponent(glm::vec3(15.f)));

	/* Scene */

	/* Entity 0 */
	mFloor.CreateCube(0,glm::vec3(18.f, 0.5f, 9.f), positionManager.getComponent(0).mPosition, Color::Green);
	/* Entity 1 */
	mWall.CreateCube(1,glm::vec3(18.f, 0.5f, 9.f), positionManager.getComponent(1).mPosition, Color::Red);
	/* Entity 2 */
	mWall2.CreateCube(2,glm::vec3(18.f, 0.5f, 9.f), positionManager.getComponent(2).mPosition, Color::Blue);


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
	float friction = 0.f;
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
				float distance = glm::distance(glm::vec2(gridPos.x,  gridPos.z), glm::vec2(vert.mPosition.x, vert.mPosition.z));

				if (distance < thresholdDistance)
				{
					heightSum -= vert.mPosition.y;
					heightCount++;
				}
			}

			// Calculate the average height based on nearby points
			if (heightCount > 0) {
				gridPos.y = heightSum / heightCount;
			}

			glm::vec3 color = glm::vec3(0, 0, 1);
			if (gridPos.y >= 0 && gridPos.y <= 10)
			{
				color = glm::vec3(1, 0,0);
				friction = 1.f;
			} else
			{
				friction = 0.f;
			}

			vertices.push_back(Vertex(gridPos, color, friction));
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

			/* First triangle of the square */
			index.push_back(Triangle(topLeft, bottomLeft, bottomRight));

			/* Second triangle of the square */
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
	

	mSurface.CreateSurfaceFromPointCLoud(vertices, index, glm::vec3(40));


	mCubes.emplace_back(mFloor);
	mCubes.emplace_back(mWall);
	mCubes.emplace_back(mWall2);
	mCubes.emplace_back(mWall3);
	mCubes.emplace_back(mWall4);

}

void initializer::Run()
{
	Create();
 	float FirstFrame = 0.0f;
	glm::vec3 color(Color::Grey);
	ParticleSystem particleSystem = ParticleSystem(100000);
	particleSystem.BindBuffer();
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(mWindow))
	{
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
				newBall.CreateSphere(glm::vec3(1.f), 4.f, 
				mUseCamera.mCameraPos + mUseCamera.mCameraFront,1.f, glm::vec3(-1.f, 0.f, 0.f), Color::Gold);
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
			float friction;
			if (mBarycentric.BarycentricCord(ball, mSurface, ballheight, faceNormal, friction))
			{
				ball.mVelocity.y = 0;
				glm::vec3 faceAcceleration = 9.81f * glm::vec3(faceNormal.x * faceNormal.y, (faceNormal.y * faceNormal.y) - 1, faceNormal.z * faceNormal.y);
				ball.mAcceleration = faceAcceleration;
				ball.mPosition.y = ballheight;

				glm::vec3 frictionForce = -friction * ball.mVelocity;
				ball.mAcceleration += frictionForce;
			}
		}



		glClearColor(color.x, color.y, color.z, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(Shader::ShaderProgram);

		glm::vec3 ParticlePos = glm::vec3(0, 10, 0);
		float lifeSpan = 5.f;
		particleSystem.emit(ParticlePos, lifeSpan);
		particleSystem.update(mDeltaTime);

		KeyBoardInput::processInput(mWindow, mPlayer);
		Update(mDeltaTime);

	

		glUniformMatrix4fv(mUseCamera.mProjectionLoc, 1, GL_FALSE, glm::value_ptr(mUseCamera.getProjection(window::mWidth, window::mHeight)));
		glUniformMatrix4fv(mUseCamera.mViewLoc, 1, GL_FALSE, glm::value_ptr(mUseCamera.getView()));
		glUniform3fv(glGetUniformLocation(Shader::ShaderProgram, "viewPos"), 1, glm::value_ptr(mUseCamera.mCameraPos));
		int counter = 0;
		for (auto& cube : mCubes)
		{
			cube.Draw(ComponentHandler::getInstance().positionManager, counter++);
		}
		for (auto& sphere : mBalls)
		{
			sphere.DrawSphere();
		}
		//mPlayer->drawPlayer();
		//mEnemy->drawEnemy();
		//mItem->drawItem();

	/*	mBSplines->Draw();*/
		/*mPCloud->Draw();*/
		mSurface.DrawSurface();
		particleSystem.draw();

	
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
	glfwTerminate();
}

void initializer::Update(float _deltaTime)
{
	Collision collision;
	//if(collision.checkBoxBoxCollision(mPlayer, mEnemy))
	//{
	//	collision.bounceBack(mEnemy, mPlayer, 1.f);
	//}
	//collision.enemyAI(mEnemy, mPlayer, 1, _deltaTime);

	movementSystem.Update(entities, _deltaTime);

	/* Collision for all balls */
	for (int i = 0; i < mBalls.size(); ++i)
	{
		for (int j = i + 1; j < mBalls.size(); ++j)
		{
			collision.checkBallBallCollision(mBalls[i], mBalls[j]);
		}
		mBalls[i].UpdatePos(_deltaTime);
	}
}
