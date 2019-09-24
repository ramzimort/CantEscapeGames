/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "RendererComponent.h"
unsigned const RendererComponent::static_type = BaseComponent::numberOfTypes++;


RendererComponent::RendererComponent(GameObject *owner) :
	BaseComponent(owner, RendererComponent::static_type)
{
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::Init()
{
}

void RendererComponent::Begin()
{
}

void RendererComponent::SetMaterial(Material* material)
{
	m_pMaterial = material;
}