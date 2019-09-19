/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
	Jose Rosenbluth: CollisionMask, InitCollisionMatrix(), CheckCollisionMatrix()
-End Header--------------------------------------------------------*/

#include "PhysicsManager.h"
#include "PhysicsUtils.h"

PhysicsManager::PhysicsManager() : mTimeAccumulator(0.0f)
{
	InitCollisionMatrix();
}


PhysicsManager::~PhysicsManager() {}


void PhysicsManager::LateUpdate(float dt)
{
	// time stabilization
	mTimeAccumulator += dt;
	while (mTimeAccumulator >= PhysicsUtils::Consts::fixedTimeStep)
	{
		// broad phase
		// narrow phase
		// velocity update
		// collision resolution
		// position update
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////====COLLISION-MATRIX-FUNCTIONS========================================================================================/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PhysicsManager::InitCollisionMatrix()
{
	//Deserialize file containing collision matrix info
	std::ifstream stream;
	stream.open(PhysicsUtils::Consts::Collision_Matrix_Dir + "CollisionMatrix.txt");
	if (stream.is_open())
	{
		for (int i = 0; i < static_cast<int>(CollisionMask::NUM); ++i)
		{
			std::string name;
			if (stream >> name)
			{
				//Go through the row adding into the collision matrix
				for (int j = 0; j < static_cast<int>(CollisionMask::NUM); ++j)
				{
					bool hitCollision;
					if (stream >> hitCollision)
					{
						mCollisionMatrix[i][j] = hitCollision;
					}
					else
					{
						std::cout << "(CollisionManager::InitCollisionMatrix)- FAILED TO READ STREAM -2!" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "(CollisionManager::InitCollisionMatrix)- FAILED TO READ STREAM! -1" << std::endl;
			}
		}

		stream.close();
	}
	else
	{
		std::cout << "(CollisionManager::InitCollisionMatrix)- FAILED TO OPEN FILE!" << std::endl;
	}

	//CollisionMatrix[CollisionMask::WALL] = {false, true, false, true, true, true, false, false, true, true, true, true, false, false};

}

bool PhysicsManager::CheckCollisionMatrix(CollisionMask obj1, CollisionMask obj2)
{
	return mCollisionMatrix[static_cast<int>(obj1)][static_cast<int>(obj2)];
}

void PhysicsManager::PrintCollisionMatrix()
{
	for (int i = 0; i < static_cast<int>(CollisionMask::NUM); ++i)
	{
		for (int j = 0; j < static_cast<int>(CollisionMask::NUM); ++j)
		{
			std::cout << mCollisionMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

