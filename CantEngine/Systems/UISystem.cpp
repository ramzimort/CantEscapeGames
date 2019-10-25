#include "UISystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/UIComponent.h"
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