/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "RendererComponent.h"
#include "Graphics/Material.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Managers/GameObjectManager.h"

unsigned const RendererComponent::static_type = BaseComponent::numberOfTypes++;

RTTR_REGISTRATION
{
	rttr::registration::class_<RendererComponent>("RendererComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("MaterialId", &RendererComponent::m_materialId)
		.property("XTileFactor", &RendererComponent::m_xTileFactor)
		.property("YTileFactor", &RendererComponent::m_yTileFactor)
		.method("Init", &RendererComponent::Init);

}

RendererComponent::RendererComponent(GameObject *owner) :
	BaseComponent(owner, RendererComponent::static_type),
	m_xTileFactor(1.f),
	m_yTileFactor(1.f)
{
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::Init(ResourceManager* resMgr)
{
	m_pMaterial = resMgr->GetMaterial(m_materialId);
}

void RendererComponent::Begin(GameObjectManager *goMgr)
{
}

void RendererComponent::SetMaterial(Material* material)
{
	m_pMaterial = material;
}

void RendererComponent::SetXTileFactor(float x_factor)
{
	m_xTileFactor = x_factor;
}
void RendererComponent::SetYTileFactor(float y_factor)
{
	m_yTileFactor = y_factor;
}