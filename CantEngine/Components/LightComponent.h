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

	const Light* GetLight() const;
	Light* GetLight() { return &m_light; }

	void SetColor(const Vector3& color);
public:
	static ComponentId const static_type;

private:
	Light m_light;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

