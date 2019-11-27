/**
 * @file Intersection.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Functions to test Intersections between simple shapes
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
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
	/**
	 * @brief Intersection test between a point and a plane
	 * 
	 * @param point 
	 * @param plane 
	 * @param epsilon 
	 * @return IntersectionType 
	 */
	IntersectionType PointPlane(const Vector3& point, const Vector4& plane, float epsilon);
	/**
	 * @brief Intersection test between a point and a sphere
	 * 
	 * @param point 
	 * @param sphereCenter 
	 * @param sphereRadius 
	 * @return true if intersects
	 * @return false otherwise
	 */
	bool PointSphere(const Vector3& point, const Vector3& sphereCenter, float sphereRadius);
	/**
	 * @brief Intersection test between a point and an aabb
	 * 
	 * @param point 
	 * @param aabbMin 
	 * @param aabbMax 
	 * @return true 
	 * @return false 
	 */
	bool PointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax);
	/**
	 * @brief Intersection test between an aabb and an aabb
	 * 
	 * @param aabbMinA 
	 * @param aabbMaxA 
	 * @param aabbMinB 
	 * @param aabbMaxB 
	 * @return true 
	 * @return false 
	 */
	bool AabbAabb(const Vector3& aabbMinA, const Vector3& aabbMaxA, const Vector3& aabbMinB, const Vector3& aabbMaxB);
	/**
	 * @brief Intersection test between a sphere and a sphere
	 * 
	 * @param sphereCenterA 
	 * @param sphereRadiusA 
	 * @param sphereCenterB 
	 * @param sphereRadiusB 
	 * @return true 
	 * @return false 
	 */
	bool SphereSphere(const Vector3& sphereCenterA, float sphereRadiusA, const Vector3& sphereCenterB, float sphereRadiusB);
	/**
	 * @brief Intersection test between an aabb and a sphere
	 * 
	 * @param aabbMin 
	 * @param aabbMax 
	 * @param sphereCenter 
	 * @param sphereRadius 
	 * @return true 
	 * @return false 
	 */
	bool AabbSphere(const Vector3& aabbMin, const Vector3& aabbMax, const Vector3& sphereCenter, float sphereRadius);
	/**
	 * @brief Intersection test between a ray and a sphere
	 * 
	 * @param rayStart 
	 * @param rayDir 
	 * @param sphereCenter 
	 * @param sphereRadius 
	 * @param t 
	 * @return true 
	 * @return false 
	 */
	bool RaySphere(const Vector3& rayStart, const Vector3& rayDir, const Vector3& sphereCenter, float sphereRadius, float& t);
	/**
	 * @brief Intersection test between a ray and an aabb
	 * 
	 * @param rayStart 
	 * @param rayDir 
	 * @param aabbMin 
	 * @param aabbMax 
	 * @param t 
	 * @param parallelCheckEpsilon 
	 * @return true 
	 * @return false 
	 */
	bool RayAabb(const Vector3& rayStart, const Vector3& rayDir, const Vector3& aabbMin, const Vector3& aabbMax, float& t, float parallelCheckEpsilon = 0.0001f);
	/**
	 * @brief Computes square distance between point and aabb
	 * 
	 * @param point 
	 * @param aabbMin 
	 * @param aabbMax 
	 * @return float 
	 */
	float SqDistPointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax);
};
