#include "MeshComponent.h"

unsigned const MeshComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<MeshComponent>("MeshComponent")
		.property("Model", &MeshComponent::m_model)
		.property("Name", &MeshComponent::m_model_name)
		.property("Type", &MeshComponent::m_mesh_type)
		.property("MissingTangents", &MeshComponent::m_calculate_missing_tangents)
		.method("Init", &MeshComponent::Init)
		.method("Begin", &MeshComponent::Begin);

	rttr::registration::enumeration<MeshType>("MeshType")(
		rttr::value("MESH_QUAD", MeshType::MESH_QUAD),
		rttr::value("MESH_CUBE", MeshType::MESH_CUBE),
		rttr::value("MESH_MODEL", MeshType::MESH_MODEL),
		rttr::value("MESH_TEXTURED_MODEL", MeshType::MESH_TEXTURED_MODEL),
		rttr::value("MESH_PLANE", MeshType::MESH_PLANE));

}

MeshComponent::MeshComponent(GameObject *owner)
	:BaseComponent(owner, static_type)
{
}

MeshComponent::~MeshComponent()
{
}


void MeshComponent::Init()
{
}


void MeshComponent::Begin()
{
}

Model* MeshComponent::GetModel()
{
	return m_model;
}

void MeshComponent::SetModel(Model* model)
{
	m_model = model;
}