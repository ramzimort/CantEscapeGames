/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "Shape.h"
#include "Sphere.h"

class Aabb : public Shape
{
public:
	Aabb();
	Aabb(const DirectX::SimpleMath::Vector3& min, const DirectX::SimpleMath::Vector3& max);

	static Aabb BuildFromCenterAndHalfExtents(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& halfExtents);
	static Aabb BuildFromMinMax(const DirectX::SimpleMath::Vector3& min, const DirectX::SimpleMath::Vector3& max);

	// Computes the volume of this aabb.
	float GetVolume() const;
	// Computes the surface area of this aabb.
	float GetSurfaceArea() const;

	// compute this aabb for the given points
	void Compute(const std::vector<DirectX::SimpleMath::Vector3>& points);

	// Shape intersect interface
	virtual bool IsIntersect(const Aabb& aabb) const override;
	virtual bool IsIntersect(const Sphere& sphere) const override;

	// Does this aabb completely contain the given aabb (not an intersection test).
	bool Contains(const Aabb& aabb) const;
	// Expand this aabb to include the given point.
	void Expand(const DirectX::SimpleMath::Vector3& point);
	// Combine the two aabbs into a new one
	static Aabb Combine(const Aabb& lhs, const Aabb& rhs);
	/// Compute aabb that would contain the obb of this aabb after transforming it.
	void Transform(const DirectX::SimpleMath::Matrix& transform);

	DirectX::SimpleMath::Vector3 GetMin() const;
	DirectX::SimpleMath::Vector3 GetMax() const;
	DirectX::SimpleMath::Vector3 GetCenter() const;
	DirectX::SimpleMath::Vector3 GetHalfExtent() const;

#ifdef DEVELOPER
	void DebugDraw(const DirectX::SimpleMath::Vector4& color) const override;

	void Print() const;
#endif

	DirectX::SimpleMath::Vector3 mMin;
	DirectX::SimpleMath::Vector3 mMax;
};
