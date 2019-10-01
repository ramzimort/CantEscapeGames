
#include "FPSCameraSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Managers/InputManager.h"
#include "Graphics/Camera.h"

unsigned const FPSCameraSystem::static_type = BaseSystem::numberOfTypes++;

extern InputManager* gpInputManager;

FPSCameraSystem::FPSCameraSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<CameraComponent>();
}


FPSCameraSystem::~FPSCameraSystem()
{
}


void FPSCameraSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	CameraComponent* camera_comp = go->GetComponent<CameraComponent>();

	BaseSystemCompNode *component_node = new FPSCameraCompNode(transform, camera_comp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void FPSCameraSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	using namespace DirectX;
	auto sys_comp = static_cast<FPSCameraCompNode*>(compNode);

	auto transform_comp = sys_comp->m_transform;
	auto camera_comp = sys_comp->m_camera_comp;


	XMFLOAT2 movement_amount(0.f, 0.f);
	if (gpInputManager->IsKeyPressed(SDL_SCANCODE_W))
	{
		movement_amount.y = 1.f;
	}

	if (gpInputManager->IsKeyPressed(SDL_SCANCODE_S))
	{
		movement_amount.y = -1.f;
	}

	if (gpInputManager->IsKeyPressed(SDL_SCANCODE_A))
	{
		movement_amount.x = -1.f;
	}

	if (gpInputManager->IsKeyPressed(SDL_SCANCODE_D))
	{
		movement_amount.x = 1.f;
	}
	XMVECTOR right = XMLoadFloat3(&camera_comp->m_camera->m_rightDir);

	if (gpInputManager->IsMousePressed(SDL_BUTTON_LEFT)
		&& !gpInputManager->IsMouseTriggered(SDL_BUTTON_LEFT))
	{

		Vector2 mouse_delta = gpInputManager->GetPointerDeltaVec2();
		XMFLOAT2 rot_amount;

		rot_amount.x = -mouse_delta.x * 100.f;
		rot_amount.y = -mouse_delta.y * 100.f;

		static const float rotation_rate = XMConvertToRadians(1.f);

		XMVECTOR rotation_vector = XMLoadFloat2(&rot_amount) * rotation_rate * dt;

		XMMATRIX pitch_mat = XMMatrixRotationAxis(right, XMVectorGetY(rotation_vector));
		XMMATRIX yaw_mat = XMMatrixRotationY(XMVectorGetX(rotation_vector));

		camera_comp->m_camera->ApplyRotation(XMMatrixMultiply(pitch_mat, yaw_mat));
	}

	
	Vector3 position = transform_comp->GetPosition();

	static const float movement_rate = 20.f;

	XMVECTOR movement = XMLoadFloat2(&movement_amount) * movement_rate * dt;

	XMVECTOR strafe = right * XMVectorGetX(movement);
	XMVECTOR forward = XMLoadFloat3(&camera_comp->m_camera->m_lookDir) * XMVectorGetY(movement);
	position += strafe;
	position += forward;
	XMFLOAT3 position3;
	XMStoreFloat3(&position3, position);
	camera_comp->m_camera->m_position = position3;
	transform_comp->SetLocalPosition(position3.x, position3.y, position3.z);
	camera_comp->m_camera->update_view_matrix();
	camera_comp->m_camera->update_view_projection_matrix();
}

void FPSCameraSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
}