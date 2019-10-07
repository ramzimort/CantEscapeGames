
#include "FPSCameraSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/TestComponents/FPSControllerComponent.h"
#include "Managers/InputManager.h"
#include "Graphics/Camera.h"

unsigned const FPSCameraSystem::static_type = BaseSystem::numberOfTypes++;
static const float movement_rate = 20.f;

FPSCameraSystem::FPSCameraSystem()
{
	Push_required_comp<FPSControllerComponent>();
	Push_required_comp<TransformComponent>();
	Push_required_comp<CameraComponent>();
}


FPSCameraSystem::~FPSCameraSystem()
{
}


void FPSCameraSystem::Register_GameObject(GameObject *go)
{
	FPSControllerComponent *controller = go->GetComponent<FPSControllerComponent>();
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	CameraComponent* camera_comp = go->GetComponent<CameraComponent>();

	BaseSystemCompNode *component_node = new FPSCameraCompNode(controller, transform, camera_comp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void FPSCameraSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	using namespace DirectX;
	auto sys_comp = static_cast<FPSCameraCompNode*>(compNode);

	auto controller = sys_comp->m_controller;
	auto transform_comp = sys_comp->m_transform;
	auto camera_comp = sys_comp->m_camera_comp;


	Vector2 movement_amount(0.f, 0.f);
	if (controller->m_forward)
	{
		movement_amount.y = 1.f;
	}

	if (controller->m_backward)
	{
		movement_amount.y = -1.f;
	}

	if (controller->m_left)
	{
		movement_amount.x = -1.f;
	}

	if (controller->m_right)
	{
		movement_amount.x = 1.f;
	}
	XMVECTOR right = XMLoadFloat3(&camera_comp->GetCamera().m_rightDir);

	if (controller->m_leftClick)
	{

		Vector2 mouse_delta = controller->m_mouseDeltaPosition;
		XMFLOAT2 rot_amount;
		rot_amount.x = -mouse_delta.x * 100.f;
		rot_amount.y = -mouse_delta.y * 100.f;
		static const float rotation_rate = XMConvertToRadians(1.f);
		XMVECTOR rotation_vector = XMLoadFloat2(&rot_amount) * rotation_rate * dt;
		XMMATRIX pitch_mat = XMMatrixRotationAxis(right, XMVectorGetY(rotation_vector));
		XMMATRIX yaw_mat = XMMatrixRotationY(XMVectorGetX(rotation_vector));
		camera_comp->GetCamera().ApplyRotation(XMMatrixMultiply(pitch_mat, yaw_mat));
	}

	Vector3 position = transform_comp->GetPosition();
	XMVECTOR movement = XMLoadFloat2(&movement_amount) * movement_rate * dt;
	XMVECTOR strafe = right * XMVectorGetX(movement);
	XMVECTOR forward = XMLoadFloat3(&camera_comp->GetCamera().m_lookDir) * XMVectorGetY(movement);
	position += strafe;
	position += forward;
	XMFLOAT3 position3;
	XMStoreFloat3(&position3, position);

	transform_comp->SetLocalPosition(position3.x, position3.y, position3.z);
}