#include "stdafx.h"
#include "FollowCurvesPathSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/TransformComponent.h"
#include "Components/FollowCurvesPathComponent.h"
#include "Components/SplineCurvesComponent.h"
#include "Graphics/AppRenderer.h"

unsigned const FollowCurvesPathSystem::static_type = BaseSystem::numberOfTypes++;

FollowCurvesPathSystem::FollowCurvesPathSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<FollowCurvesPathComponent>();
}


FollowCurvesPathSystem::~FollowCurvesPathSystem()
{
}


void FollowCurvesPathSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	FollowCurvesPathComponent* followCurvesPathComp = go->GetComponent<FollowCurvesPathComponent>();

	BaseSystemCompNode *component_node = new FollowCurvesPathCompNode(transform, followCurvesPathComp);
	component_node->m_goID = go->GetId();
	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void FollowCurvesPathSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	auto followCurvesPathCompNode = static_cast<FollowCurvesPathCompNode*>(compNode);
	auto transform_comp = followCurvesPathCompNode->m_transform;
	auto followCurvesPathComp = followCurvesPathCompNode->m_followCurvesPathComp;

	if (!followCurvesPathComp->m_enableMotionAlongPath || !followCurvesPathComp->m_motionPathCurveGameobj)
	{
		return;
	}
	if (!followCurvesPathComp->m_currentlyInReverseMotion)
	{
		followCurvesPathComp->m_curMotionTime += dt;
	}
	else
	{
		followCurvesPathComp->m_curMotionTime -= dt;
	}

	const auto& arclength_table = followCurvesPathComp->m_adaptiveArcLengthTable;
	float total_time_needed = arclength_table[arclength_table.size() - 1].dist / followCurvesPathComp->m_motionSpeed;
	float total_dist = 0.f;
	float t3 = total_time_needed;
	float t1 = 0.01f * total_time_needed;
	float t2 = 0.99f * total_time_needed;

	total_time_needed += ((t3 - t2) + t1);

	float local_time = followCurvesPathComp->m_curMotionTime;

	float v0 = followCurvesPathComp->m_motionSpeed;

	SplineCurvesComponent* spline_curves_comp = followCurvesPathComp->m_motionPathCurveGameobj->GetComponent<SplineCurvesComponent>();

	float normal_dist = v0 * (t1 / 2.f) + v0 * (local_time - t1);

	if (local_time < t1)
	{
		float eased_in_dist = (v0 * local_time * local_time) / (2.f * t1);
		total_dist = eased_in_dist;
	}
	if (local_time > t2 && local_time < total_time_needed)
	{
		float first_term = (v0 * t1 * 0.5f);
		float second_term = v0 * (t2 - t1);
		float third_term = (v0 - (v0 * (((local_time - t2) / (total_time_needed - t2))) / 2.f)) * (local_time - t2);

		total_dist = first_term + second_term + third_term;
	}

	if (local_time > t1 && local_time < t2)
	{
		total_dist = normal_dist;
	}

	float s_val = followCurvesPathComp->GetSplineCurvesParameterFromDistance(total_dist);

	if (local_time >= total_time_needed)
	{
		if (followCurvesPathComp->m_enableReverseMotion)
		{
			followCurvesPathComp->m_currentlyInReverseMotion = !followCurvesPathComp->m_currentlyInReverseMotion;
			followCurvesPathComp->m_curMotionTime = total_time_needed;
			total_dist = followCurvesPathComp->m_motionSpeed * followCurvesPathComp->m_curMotionTime;
			s_val = followCurvesPathComp->GetSplineCurvesParameterFromDistance(total_dist);
		}
		else
		{
			followCurvesPathComp->m_enableMotionAlongPath = false;
			return;
		}
		
	}
	else if (local_time <= 0.f)
	{
		if (followCurvesPathComp->m_enableReverseMotion)
		{
			followCurvesPathComp->m_currentlyInReverseMotion = !followCurvesPathComp->m_currentlyInReverseMotion;
			followCurvesPathComp->m_curMotionTime = 0.0001f;
			total_dist = followCurvesPathComp->m_motionSpeed * followCurvesPathComp->m_curMotionTime;
			s_val = followCurvesPathComp->GetSplineCurvesParameterFromDistance(total_dist);
		}
		else
		{
			followCurvesPathComp->m_enableMotionAlongPath = false;
			return;
		}
	}


	Vector3 newPos = spline_curves_comp->Evaluate(s_val);
	transform_comp->SetLocalPosition(newPos + followCurvesPathComp->m_offsetFollowPathPosition);

	if (followCurvesPathComp->m_enableMotionOrientation)
	{
		Vector3 curve_derivative = spline_curves_comp->EvaluateDerivative(s_val);
		curve_derivative.Normalize();
		Vector3 new_right = Vector3(0.f, 1.f, 0.f).Cross(curve_derivative);
		new_right.Normalize();
		Vector3 new_up = curve_derivative.Cross(new_right);
		new_up.Normalize();
		Matrix rotation_matrix(new_right, new_up, curve_derivative);
		transform_comp->SetLocalRotationMatrix(rotation_matrix);
	}
}