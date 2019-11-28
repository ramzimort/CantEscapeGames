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
	friend class FollowCurvesPathSystem;
public:
	FollowCurvesPathComponent(GameObject* ownerGameObj);
	virtual ~FollowCurvesPathComponent();

	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;

	void SetMotionSpeed(float motionSpeed);
	float GetMotionSpeed()const { return m_motionSpeed; }
	void SetCurveGameObjectToFollow(const std::string& gameObjectTag);
	void SetEnableMotionAlongPath(bool flag) { m_enableMotionAlongPath = flag; };
	bool IsMotionAlongPathEnabled()const { return m_enableMotionAlongPath; }
	void SetEnableMotionOrientation(bool flag) { m_enableMotionOrientation = flag; }
	bool GetEnableMotionOrientation() const { return m_enableMotionOrientation; }
	float GetSplineCurvesParameterFromDistance(float distance) const;

	void ResetMotionTime();
	void SetEnableReverseMotion(bool flag);
	void SetOffsetFollowPathPosition(const Vector3& pos) { m_offsetFollowPathPosition = pos; };
	Vector3 GetOffSetFollowPathPosition() const { return m_offsetFollowPathPosition; }
public:
	static ComponentId const static_type;
private:
	void InitAdaptiveArcLengthTable(GameObject* splineCurveGameObject);
private:
	StringId m_motionPathCurveGameobjTagName;
	GameObject* m_motionPathCurveGameobj;
	float m_arcLengthTolerance;
	float m_motionSpeed;
	float m_curMotionTime;
	bool m_enableMotionAlongPath;
	bool m_enableMotionOrientation;
	bool m_enableReverseMotion;
	bool m_currentlyInReverseMotion;
	//in World space
	Vector3 m_offsetFollowPathPosition;
	std::vector<ArcLengthData> m_adaptiveArcLengthTable;
	GameObjectManager* m_gameObjManager;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

