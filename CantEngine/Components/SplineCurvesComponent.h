#pragma once
#include "BaseComponent.h"

class TransformComponent;

struct SplineControlPoints
{
	float x, y, z;
};

class SplineCurvesComponent :
	public BaseComponent
{
public:
	friend class SplineCurvesSystem;
public:
	SplineCurvesComponent(GameObject *owner);
	virtual ~SplineCurvesComponent();

	void Begin(GameObjectManager *goMgr) override;
	void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	Vector3 Evaluate(float global_t) const;
	Vector3 EvaluateDerivative(float global_t) const;
	void GetWorldControlPoints(std::vector<Vector3>& worldPoints) const;
	const std::vector<Vector3>& GetControlPoints() const { return m_splineControlPoints; }
public:
	static ComponentId const static_type;
private:
	void TransformAlternatePointsToControlPoints();
private:
	std::vector<Vector3> m_splineControlPoints;
	std::vector<SplineControlPoints> m_alternatePoints;
	Matrix m_splineMatrix;
	TransformComponent* m_ownerTransformComp;
	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

