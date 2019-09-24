#include "MeshesComponent.h"




unsigned const MeshesComponent::static_type = BaseComponent::numberOfTypes++;

MeshesComponent::MeshesComponent(GameObject *owner)
	:BaseComponent(owner, static_type)
{
}

MeshesComponent::~MeshesComponent()
{
}


void MeshesComponent::Init()
{
}


void MeshesComponent::Begin()
{
}

Model* MeshesComponent::GetModel()
{
	return m_model;
}

void MeshesComponent::SetModel(Model* model)
{
	m_model = model;
}