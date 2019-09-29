/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

namespace Intersection
{
	enum IntersectionType
	{
		Coplanar = 0,
		Outside	 = 1,
		Inside   = 2,
		Overlaps = 4
	};

	IntersectionType PointPlane(const Vector3& point, const Vector4& plane, float epsilon);
	bool PointSphere(const Vector3& point, const Vector3& sphereCenter, float sphereRadius);
	bool PointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax);
	bool AabbAabb(const Vector3& aabbMinA, const Vector3& aabbMaxA, const Vector3& aabbMinB, const Vector3& aabbMaxB);
	bool SphereSphere(const Vector3& sphereCenterA, float sphereRadiusA, const Vector3& sphereCenterB, float sphereRadiusB);
	bool AabbSphere(const Vector3& aabbMin, const Vector3& aabbMax, const Vector3& sphereCenter, float sphereRadius);

	float SqDistPointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax);

};
