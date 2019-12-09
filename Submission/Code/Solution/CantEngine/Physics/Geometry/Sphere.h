/**
 * @file Sphere.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of the Sphere shape
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

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

	//void ComputeCentroid(const std::vector<Vector3>& points);
	//void ComputeRitter(const std::vector<Vector3>& points);
	//void ComputePCA(const std::vector<Vector3>& points, int maxIterations = 50);

	/**
	 * @brief Interseection test with an aabb
	 * 
	 * @param aabb 
	 * @return true 
	 * @return false 
	 */
	virtual bool IsIntersect(const Aabb& aabb) const override;
	/**
	 * @brief Intersection test with a sphere
	 * 
	 * @param sphere 
	 * @return true 
	 * @return false 
	 */
	virtual bool IsIntersect(const Sphere& sphere) const override;
	/**
	 * @brief checks if the given point is inside the sphere
	 * 
	 * @param point 
	 * @return true 
	 * @return false 
	 */
	bool ContainsPoint(const Vector3& point);

	/**
	 * @brief Get the Center of the sphere
	 * 
	 * @return Vector3 
	 */
	Vector3 GetCenter() const;
	/**
	 * @brief Get the Radius of the sphere
	 * 
	 * @return float 
	 */
	float GetRadius() const;

	/**
	 * @brief compare if spheres are close enough to be same
	 * 
	 * @param rhs 
	 * @param epsilon 
	 * @return true 
	 * @return false 
	 */
	bool Compare(const Sphere& rhs, float epsilon) const;

#ifdef DEVELOPER
	void DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const override;

	void Print() const;
#endif
	Vector3 m_Center;
	float m_Radius;
};