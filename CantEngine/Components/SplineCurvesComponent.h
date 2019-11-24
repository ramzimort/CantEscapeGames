#pragma once
#include "BaseComponent.h"

class SplineCurvesComponent :
	public BaseComponent
{
public:
	SplineCurvesComponent(GameObject *owner);
	virtual ~SplineCurvesComponent();


	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
public:
	static ComponentId const static_type;
private:

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

