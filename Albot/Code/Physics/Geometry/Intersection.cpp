/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "Intersection.h"
#include "../PhysicsUtils.h"

Intersection::IntersectionType Intersection::PointPlane(const DirectX::SimpleMath::Vector3& point, const DirectX::SimpleMath::Vector4& plane, float epsilon)
{
	// Dot(n, p) - d = distance   or   n.x * p.x + n.y * p.y + n.z * p.z + d * (-1) = distance
	const DirectX::SimpleMath::Vector4 point_v4(point.x, point.y, point.z, -1);
	const float distance = plane.Dot(point_v4);

	if (distance < -epsilon)
	{
		return IntersectionType::Outside;
	}
	if (distance > epsilon)
	{
		return IntersectionType::Inside;
	}
	return IntersectionType::Coplanar;
}

bool Intersection::PointSphere(const DirectX::SimpleMath::Vector3& point,
								const DirectX::SimpleMath::Vector3& sphereCenter, float sphereRadius)
{
	const DirectX::SimpleMath::Vector3 v = sphereCenter - point;
	return v.LengthSquared() <= sphereRadius * sphereRadius;
}

bool Intersection::PointAabb(const DirectX::SimpleMath::Vector3& point, const DirectX::SimpleMath::Vector3& aabbMin,
	const DirectX::SimpleMath::Vector3& aabbMax)
{
	return !(point.x < aabbMin.x || point.x > aabbMax.x ||
			 point.y < aabbMin.y || point.y > aabbMax.y ||
			 point.z < aabbMin.z || point.z > aabbMax.z);
}

bool Intersection::AabbAabb(const DirectX::SimpleMath::Vector3& aabbMinA, const DirectX::SimpleMath::Vector3& aabbMaxA,
	const DirectX::SimpleMath::Vector3& aabbMinB, const DirectX::SimpleMath::Vector3& aabbMaxB)
{
	return !(aabbMaxB.x < aabbMinA.x || aabbMaxA.x < aabbMinB.x ||
			 aabbMaxB.y < aabbMinA.y || aabbMaxA.y < aabbMinB.y ||
			 aabbMaxB.z < aabbMinA.z || aabbMaxA.z < aabbMinB.z);
}

bool Intersection::SphereSphere(const DirectX::SimpleMath::Vector3& sphereCenterA, float sphereRadiusA,
								const DirectX::SimpleMath::Vector3& sphereCenterB, float sphereRadiusB)
{
	return PointSphere(sphereCenterA, sphereCenterB, sphereRadiusA + sphereRadiusB);
}

bool Intersection::AabbSphere(const DirectX::SimpleMath::Vector3& aabbMin, const DirectX::SimpleMath::Vector3& aabbMax,
	const DirectX::SimpleMath::Vector3& sphereCenter, float sphereRadius)
{
	return SqDistPointAabb(sphereCenter, aabbMin, aabbMax) <= sphereRadius * sphereRadius;
}

float Intersection::SqDistPointAabb(const DirectX::SimpleMath::Vector3& point,
	const DirectX::SimpleMath::Vector3& aabbMin, const DirectX::SimpleMath::Vector3& aabbMax)
{
	float sqDist = 0.0f;

	const DirectX::XMVECTOR& p = point;
	const DirectX::XMVECTOR& min = aabbMin;
	const DirectX::XMVECTOR& max = aabbMax;

	for (int i = 0; i < 3; ++i)
	{
		if (p.m128_f32[i] < min.m128_f32[i]) sqDist += PhysicsUtils::Sqr(min.m128_f32[i] - p.m128_f32[i]);
		if (p.m128_f32[i]> max.m128_f32[i]) sqDist += PhysicsUtils::Sqr(p.m128_f32[i] - max.m128_f32[i]);
	}
	
	return sqDist;
}

