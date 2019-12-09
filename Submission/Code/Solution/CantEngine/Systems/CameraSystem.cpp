
#include "CameraSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Graphics/AppRenderer.h"

unsigned const CameraSystem::static_type = BaseSystem::numberOfTypes++;

CameraSystem::CameraSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<CameraComponent>();
}


CameraSystem::~CameraSystem()
{
}


void CameraSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	CameraComponent* cameraComp = go->GetComponent<CameraComponent>();

	BaseSystemCompNode *component_node = new CameraCompNode(transform, cameraComp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void CameraSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	CameraCompNode *node = static_cast<CameraCompNode*>(compNode);

	TransformComponent *transformComp = node->m_pTransformComponent;
	CameraComponent* cameraComp = node->m_pCameraComponent;
	Camera& camera = cameraComp->GetCamera();

	//UPDATE CODE
	if (!cameraComp->m_isTransformIndependent) 
	{
		camera.SetCameraPosition(transformComp->GetPosition());
		camera.SetCameraRotation(transformComp->GetRotationMatrix());
	}

	camera.UpdateViewMatrix();
	camera.UpdateProjectionMatrix();
	camera.UpdateViewProjectionMatrix();
}