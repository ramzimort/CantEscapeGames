/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "Shape.h"
#include "Aabb.h"

///////////// PCA Helpers ////////////////////////////
//DirectX::SimpleMath::Matrix ComputeCovarianceMatrix(const std::vector<DirectX::SimpleMath::Vector3>& points);
//DirectX::SimpleMath::Matrix ComputeJacobiRotation(const DirectX::SimpleMath::Matrix& matrix);
//void ComputeEigenValuesAndVectors(const DirectX::SimpleMath::Matrix& covariance, DirectX::SimpleMath::Vector3& eigenValues, DirectX::SimpleMath::Matrix& eigenVectors, int maxIterations = 50);

//////////////////////////////////////// Sphere ////////////////////////////////////////
class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const DirectX::SimpleMath::Vector3& center, float radius);

	void ComputeCentroid(const std::vector<DirectX::SimpleMath::Vector3>& points);
	void ComputeRitter(const std::vector<DirectX::SimpleMath::Vector3>& points);
	void ComputePCA(const std::vector<DirectX::SimpleMath::Vector3>& points, int maxIterations = 50);

	// Shape intersect interface
	virtual bool IsIntersect(const Aabb& aabb) const override;
	virtual bool IsIntersect(const Sphere& sphere) const override;

	// Does the sphere contain the given point?
	bool ContainsPoint(const DirectX::SimpleMath::Vector3& point);

	DirectX::SimpleMath::Vector3 GetCenter() const;
	float GetRadius() const;

	// See if this aabb is equal to another (with epsilon). Used for unit testing.
	bool Compare(const Sphere& rhs, float epsilon) const;

#ifdef DEVELOPER
	void DebugDraw(const DirectX::SimpleMath::Vector4& color) const override;

	void Print() const;
#endif
	DirectX::SimpleMath::Vector3 mCenter;
	float mRadius;
};