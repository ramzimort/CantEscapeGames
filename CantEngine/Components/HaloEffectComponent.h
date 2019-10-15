#pragma once
#include "BaseComponent.h"
#include "Graphics/HaloEffect.h"

class HaloEffectComponent :
	public BaseComponent
{
public:
	friend class HaloEffectSystem;
public:
	HaloEffectComponent(GameObject *owner);
	virtual ~HaloEffectComponent();

	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;

public:
	static ComponentId const static_type;
private:
	HaloEffect m_haloEffect;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

