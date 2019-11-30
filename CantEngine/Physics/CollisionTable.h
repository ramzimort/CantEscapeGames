/**
 * @file CollisionTable.h
 * @author Jose Rosenbluth, Aleksey Perfilev
 * @date 12/4/2019
 * @brief Stores a table of which collision masks collide with which
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
 #pragma once

/**
 * @brief Stores a table of which collision masks collide with which
 * 
 */
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
	
	/**
	 * @brief Reads CollisionMatrix.txt to construct the table and constructs it
	 * 
	 */
	void InitCollisionMatrix();
	/**
	 * @brief Checks if the given collision masks should collide
	 * 
	 * @param obj1 
	 * @param obj2 
	 * @return true 
	 * @return false 
	 */
	bool CheckCollisionMatrix(CollisionMask obj1, CollisionMask obj2) const;

#ifdef DEVELOPER
	void PrintCollisionMatrix() const;
#endif

private:
	// COLLISION MATRIX
	bool m_CollisionMatrix[static_cast<int>(CollisionMask::NUM)][static_cast<int>(CollisionMask::NUM)];

};