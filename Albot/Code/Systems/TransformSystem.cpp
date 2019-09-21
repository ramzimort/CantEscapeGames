/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "TransformSystem.h"
#include "../GameObjects/GameObject.h"
unsigned const TransformSystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../Components/TransformComponent.h"


TransformSystem::TransformSystem() :
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<Transform>();
}


void TransformSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	Transform *transform = go->GetComponent<Transform>();

	TransformCompNode *component_node = new TransformCompNode(transform);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void TransformSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	TransformCompNode *node = static_cast<TransformCompNode*>(compNode);
	Transform *transformComp = node->n_transform;

	//UPDATE CODE GOES HERE
	
	//Skip calculations if no need to recalculate
	if (!transformComp->NeedToRecalculate())
		return;

	//Translation
	Matrix T = Matrix::CreateTranslation(transformComp->GetPosition());

	//Rotation
	Vector3 const& rot = transformComp->GetRotation();
	float xRad = DirectX::XMConvertToRadians(rot.x);
	float yRad = DirectX::XMConvertToRadians(rot.y);
	float zRad = DirectX::XMConvertToRadians(rot.z);
	Matrix R = Matrix::CreateRotationX(xRad) * 
		Matrix::CreateRotationY(yRad) * 
		Matrix::CreateRotationZ(zRad);

	//Scale
	Matrix H = Matrix::CreateScale(transformComp->GetScale());

	//Model
	transformComp->SetModel(T*R*H);

	transformComp->ResetDirtyFlag();
}