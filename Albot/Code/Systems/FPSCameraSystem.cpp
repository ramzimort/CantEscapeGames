#include "FPSCameraSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"

FPSCameraSystem::FPSCameraSystem()
{
}


FPSCameraSystem::~FPSCameraSystem()
{
}


void FPSCameraSystem::Register_GameObject(GameObject *go)
{
	Transform *transform = go->GetComponent<Transform>();
	CameraComponent* camera_comp = go->GetComponent<CameraComponent>();

	BaseSystemCompNode *component_node = new FPSCameraCompNode(transform, camera_comp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void FPSCameraSystem::Update(float dt, BaseSystemCompNode *compNode)
{
}

void FPSCameraSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
}