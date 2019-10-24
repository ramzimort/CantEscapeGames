#pragma once

#include "BaseComponent.h"
#include "Helper/Hash.h"

class Texture;

class SkyboxIrradianceComponent :
	public BaseComponent
{
public:
	friend class SkyboxIrradianceSystem;
public:
	SkyboxIrradianceComponent(GameObject *owner);
	~SkyboxIrradianceComponent();

	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;
public:
	//Unique class identifier
	static ComponentId const static_type;
private:
	StringId m_skyboxEquirectangularTextureID;
	StringId m_skyboxEquirectangularIrradianceTextureID;

	Texture* m_pSkyboxEquirectangularTexture;
	Texture* m_pSkyboxEquirectangularIrradianceTexture;

	Texture* m_pSkyboxTexture;
	Texture* m_pSkyboxIrradianceTexture;

	Texture* m_pIblPrefilteredEnvMapTexture;

	bool m_hasBakedData;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

