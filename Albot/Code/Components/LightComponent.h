#pragma once

#include "BaseComponent.h"
#include "Graphics/Light.h"


class GameObjectManager;


class LightComponent :
	public BaseComponent
{
public:
	friend class LightSystem;
public:
	LightComponent(GameObject *owner);
	virtual ~LightComponent();

	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	void Begin(GameObjectManager *goMgr) override;

	//TODO: temp function to set light from lambda
	//light data should be serliaized
	void SetLight(const Light& light) { m_light = light; }
	const Light* GetLight() const;

public:
	static ComponentId const static_type;

private:
	Light m_light;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

