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
//Matrix ComputeCovarianceMatrix(const std::vector<Vector3>& points);
//Matrix ComputeJacobiRotation(const Matrix& matrix);
//void ComputeEigenValuesAndVectors(const Matrix& covariance, Vector3& eigenValues, Matrix& eigenVectors, int maxIterations = 50);

//////////////////////////////////////// Sphere ////////////////////////////////////////
class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const Vector3& center, float radius);

	void ComputeCentroid(const std::vector<Vector3>& points);
	void ComputeRitter(const std::vector<Vector3>& points);
	void ComputePCA(const std::vector<Vector3>& points, int maxIterations = 50);

	virtual bool IsIntersect(const Aabb& aabb) const override;
	virtual bool IsIntersect(const Sphere& sphere) const override;

	bool ContainsPoint(const Vector3& point);

	Vector3 GetCenter() const;
	float GetRadius() const;

	bool Compare(const Sphere& rhs, float epsilon) const;

#ifdef DEVELOPER
	void DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const override;

	void Print() const;
#endif
	Vector3 m_Center;
	float m_Radius;
};