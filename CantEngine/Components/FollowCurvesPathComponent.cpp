#include "stdafx.h"
#include "FollowCurvesPathComponent.h"
#include "SplineCurvesComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<FollowCurvesPathComponent>("FollowCurvesPathComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.method("Init", &FollowCurvesPathComponent::Init);
}


unsigned const FollowCurvesPathComponent::static_type = BaseComponent::numberOfTypes++;

FollowCurvesPathComponent::FollowCurvesPathComponent(GameObject* ownerGameObj)
	:BaseComponent(ownerGameObj, static_type),
	m_motionSpeed(1.f),
	m_arcLengthTolerance(0.001f),
	m_motionPathCurveGameobjTagName("")
{
}


FollowCurvesPathComponent::~FollowCurvesPathComponent()
{
}

void FollowCurvesPathComponent::Begin(GameObjectManager *goMgr)
{
	m_gameObjManager = goMgr;
	InitAdaptiveArcLengthTable();
}

void FollowCurvesPathComponent::Init(ResourceManager* resMgr, DXRenderer* dxrenderer)
{
}

void FollowCurvesPathComponent::SetMotionSpeed(float motionSpeed)
{
	m_motionSpeed = motionSpeed;
}


void FollowCurvesPathComponent::SetCurveGameObjectToFollow(const std::string& gameObjectTag)
{
	m_motionPathCurveGameobjTagName = gameObjectTag;
	InitAdaptiveArcLengthTable();
}

float FollowCurvesPathComponent::GetSplineCurvesParameterFromDistance(float distance) const
{
	float s0Index = 0.f;
	float s1Index = 0.f;

	float s0Dist = 0.f;
	float s1Dist = 0.f;

	float alphaVal = 0.f;
	for (int index = 1; index < m_adaptiveArcLengthTable.size(); ++index)
	{
		const ArcLengthData& prevArcLengthData = m_adaptiveArcLengthTable[index - 1];
		const ArcLengthData& arcLengthData = m_adaptiveArcLengthTable[index];
		if (prevArcLengthData.dist <= distance && distance <= arcLengthData.dist)
		{
			s0Index = prevArcLengthData.s;
			s1Index = arcLengthData.s;
			alphaVal = (distance - prevArcLengthData.dist) / (arcLengthData.dist - prevArcLengthData.dist);
			break;
		}
	}
	if (s0Index == 0.f && s1Index == 0.f)
	{
		return -1.f;
	}

	float finalS = s0Index + alphaVal * (s1Index - s0Index);
	return finalS;
}

void FollowCurvesPathComponent::InitAdaptiveArcLengthTable()
{
	GameObject* ownerGameObj = GetOwner();
	if (m_motionPathCurveGameobjTagName == StringId(""))
	{
		return;
	}
	GameObject* curveGameobj = m_gameObjManager->FindGameObject(m_motionPathCurveGameobjTagName.getName());
	if (!curveGameobj)
	{
		return;
	}
	m_motionPathCurveGameobj = curveGameobj;
	SplineCurvesComponent* splineCurveComp = m_motionPathCurveGameobj->GetComponent<SplineCurvesComponent>();

	const std::vector<Vector3>& controlPoints = splineCurveComp->GetControlPoints();
	struct StackData
	{
		float s0;
		float s1;
	};
	std::map<float, float> adaptiveArcLengthMap;
	std::stack<StackData> adaptiveStacks;
	adaptiveStacks.push(StackData{ 0.f, 1.f });

	while (!adaptiveStacks.empty())
	{
		StackData stackData = adaptiveStacks.top();
		adaptiveStacks.pop();

		float half_s = (stackData.s1 + stackData.s0) / 2.f;

		Vector3 evaluatedHalfSPos = splineCurveComp->Evaluate(half_s);
		Vector3 evaluatedS0Pos = splineCurveComp->Evaluate(stackData.s0);
		Vector3 evaluatedS1Pos = splineCurveComp->Evaluate(stackData.s1);

		float A_term = (evaluatedHalfSPos - evaluatedS0Pos).Length();
		float B_term = (evaluatedS1Pos - evaluatedHalfSPos).Length();
		float C_term = (evaluatedS1Pos - evaluatedS0Pos).Length();

		float d_term = A_term + B_term - C_term;

		if (d_term <= m_arcLengthTolerance)
		{
			adaptiveArcLengthMap.insert(std::make_pair(half_s, A_term));
			adaptiveArcLengthMap.insert(std::make_pair(stackData.s1, A_term + B_term));
		}
		else
		{
			adaptiveStacks.push(StackData{ stackData.s0, half_s });
			adaptiveStacks.push(StackData{ half_s, stackData.s1 });
		}
	}
	float prevLength = 0.f;
	m_adaptiveArcLengthTable.clear();
	m_adaptiveArcLengthTable.push_back(ArcLengthData{ 0.f, 0.f });
	for (auto& pair : adaptiveArcLengthMap)
	{
		float cur_length = pair.second + prevLength;
		m_adaptiveArcLengthTable.push_back(ArcLengthData{ pair.first, cur_length });
		prevLength = cur_length;
	}
}