#include "stdafx.h"
#include "SplineCurvesComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"
#include "Components/TransformComponent.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<SplineControlPoints>("SplineControlPoints")
	.property("x", &SplineControlPoints::x)
	.property("y", &SplineControlPoints::y)
	.property("z", &SplineControlPoints::z);
	/*.property("Point", &SplineControlPoints::m_point)
	.property("Name", &SplineControlPoints::m_name);*/

	rttr::registration::class_<SplineCurvesComponent>("SplineCurvesComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("SplineControlPoints", &SplineCurvesComponent::m_splineControlPoints)
		.property("SplineAlternatePoints", &SplineCurvesComponent::m_alternatePoints)
		.method("Init", &SplineCurvesComponent::Init);
}

unsigned const SplineCurvesComponent::static_type = BaseComponent::numberOfTypes++;

SplineCurvesComponent::SplineCurvesComponent(GameObject* owner)
	:BaseComponent(owner, static_type)
{
}


SplineCurvesComponent::~SplineCurvesComponent()
{
}

void SplineCurvesComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
	m_splineMatrix = Matrix(
		Vector4(-1.f, 3.f, -3.f, 1.f),
		Vector4(2.f, -5.f, 4.f, -1.f),
		Vector4(-1.f, 0.f, 1.f, 0.f),
		Vector4(0.f, 2.f, 0.f, 0.f));
	m_splineMatrix *= (1.f / 2.f);
	TransformAlternatePointsToControlPoints();
	m_ownerTransformComp = GetOwner()->GetComponent<TransformComponent>();
}

void SplineCurvesComponent::Begin(GameObjectManager *goMgr)
{

}

Vector3 SplineCurvesComponent::Evaluate(float global_t) const
{
	Vector3 ownerPosition = m_ownerTransformComp->GetPosition();
	size_t controlPointsSize = m_splineControlPoints.size();
	assert(controlPointsSize >= 4);
	size_t segmentCount = controlPointsSize - 3;

	float perSegmentLocalTBound = 1.f / (float)segmentCount;
	uint32_t start_index = std::min((uint32_t)(segmentCount - 1), (uint32_t)std::floor((global_t / perSegmentLocalTBound)));

	float beginSegmentBoundaryValue = (float)start_index * perSegmentLocalTBound;
	float localT = (global_t - beginSegmentBoundaryValue) / perSegmentLocalTBound;
	Matrix controlPointsMatrix
	(
		MathUtil::v3_to_v4(m_splineControlPoints[start_index] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 1] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 2] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 3] + ownerPosition)
	);

	float localT2 = localT * localT;
	float localT3 = localT2 * localT;
	Vector4 tVectorMatrix(localT3, localT2, localT, 1.f);

	Matrix finalMatrix = m_splineMatrix * controlPointsMatrix;
	return MathUtil::v4_to_v3(Vector4::Transform(tVectorMatrix, finalMatrix));
}

Vector3 SplineCurvesComponent::EvaluateDerivative(float global_t) const
{
	Vector3 ownerPosition = m_ownerTransformComp->GetPosition();
	size_t controlPointsSize = m_splineControlPoints.size();
	assert(controlPointsSize >= 4);
	size_t segmentCount = controlPointsSize - 3;

	float perSegmentLocalTBound = 1.f / (float)segmentCount;
	uint32_t start_index = std::min((uint32_t)(segmentCount - 1), (uint32_t)std::floor((global_t / perSegmentLocalTBound)));

	float beginSegmentBoundaryValue = (float)start_index * perSegmentLocalTBound;
	float localT = (global_t - beginSegmentBoundaryValue) / perSegmentLocalTBound;
	Matrix controlPointsMatrix
	(
		MathUtil::v3_to_v4(m_splineControlPoints[start_index] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 1] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 2] + ownerPosition),
		MathUtil::v3_to_v4(m_splineControlPoints[start_index + 3] + ownerPosition)
	);

	float localT2 = localT * localT;

	float derivateLocalT3 = 3.f * localT2;
	float derivateLocalT2 = 2.f * localT;

	Vector4 t_vector_matrix(derivateLocalT3, derivateLocalT2, 1.f, 0.f);

	Matrix finalMatrix = m_splineMatrix * controlPointsMatrix;
	return MathUtil::v4_to_v3(Vector4::Transform(t_vector_matrix, finalMatrix));
}

void SplineCurvesComponent::TransformAlternatePointsToControlPoints()
{
	m_splineControlPoints.clear();
	for (const auto& splineControl : m_alternatePoints)
	{
		m_splineControlPoints.push_back(Vector3(splineControl.x, splineControl.y, splineControl.z));
	}
}

void SplineCurvesComponent::GetWorldControlPoints(std::vector<Vector3>& worldPoints) const
{
	worldPoints.clear();
	Vector3 ownerPosition = m_ownerTransformComp->GetPosition();
	for (const auto& splineVec3Point : m_splineControlPoints)
	{
		worldPoints.push_back(ownerPosition + splineVec3Point);
	}
}