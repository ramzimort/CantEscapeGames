#pragma once

class CollisionTable
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

	CollisionTable();
	~CollisionTable();
	
	void InitCollisionMatrix();
	bool CheckCollisionMatrix(CollisionMask obj1, CollisionMask obj2) const;

#ifdef DEVELOPER
	void PrintCollisionMatrix() const;
#endif

private:
	// COLLISION MATRIX
	bool m_CollisionMatrix[static_cast<int>(CollisionMask::NUM)][static_cast<int>(CollisionMask::NUM)];

};