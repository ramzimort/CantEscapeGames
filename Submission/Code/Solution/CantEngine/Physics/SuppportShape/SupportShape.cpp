/**
 * @file SupportShape.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#include "SupportShape.h"
#include "Physics/PhysicsUtils.h"

Vector3 SupportShape::GetCenter(const std::vector<VertexData>& localPoints, const Matrix& localToWorldTransform) const
{
	const Vector3 firstPoint = localPoints[0].m_position;
	Vector3 min(firstPoint.x, firstPoint.y, firstPoint.z);
	Vector3 max(min);
	
	for (const VertexData& point : localPoints)
	{
		min = PhysicsUtils::Min(min, point.m_position);
		max = PhysicsUtils::Max(max, point.m_position);
	}

	const Vector3 center((max - min) * 0.5f);

	return Vector3::Transform(center, localToWorldTransform);
}

Vector3 SupportShape::Support(const Vector3& worldDirection, const std::vector<VertexData>& localPoints,
	const Matrix& localToWorldTransform) const
{
	const Vector3 localDirection = Vector3::Transform(worldDirection, localToWorldTransform.Invert());

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
	
	return Vector3::Transform(localPoints[maxIndex].m_position, localToWorldTransform);
}

