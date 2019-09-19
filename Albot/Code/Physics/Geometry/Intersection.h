/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

class Intersection
{
public:
	enum IntersectionType
	{
		Coplanar = 0,
		Outside	 = 1,
		Inside   = 2,
		Overlaps = 4
	};

	static IntersectionType PointPlane(const DirectX::SimpleMath::Vector3& point, const DirectX::SimpleMath::Vector4& plane, float epsilon);

	static bool PointSphere(const DirectX::SimpleMath::Vector3& point, const DirectX::SimpleMath::Vector3& sphereCenter, float sphereRadius);

	static bool PointAabb(const DirectX::SimpleMath::Vector3& point, const DirectX::SimpleMath::Vector3& aabbMin, const DirectX::SimpleMath::Vector3& aabbMax);

	static bool AabbAabb(const DirectX::SimpleMath::Vector3& aabbMinA, const DirectX::SimpleMath::Vector3& aabbMaxA,
						 const DirectX::SimpleMath::Vector3& aabbMinB, const DirectX::SimpleMath::Vector3& aabbMaxB);

	static bool SphereSphere(const DirectX::SimpleMath::Vector3& sphereCenterA, float sphereRadiusA,
							 const DirectX::SimpleMath::Vector3& sphereCenterB, float sphereRadiusB);
	static bool AabbSphere(const DirectX::SimpleMath::Vector3& aabbMin, const DirectX::SimpleMath::Vector3& aabbMax,
						   const DirectX::SimpleMath::Vector3& sphereCenter, float sphereRadius);

	static float SqDistPointAabb(const DirectX::SimpleMath::Vector3& point,
								 const DirectX::SimpleMath::Vector3& aabbMin, const DirectX::SimpleMath::Vector3& aabbMax);

};
