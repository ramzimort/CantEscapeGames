#include "stdafx.h"
#include "SplineCurvesSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/SplineCurvesComponent.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/DebugRendering/DebugRendering.h"
#include "Graphics/GraphicsSettings.h"


unsigned const SplineCurvesSystem::static_type = BaseSystem::numberOfTypes++;

SplineCurvesSystem::SplineCurvesSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<SplineCurvesComponent>();
}


SplineCurvesSystem::~SplineCurvesSystem()
{
}


void SplineCurvesSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	SplineCurvesComponent* splineCurvesComp = go->GetComponent<SplineCurvesComponent>();

	BaseSystemCompNode *component_node = new SplineCurvesCompNode(transform, splineCurvesComp);
	component_node->m_goID = go->GetId();
	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void SplineCurvesSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	auto splineCompNode = static_cast<SplineCurvesCompNode*>(compNode);
	auto transform_comp = splineCompNode->m_transform;
	auto curvesComp = splineCompNode->m_splineCurvesComp;
	curvesComp->TransformAlternatePointsToControlPoints();
}

void SplineCurvesSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	auto splineCompNode = static_cast<SplineCurvesCompNode*>(compNode);
	auto transform_comp = splineCompNode->m_transform;
	auto curvesComp = splineCompNode->m_splineCurvesComp;

#ifdef DEVELOPER
	if (!GraphicsSettings::Draw_Spline_Curve)
	{
		return;
	}
	float steps = 0.01f;
	Vector3 prevPosition = curvesComp->Evaluate(0.f);
	for (float index = steps; index <= (1.f); index += steps)
	{
		Vector3 curPosition = curvesComp->Evaluate(index);
		DebugLineInstance debugLineInstance = {};
		debugLineInstance.m_startpos = prevPosition;
		debugLineInstance.m_endpos = curPosition;
		debugLineInstance.m_color = Vector3(1.f, 0.f, 0.f);
		m_pAppRenderer->GetDebugRendering().RegisterDebugLineInstance(debugLineInstance);
		prevPosition = curPosition;
	}
	std::vector<Vector3> splineWorldControlPoints;
	curvesComp->GetWorldControlPoints(splineWorldControlPoints);
	for (int32_t index = 0; index < curvesComp->m_splineControlPointsNum; ++index)
	{
		Vector3 position = splineWorldControlPoints[index];
		Vector3 half_size(0.01f);
		DebugAABBInstance debugAABBInstance = {};
		debugAABBInstance.m_min_bound = position - half_size;
		debugAABBInstance.m_max_bound = position + half_size;
		debugAABBInstance.m_color = Vector3(0.f, 0.f, 1.f);
		m_pAppRenderer->GetDebugRendering().RegisterDebugAABB(debugAABBInstance);
	}
#endif
}

void SplineCurvesSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}