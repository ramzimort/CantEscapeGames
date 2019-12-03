/**
 * @file Aabb.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
#include "Aabb.h"
#include "Intersection.h"
#include "Physics/PhysicsUtils.h"
#include "Graphics/AppRenderer.h"


Aabb::Aabb() : m_Min(FLT_MAX), m_Max(-FLT_MAX)
{
}

Aabb::Aabb(const Vector3& min, const Vector3& max) : m_Min(min), m_Max(max)
{
}

Aabb Aabb::BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents)
{
	return Aabb(center - halfExtents, center + halfExtents); 
}

Aabb Aabb::BuildFromMinMax(const Vector3& min, const Vector3& max)
{
	return Aabb(min, max);
}

Aabb Aabb::BuildFromLocalAABBAndModelMatrix(const Matrix& modelMatrix, const Aabb& localAABB)
{
	const Vector3 translationPortion(modelMatrix._41, modelMatrix._42, modelMatrix._43);

	const Vector3& oriMin = localAABB.m_Min;
	const Vector3& oriMax = localAABB.m_Max;

	Vector3 worldMinPoint, worldMaxPoint;

	worldMinPoint = translationPortion;
	worldMaxPoint = translationPortion;


	float m11, m12, m13, m21, m22, m23, m31, m32, m33;

	m11 = modelMatrix._11;
	m12 = modelMatrix._12;
	m13 = modelMatrix._13;
	m21 = modelMatrix._21;
	m22 = modelMatrix._22;
	m23 = modelMatrix._23;
	m31 = modelMatrix._31;
	m32 = modelMatrix._32;
	m33 = modelMatrix._33;

	if (m11 > 0.f)
	{
		worldMinPoint.x += m11 * oriMin.x;
		worldMaxPoint.x += m11 * oriMax.x;
	}
	else
	{
		worldMinPoint.x += m11 * oriMax.x;
		worldMaxPoint.x += m11 * oriMin.x;
	}


	if (m12 > 0.f)
	{
		worldMinPoint.y += m12 * oriMin.x;
		worldMaxPoint.y += m12 * oriMax.x;
	}
	else
	{
		worldMinPoint.y += m12 * oriMax.x;
		worldMaxPoint.y += m12 * oriMin.x;
	}

	if (m13 > 0.f)
	{
		worldMinPoint.z += m13 * oriMin.x;
		worldMaxPoint.z += m13 * oriMax.x;
	}
	else
	{
		worldMinPoint.z += m13 * oriMax.x;
		worldMaxPoint.z += m13 * oriMin.x;
	}

	//

	if (m21 > 0.f)
	{
		worldMinPoint.x += m21 * oriMin.y;
		worldMaxPoint.x += m21 * oriMax.y;
	}
	else
	{
		worldMinPoint.x += m21 * oriMax.y;
		worldMaxPoint.x += m21 * oriMin.y;
	}


	if (m22 > 0.f)
	{
		worldMinPoint.y += m22 * oriMin.y;
		worldMaxPoint.y += m22 * oriMax.y;
	}
	else
	{
		worldMinPoint.y += m22 * oriMax.y;
		worldMaxPoint.y += m22 * oriMin.y;
	}

	if (m23 > 0.f)
	{
		worldMinPoint.z += m23 * oriMin.y;
		worldMaxPoint.z += m23 * oriMax.y;
	}
	else
	{
		worldMinPoint.z += m23 * oriMax.y;
		worldMaxPoint.z += m23 * oriMin.y;
	}

	//


	if (m31 > 0.f)
	{
		worldMinPoint.x += m31 * oriMin.z;
		worldMaxPoint.x += m31 * oriMax.z;
	}
	else
	{
		worldMinPoint.x += m31 * oriMax.z;
		worldMaxPoint.x += m31 * oriMin.z;
	}


	if (m32 > 0.f)
	{
		worldMinPoint.y += m32 * oriMin.z;
		worldMaxPoint.y += m32 * oriMax.z;
	}
	else
	{
		worldMinPoint.y += m32 * oriMax.z;
		worldMaxPoint.y += m32 * oriMin.z;
	}

	if (m33 > 0.f)
	{
		worldMinPoint.z += m33 * oriMin.z;
		worldMaxPoint.z += m33 * oriMax.z;
	}
	else
	{
		worldMinPoint.z += m33 * oriMax.z;
		worldMaxPoint.z += m33 * oriMin.z;
	}

	return BuildFromMinMax(worldMinPoint, worldMaxPoint);
}


float Aabb::GetVolume() const
{
	return abs((m_Max.x - m_Min.x) * (m_Max.y - m_Min.y) * (m_Max.z - m_Min.z));
}

float Aabb::GetSurfaceArea() const
{
	return 2.0f * (abs((m_Max.x - m_Min.x) * (m_Max.y - m_Min.y)) +
		abs((m_Max.x - m_Min.x) * (m_Max.z - m_Min.z)) +
		abs((m_Max.y - m_Min.y) * (m_Max.z - m_Min.z)));
}

void Aabb::Compute(const std::vector<Vector3>& points)
{
	m_Min = Vector3(FLT_MAX);
	m_Max = Vector3(-FLT_MAX);

	for (const Vector3& point : points)
	{
		m_Min = PhysicsUtils::Min(m_Min, point);
		m_Max = PhysicsUtils::Max(m_Max, point);
	}
}

bool Aabb::IsIntersect(const Aabb& aabb) const
{
	return Intersection::AabbAabb(m_Min, m_Max, aabb.m_Min, aabb.m_Max);
}

bool Aabb::IsIntersect(const Sphere& sphere) const
{
	return Intersection::AabbSphere(m_Min, m_Max, sphere.m_Center, sphere.m_Radius);
}

bool Aabb::Contains(const Aabb& aabb) const
{
	return m_Min.x < aabb.m_Min.x && m_Min.y < aabb.m_Min.y && m_Min.z < aabb.m_Min.z &&
		   m_Max.x > aabb.m_Max.x && m_Max.y > aabb.m_Max.y && m_Max.z > aabb.m_Max.z;
}

void Aabb::Expand(const Vector3& point)
{
	m_Min = PhysicsUtils::Min(m_Min, point);
	m_Max = PhysicsUtils::Max(m_Max, point);
}

Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
{
	Aabb result;
	
	result.m_Min = PhysicsUtils::Min(lhs.m_Min, rhs.m_Min);
	result.m_Max = PhysicsUtils::Max(lhs.m_Max, rhs.m_Max);
	
	return result;
}

void Aabb::Transform(const Matrix& transform)
{
	// getting |M|, where M is rotation portion of the transformation
	Matrix m(transform.Right(), transform.Up(), transform.Backward());
	DirectX::XMFLOAT4X4& mXMFLOAT = m;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mXMFLOAT.m[i][j] = std::abs(mXMFLOAT.m[i][j]);
		}
	}
	const Matrix absM(mXMFLOAT);

	const Vector3 translate(transform.Translation());

	const Vector3 halfExtents = GetHalfExtent();
	const Vector3 halfExtentTransformed = Vector3::Transform(halfExtents, absM);

	const Vector3 center = translate + Vector3::Transform(GetCenter(), m);

	const Aabb aabb = BuildFromCenterAndHalfExtents(center, halfExtentTransformed);

	m_Min = aabb.m_Min;
	m_Max = aabb.m_Max;
}

Vector3 Aabb::GetMin() const
{
	return m_Min;
}

Vector3 Aabb::GetMax() const
{
	return m_Max;
}

Vector3 Aabb::GetCenter() const
{
	return (m_Min + m_Max) * 0.5f;
}

Vector3 Aabb::GetHalfExtent() const
{
	return (m_Max - m_Min) * 0.5f;
}
#ifdef DEVELOPER

void Aabb::DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const
{
	DebugAABBInstance aabbInstance = {};
	aabbInstance.m_min_bound = m_Min;
	aabbInstance.m_max_bound = m_Max;
	aabbInstance.m_color = MathUtil::v4_to_v3(color);
	pAppRenderer->GetDebugRendering().RegisterDebugAABB(aabbInstance);
}

void Aabb::Print() const
{
	DEBUG_LOG("Min = ");
	DEBUG_LOGVec3(m_Min);

	DEBUG_LOG("Max = ");
	DEBUG_LOGVec3(m_Max);
}
#endif
