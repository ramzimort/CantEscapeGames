#include "stdafx.h"
#include "CollisionTable.h"
#include "PhysicsUtils.h"

CollisionTable::CollisionTable()
{
	InitCollisionMatrix();
}

CollisionTable::~CollisionTable()
{
}

void CollisionTable::InitCollisionMatrix()
{//Deserialize file containing collision matrix info
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
						m_CollisionMatrix[i][j] = hitCollision;
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

bool CollisionTable::CheckCollisionMatrix(CollisionMask obj1, CollisionMask obj2) const
{
	return m_CollisionMatrix[static_cast<int>(obj1)][static_cast<int>(obj2)];
}

#ifdef DEVELOPER
void CollisionTable::PrintCollisionMatrix() const
{
	for (int i = 0; i < static_cast<int>(CollisionMask::NUM); ++i)
	{
		for (int j = 0; j < static_cast<int>(CollisionMask::NUM); ++j)
		{
			std::cout << m_CollisionMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
#endif