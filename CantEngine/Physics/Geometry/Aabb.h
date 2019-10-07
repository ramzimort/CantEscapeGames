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
	Aabb(const Vector3& min, const Vector3& max);

	static Aabb BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents);
	static Aabb BuildFromMinMax(const Vector3& min, const Vector3& max);
	static Aabb BuildFromLocalAABBAndModelMatrix(const Matrix& modelMatrix, const Aabb& localAABB);

	float GetVolume() const;
	float GetSurfaceArea() const;

	// compute this aabb for the given points
	void Compute(const std::vector<Vector3>& points);

	virtual bool IsIntersect(const Aabb& aabb) const override;
	virtual bool IsIntersect(const Sphere& sphere) const override;

	bool Contains(const Aabb& aabb) const;
	void Expand(const Vector3& point);
	static Aabb Combine(const Aabb& lhs, const Aabb& rhs);
	
	/// Compute aabb that would contain the obb of this aabb after transforming it.
	void Transform(const Matrix& transform);

	Vector3 GetMin() const;
	Vector3 GetMax() const;
	Vector3 GetCenter() const;
	Vector3 GetHalfExtent() const;

#ifdef DEVELOPER
	void DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const override;

	void Print() const;
#endif

	Vector3 m_Min;
	Vector3 m_Max;
};
