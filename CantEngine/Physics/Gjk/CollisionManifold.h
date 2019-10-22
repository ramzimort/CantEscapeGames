/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/
#pragma once

#include "Components/RigidbodyComponent.h"

class RigidbodyComponent;
class CollisionManifold
{
public:
	CollisionManifold();
	CollisionManifold(const CollisionManifold& rhs);
	CollisionManifold& operator=(const CollisionManifold& rhs);
	
	RigidbodyComponent* m_objectA;      // body containing vertex
	RigidbodyComponent* m_objectB;      // body containing face
	Vector3 m_pA;						// Collision point for object A
	Vector3 m_pB;						// Collision point for object B
	Vector3 m_normal;					// outwards pointing normal of face

	float m_depth;
	bool m_isColliding;
};
