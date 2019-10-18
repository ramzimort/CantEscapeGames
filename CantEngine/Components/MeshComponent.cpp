
#include "MeshComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

unsigned const MeshComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<MeshComponent>("MeshComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("ModelId", &MeshComponent::m_modelId)
		.property("Type", &MeshComponent::m_mesh_type)
		.property("MissingTangents", &MeshComponent::m_calculate_missing_tangents)
		.method("Init", &MeshComponent::Init);


	rttr::registration::enumeration<MeshType>("MeshType")(
		rttr::value("Quad", MeshType::MESH_QUAD),
		rttr::value("Cube", MeshType::MESH_CUBE),
		rttr::value("Model", MeshType::MESH_MODEL),
		rttr::value("TexturedModel", MeshType::MESH_TEXTURED_MODEL),
		rttr::value("Plane", MeshType::MESH_PLANE));

}

MeshComponent::MeshComponent(GameObject *owner)
	:BaseComponent(owner, static_type)
{
}

MeshComponent::~MeshComponent()
{
}


void MeshComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	m_model = resMgr->GetModel(m_modelId);
}


void MeshComponent::Begin(GameObjectManager *goMgr)
{
}

Model* MeshComponent::GetModel()
{
	return m_model;
}