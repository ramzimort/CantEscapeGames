/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "Aabb.h"
#include "Intersection.h"
#include "../PhysicsUtils.h"

Aabb::Aabb() : mMin(FLT_MAX), mMax(-FLT_MAX) //set the aabb to an initial bad value (where the min is smaller than the max)
{
}

Aabb::Aabb(const DirectX::SimpleMath::Vector3& min, const DirectX::SimpleMath::Vector3& max) : mMin(min), mMax(max)
{
}

Aabb Aabb::BuildFromCenterAndHalfExtents(const DirectX::SimpleMath::Vector3& center, const DirectX::SimpleMath::Vector3& halfExtents)
{
	return Aabb(center - halfExtents, center + halfExtents); 
}

Aabb Aabb::BuildFromMinMax(const DirectX::SimpleMath::Vector3& min, const DirectX::SimpleMath::Vector3& max)
{
	return Aabb(min, max);
}

float Aabb::GetVolume() const
{
	return abs((mMax.x - mMin.x) * (mMax.y - mMin.y) * (mMax.z - mMin.z));
}

float Aabb::GetSurfaceArea() const
{
	// Return the aabb's surface area
	return 2.0f * (abs((mMax.x - mMin.x) * (mMax.y - mMin.y)) +
		abs((mMax.x - mMin.x) * (mMax.z - mMin.z)) +
		abs((mMax.y - mMin.y) * (mMax.z - mMin.z)));
}

void Aabb::Compute(const std::vector<DirectX::SimpleMath::Vector3>& points)
{
	mMin = DirectX::SimpleMath::Vector3(FLT_MAX);
	mMax = DirectX::SimpleMath::Vector3(-FLT_MAX);

	for (const DirectX::SimpleMath::Vector3& point : points)
	{
		mMin = PhysicsUtils::Min(mMin, point);
		mMax = PhysicsUtils::Max(mMax, point);
	}
}

bool Aabb::IsIntersect(const Aabb& aabb) const
{
	return Intersection::AabbAabb(mMin, mMax, aabb.mMin, aabb.mMax);
}

bool Aabb::IsIntersect(const Sphere& sphere) const
{
	return Intersection::AabbSphere(mMin, mMax, sphere.mCenter, sphere.mRadius);
}

bool Aabb::Contains(const Aabb& aabb) const
{
	// Return if aabb is completely contained in this
	return mMin.x < aabb.mMin.x && mMin.y < aabb.mMin.y && mMin.z < aabb.mMin.z &&
		mMax.x > aabb.mMax.x && mMax.y > aabb.mMax.y && mMax.z > aabb.mMax.z;
}

void Aabb::Expand(const DirectX::SimpleMath::Vector3& point)
{
	mMin = PhysicsUtils::Min(mMin, point);
	mMax = PhysicsUtils::Max(mMax, point);
}

Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
{
	Aabb result;
	result.mMin = PhysicsUtils::Min(lhs.mMin, rhs.mMin);
	result.mMax = PhysicsUtils::Max(lhs.mMax, rhs.mMax);
	return result;
}

void Aabb::Transform(const DirectX::SimpleMath::Matrix& transform)
{
	// Compute aabb of the this aabb after it is transformed.
	using namespace DirectX::SimpleMath;
	Matrix m(transform.Right(), transform.Up(), transform.Backward());

	// getting |M|, where M is rotation portion of the transformation
	DirectX::XMFLOAT4X4& mXMFLOAT = m;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mXMFLOAT.m[i][j] = std::abs(mXMFLOAT.m[i][j]);
		}
	}
	const Matrix absM(mXMFLOAT);

	// translation portion of the transformation
	const Vector3 translate(transform.Translation());

	// transforming half extent of the aabb (half of the diagonal)
	const Vector3 halfExtents = GetHalfExtent();
	const Vector3 halfExtentTransformed = Vector3::Transform(halfExtents, absM);

	// transforming center of the aabb
	const DirectX::SimpleMath::Vector3 center = translate + Vector3::Transform(GetCenter(), m);

	// buuilding transformed aabb
	const Aabb aabb = BuildFromCenterAndHalfExtents(center, halfExtentTransformed);

	mMin = aabb.mMin;
	mMax = aabb.mMax;
}

DirectX::SimpleMath::Vector3 Aabb::GetMin() const
{
	return mMin;
}

DirectX::SimpleMath::Vector3 Aabb::GetMax() const
{
	return mMax;
}

DirectX::SimpleMath::Vector3 Aabb::GetCenter() const
{
	return (mMin + mMax) * 0.5f;
}

DirectX::SimpleMath::Vector3 Aabb::GetHalfExtent() const
{
	return (mMax - mMin) * 0.5f;
}
#ifdef DEVELOPER

void Aabb::DebugDraw(const DirectX::SimpleMath::Vector4& /*color*/) const
{
	
}

void Aabb::Print() const
{
	std::cout << "min = ";
	std::cout << "[" << mMin.x << ", " << mMin.y << ", " << mMin.z << "]" << std::endl;
	std::cout << "max = ";
	std::cout << "[" << mMax.x << ", " << mMax.y << ", " << mMax.z << "]" << std::endl;
}
#endif
