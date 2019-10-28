#include "UISystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/UIComponent.h"
#include "Components/RendererComponent.h"
#include "Graphics/AppRenderer.h"

unsigned const UISystem::static_type = BaseSystem::numberOfTypes++;

UISystem::UISystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<UIComponent>();
}

UISystem::~UISystem()
{
}

void UISystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	UIComponent* uiComp = go->GetComponent<UIComponent>();

	BaseSystemCompNode *component_node = new UICompNode(transform, uiComp);
	component_node->m_goID = go->GetId();
	this->m_ObjComponentsMap[go->GetId()] = component_node;
}
void UISystem::EarlyUpdate(float dt)
{
	for (auto& node : m_ObjComponentsMap)
	{
		UICompNode* uiNode = static_cast<UICompNode*>(node.second);
		UIComponent* uiComponent = uiNode->uiComp;
		TransformComponent* transformComponent = uiNode->m_transform;

		if (uiComponent->isTriggerd == true)
		{
			int test = 0;
		}

	}



}

void UISystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}

void UISystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	UICompNode* uiNode = static_cast<UICompNode*>(compNode);

	RendererComponent* rendererComponent = uiNode->m_transform->GetOwner()->GetComponent<RendererComponent>();

	if (!rendererComponent)
	{
		return;
	}

	Vector3 position = uiNode->m_transform->GetWorldPosition();
	Vector3 scale = uiNode->m_transform->GetScale();

	UIObjectInstanceRenderData uiObjectInstanceRenderData = {};
	uiObjectInstanceRenderData.m_windowSpacePosition = Vector2(position.x, position.y);
	uiObjectInstanceRenderData.m_windowSpaceSize = Vector2(scale.x, scale.y);
	uiObjectInstanceRenderData.m_pUIMaterial = rendererComponent->m_pMaterial;
	uiObjectInstanceRenderData.m_rotationMatrix = &uiNode->m_transform->GetRotationMatrix();

	m_pAppRenderer->RegisterUIObjectInstance(uiObjectInstanceRenderData);
}