#pragma once
#include "BaseComponent.h"
#define MAX_SPLINE_CONTROL_POINTS 50
class TransformComponent;

struct SplineControlPoint
{
	float x, y, z;
};

class SplineCurvesComponent :
	public BaseComponent
{
public:
	friend class SplineCurvesSystem;
	friend class DebugManager;
public:
	SplineCurvesComponent(GameObject *owner);
	virtual ~SplineCurvesComponent();

	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	Vector3 Evaluate(float global_t) const;
	Vector3 EvaluateDerivative(float global_t) const;
	void GetWorldControlPoints(std::vector<Vector3>& worldPoints) const;
	const std::vector<Vector3>& GetControlPoints() const { return m_splineControlPoints; }
	void SetSplineControlPointsNum(int32_t splineControlPointsNum) { m_splineControlPointsNum = splineControlPointsNum; }
	void SetSplineAlternatePoint(uint32_t index, const Vector3& p);
	int32_t GetSplineControlPointsNum() const { return m_splineControlPointsNum; }
	std::vector<SplineControlPoint>& GetAlternatePoints() { return m_alternatePoints; }
public:
	static ComponentId const static_type;
private:
	void TransformAlternatePointsToControlPoints();
private:
	std::vector<Vector3> m_splineControlPoints;
	std::vector<SplineControlPoint> m_alternatePoints;
	int32_t m_splineControlPointsNum;
	Matrix m_splineMatrix;
	TransformComponent* m_ownerTransformComp;
	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

