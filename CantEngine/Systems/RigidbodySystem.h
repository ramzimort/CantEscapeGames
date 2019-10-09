/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

///Includes
#include "BaseSystem.h"
#include "Physics/SpatialPartition/DynamicAabbTree.h"
#include "Physics/CollisionTable.h"
#include "../Components/MeshComponent.h"
#include "Physics/Gjk/CollisionManifold.h"

class TransformComponent;
class RigidbodyComponent;
class MeshComponent;
class AppRenderer;


///TEST SYSTEM, WILL REQUIRE A TRANSFORM AND RENDERER COMP

struct RigidbodyCompNode : BaseSystemCompNode
{
	TransformComponent* m_transform;
	RigidbodyComponent* m_rigidbody;
	MeshComponent* m_mesh;

	//Ctor
	RigidbodyCompNode(TransformComponent* transform, RigidbodyComponent* rigidbody, MeshComponent* mesh)
	: m_transform(transform), m_rigidbody(rigidbody), m_mesh(mesh)
	{}
};


class RigidbodySystem : public BaseSystem
{
//Public interface
public:
	RigidbodySystem();
	virtual ~RigidbodySystem() {}

	virtual void Register_GameObject(GameObject *go) override;
	void RegisterAppRenderer(AppRenderer* renderer);

	virtual void LateUpdate(float dt) override;

	//void UpdateBoundingBox(const TransformComponent& transform, const Model);
protected:
	//protected vars

public:
	//To compare when using templates
	static unsigned int const static_type;

	DynamicAabbTree& GetAabbTree();
	
private:
	// this is used to make fixed update time
	float  m_timeAccumulator;

	DynamicAabbTree m_broadPhase;
		
	CollisionTable m_collisionTable;
	AppRenderer* m_pAppRenderer;

	Vector4 debugColor = { 0, 1, 0, 1 };
};