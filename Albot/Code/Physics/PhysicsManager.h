/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
	Jose Rosenbluth: CollisionMask, InitCollisionMatrix(), CheckCollisionMatrix()
-End Header--------------------------------------------------------*/

#pragma once

#include "SpatialPartition/DynamicAabbTree.h"

class PhysicsManager
{
public:
	enum class CollisionMask
	{
		WALL = 0,
		DINAMIC_OBJ,
		STATIC_OBJ,
		PLAYER,
		ENEMY,
		WEAPONS,
		UI,
		LOW_OBSTACLE,
		ENEMY_PROJ,
		NEUTRAL_PROJ,
		PLAYER_PROJ,
		MELEE_ENEMY,
		LOW_PROJ_WALL,
		ENEMY_BLOCK_WALL,
		NUM
	};

	PhysicsManager();
	~PhysicsManager();

	void LateUpdate(float deltaTime);

	//Collision matrix
	void InitCollisionMatrix();
	bool CheckCollisionMatrix(CollisionMask obj1, CollisionMask obj2);

	void PrintCollisionMatrix();
private:
	// this is used to make fixed frame rate
	float  mTimeAccumulator;

	DynamicAabbTree mBroadPhase;
	
	// COLLISION MATRIX
	bool mCollisionMatrix[static_cast<int>(CollisionMask::NUM)][static_cast<int>(CollisionMask::NUM)];
};// 