/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "Intersection.h"
#include "../PhysicsUtils.h"

namespace Intersection
{
	IntersectionType PointPlane(const Vector3& point, const Vector4& plane, float epsilon)
	{
		// Dot(n, p) - d = distance   or   n.x * p.x + n.y * p.y + n.z * p.z + d * (-1) = distance
		const Vector4 point_v4(point.x, point.y, point.z, -1);
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

	bool PointSphere(const Vector3& point, const Vector3& sphereCenter, float sphereRadius)
	{
		const Vector3 v = sphereCenter - point;
		return v.LengthSquared() <= sphereRadius * sphereRadius;
	}

	bool PointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax)
	{
		return !(point.x < aabbMin.x || point.x > aabbMax.x ||
			point.y < aabbMin.y || point.y > aabbMax.y ||
			point.z < aabbMin.z || point.z > aabbMax.z);
	}

	bool AabbAabb(const Vector3& aabbMinA, const Vector3& aabbMaxA, const Vector3& aabbMinB, const Vector3& aabbMaxB)
	{
		return !(aabbMaxB.x < aabbMinA.x || aabbMaxA.x < aabbMinB.x ||
			aabbMaxB.y < aabbMinA.y || aabbMaxA.y < aabbMinB.y ||
			aabbMaxB.z < aabbMinA.z || aabbMaxA.z < aabbMinB.z);
	}

	bool SphereSphere(const Vector3& sphereCenterA, float sphereRadiusA, const Vector3& sphereCenterB, float sphereRadiusB)
	{
		return PointSphere(sphereCenterA, sphereCenterB, sphereRadiusA + sphereRadiusB);
	}

	bool AabbSphere(const Vector3& aabbMin, const Vector3& aabbMax, const Vector3& sphereCenter, float sphereRadius)
	{
		return SqDistPointAabb(sphereCenter, aabbMin, aabbMax) <= sphereRadius * sphereRadius;
	}

	float SqDistPointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax)
	{
		float sqDist = 0.0f;

		const DirectX::XMVECTOR& p = point;
		const DirectX::XMVECTOR& min = aabbMin;
		const DirectX::XMVECTOR& max = aabbMax;

		for (int i = 0; i < 3; ++i)
		{
			if (p.m128_f32[i] < min.m128_f32[i]) sqDist += PhysicsUtils::Sqr(min.m128_f32[i] - p.m128_f32[i]);
			if (p.m128_f32[i] > max.m128_f32[i]) sqDist += PhysicsUtils::Sqr(p.m128_f32[i] - max.m128_f32[i]);
		}

		return sqDist;
	}
}
