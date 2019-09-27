/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "RendererComponent.h"
#include "Graphics/Material.h"

unsigned const RendererComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<RendererComponent>("RendererComponent")
		.property("Name", &RendererComponent::m_material_name)
		.property("Material", &RendererComponent::m_pMaterial)
		.property("XTileFactor", &RendererComponent::m_x_tile_factor)
		.property("YTileFactor", &RendererComponent::m_y_tile_factor)
		.method("Init", &RendererComponent::Init)
		.method("Begin", &RendererComponent::Begin);
}

RendererComponent::RendererComponent(GameObject *owner) :
	BaseComponent(owner, RendererComponent::static_type),
	m_x_tile_factor(1.f),
	m_y_tile_factor(1.f)
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

void RendererComponent::SetXTileFactor(float x_factor)
{
	m_x_tile_factor = x_factor;
}
void RendererComponent::SetYTileFactor(float y_factor)
{
	m_y_tile_factor = y_factor;
}