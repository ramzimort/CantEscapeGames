#pragma once
///Includes
#include "BaseSystem.h"
#include "Managers/EventManager.h"

class UIComponent;
class TransformComponent;
class AppRenderer;

struct UICompNode : BaseSystemCompNode
{
	TransformComponent *m_transform;
	UIComponent* uiComp;
	//Ctor
	UICompNode(TransformComponent *transform,
		UIComponent* _uiComp) : m_transform(transform),
		uiComp(_uiComp)
	{}
};

class UISystem : public BaseSystem
{
public:
	UISystem();
	virtual ~UISystem();

	virtual void Register_GameObject(GameObject *go) override;
	virtual void EarlyUpdate(float dt) override;

	//virtual void Draw(float dt, BaseSystemCompNode *compNode) override;

	void RegisterAppRenderer(AppRenderer* appRenderer);

public:
	//To compare when using templates
	static unsigned int const static_type;
private:
	AppRenderer* m_pAppRenderer;

	//Current UI State
	int currentState;
	//Previous UI state
	int previousstate;
};