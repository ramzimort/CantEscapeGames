#pragma once
#include "BaseComponent.h"

class UIComponent :	public BaseComponent
{
//Friend classes
public: 
	friend class UISystem;

public:
	UIComponent(GameObject *owner);
	virtual ~UIComponent();

	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	void Begin(GameObjectManager *goMgr) override;

public:
	//Unique class identifier
	static ComponentId const static_type;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};
