/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "ModelSupportShape.h"

ModelSupportShape::ModelSupportShape(const Model& model, const TransformComponent& transform) : m_model(model), m_transform(transform)
{

}

Vector3 ModelSupportShape::GetCenter() const
{
	return m_transform.GetPosition();

	//const Aabb localAabb = m_model->GetAABB();
	//Vector3 diagonal = localAabb.m_Max - localAabb.m_Min;
	//Vector3 center = localAabb.m_Min + (diagonal) * 0.5f;
	//center.x /= abs(diagonal.x);
	//center.y /= abs(diagonal.y);
	//center.z /= abs(diagonal.z);
	//return Vector3::Transform(center, m_modelToWorld);

	//return SupportShape::GetCenter(m_model->GetVerticesList(), m_modelToWorld);

}

Vector3 ModelSupportShape::Support(const Vector3& worldDirection) const
{
	const Matrix& modelToWorld = m_transform.GetModel();

	Vector3 localDirection = Vector3::Transform(worldDirection, modelToWorld.Invert());
	const Aabb& localAabb = m_model.GetAABB();
	Vector3 diagonal = localAabb.m_Max - localAabb.m_Min;
	localDirection.x *= abs(diagonal.x);
	localDirection.y *= abs(diagonal.y);
	localDirection.z *= abs(diagonal.z);
	const Model::VerticesList& localPoints = m_model.GetVerticesList();
	float maxProj = -FLT_MAX;
	size_t maxIndex = 0;
	for (unsigned int i = 0; i < localPoints.size(); ++i)
	{
		const float proj = localPoints[i].m_position.Dot(localDirection);
		if (proj > maxProj)
		{
			maxProj = proj;
			maxIndex = i;
		}
	}

	return Vector3::Transform(localPoints[maxIndex].m_position, modelToWorld);
	//return SupportShape::Support(worldDirection, m_model->GetVerticesList(), m_modelToWorld);
}
