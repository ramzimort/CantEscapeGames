#pragma once
#include "BaseComponent.h"
#include "Helper/Hash.h"

struct ArcLengthData
{
	float s;
	float dist;
};

class FollowCurvesPathComponent :
	public BaseComponent
{
public:
	FollowCurvesPathComponent(GameObject* ownerGameObj);
	virtual ~FollowCurvesPathComponent();

	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;

	void SetMotionSpeed(float motionSpeed);
	void SetCurveGameObjectToFollow(const std::string& gameObjectTag);
	float GetSplineCurvesParameterFromDistance(float distance) const;
public:
	static ComponentId const static_type;
private:
	void InitAdaptiveArcLengthTable();
private:
	StringId m_motionPathCurveGameobjTagName;
	GameObject* m_motionPathCurveGameobj;
	float m_arcLengthTolerance;
	float m_motionSpeed;
	std::vector<ArcLengthData> m_adaptiveArcLengthTable;
	GameObjectManager* m_gameObjManager;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

