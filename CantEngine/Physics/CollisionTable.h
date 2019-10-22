#pragma once

class CollisionTable
{
public:
	enum CollisionMask
	{
		#define COLLISION_MASK(ENTRY) ENTRY,
		#include "Physics/CollisionMaskTypes.def"
		#undef COLLISION_MASK
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