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

		
	}
}

void UISystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}

void UISystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	UICompNode* uiNode = static_cast<UICompNode*>(compNode);
	UIComponent* uiComponent = uiNode->uiComp;
	if (uiComponent->m_enabled == false)
	{
		return;
	}
	RendererComponent* rendererComponent = uiNode->m_transform->GetOwner()->GetComponent<RendererComponent>();

	if (!rendererComponent)
	{
		return;
	}

	Vector3 position = uiNode->m_transform->GetPosition();
	Vector3 scale = uiNode->m_transform->GetScale();
	

	UIObjectInstanceRenderData uiObjectInstanceRenderData = {};
	uiObjectInstanceRenderData.m_windowSpacePosition = Vector3(position.x, position.y, position.z);
	uiObjectInstanceRenderData.m_windowSpaceSize = Vector3(scale.x, scale.y, 0.01f);
	uiObjectInstanceRenderData.m_pUIMaterial = rendererComponent->m_pMaterial;
	uiObjectInstanceRenderData.m_rotationMatrix = &uiNode->m_transform->GetRotationMatrix();
	m_pAppRenderer->RegisterUIObjectInstance(uiObjectInstanceRenderData);
	
	
	m_pAppRenderer->GetSDLWindow();

	if (uiComponent->m_textUI != L"")
	{
		Vector2 ui_2d_position(position.x, position.y);
		int width, height;
		SDL_GetWindowSize(&m_pAppRenderer->GetSDLWindow(), &width, &height);
		ui_2d_position.x /= (float)width;
		ui_2d_position.y /= (float)height;
		m_pAppRenderer->RegisterTextFontInstance(uiComponent->m_textUI, uiComponent->m_textFontType, uiComponent->m_textPosition + ui_2d_position,
			uiComponent->m_textColor, uiComponent->m_textScale, uiComponent->m_textRotation);
	}
}