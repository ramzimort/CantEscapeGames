/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "CollisionManifold.h"
CollisionManifold::CollisionManifold() : m_objectA(nullptr), m_objectB(nullptr), m_depth(0.0f), m_isColliding(false)
{
}

CollisionManifold::CollisionManifold(const CollisionManifold& rhs) : m_objectA(rhs.m_objectA),
m_objectB(rhs.m_objectB),
m_pA(rhs.m_pA),
m_pB(rhs.m_pB),
m_normal(rhs.m_normal),
m_depth(rhs.m_depth),
m_isColliding(rhs.m_isColliding)
{
}

CollisionManifold& CollisionManifold::operator=(const CollisionManifold& rhs)
{
	this->m_objectA = rhs.m_objectA;
	this->m_objectB = rhs.m_objectB;
	this->m_pA = rhs.m_pA;
	this->m_pB = rhs.m_pB;
	this->m_normal = rhs.m_normal;
	this->m_depth = rhs.m_depth;
	this->m_isColliding = rhs.m_isColliding;

	return *this;
}
