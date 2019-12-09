/**
 * @file Aabb.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of axis alligned bounding box
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

#include "Shape.h"
#include "Sphere.h"

/**
 * @brief Axis alligned bounding box, represented by aposite corners min and max
 * 
 */
class Aabb : public Shape
{
public:
	Aabb();
	Aabb(const Vector3& min, const Vector3& max);

	/**
	 * @brief Builds Aabb from the coordinates of the center of the aabb and the half extent vector to one of the corners
	 * 
	 * @param center 
	 * @param halfExtents 
	 * @return Aabb 
	 */
	static Aabb BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents);
	/**
	 * @brief Builds Aabb from given aposite corners min and max
	 * 
	 * @param min 
	 * @param max 
	 * @return Aabb 
	 */
	static Aabb BuildFromMinMax(const Vector3& min, const Vector3& max);
	/**
	 * @brief Builds Aabb around transformed aabb
	 * 
	 * @param modelMatrix 
	 * @param localAABB 
	 * @return Aabb 
	 */
	static Aabb BuildFromLocalAABBAndModelMatrix(const Matrix& modelMatrix, const Aabb& localAABB);
	/**
	 * @brief Computes Volume of the Aabb
	 * 
	 * @return float 
	 */
	float GetVolume() const;
	/**
	 * @brief Computes Surface Area of the Aabb
	 * 
	 * @return float 
	 */
	float GetSurfaceArea() const;

	/**
	 * @brief Initializes this Aabb from constructing an Aabb that contains all the given points
	 * 
	 * @param points 
	 */
	void Compute(const std::vector<Vector3>& points);

	/**
	 * @brief check for collision with another Aabb
	 * 
	 * @param aabb 
	 * @return true if intersects
	 * @return false if not intersects
	 */
	virtual bool IsIntersect(const Aabb& aabb) const override;
	/**
	 * @brief Check for collision with a sphere
	 * 
	 * @param sphere 
	 * @return true if intersects
	 * @return false if not intersects
	 */
	virtual bool IsIntersect(const Sphere& sphere) const override;
	/**
	 * @brief Check if given Aabb fully inside of this Aabb
	 * 
	 * @param aabb 
	 * @return true if this aabb contains given aabb
	 * @return false otherwise
	 */
	bool Contains(const Aabb& aabb) const;
	/**
	 * @brief Expands this Aabb to contain also given point
	 * 
	 * @param point 
	 */
	void Expand(const Vector3& point);
	/**
	 * @brief Builds an Aabb that contains both of the provided Aabbs
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return Aabb 
	 */
	static Aabb Combine(const Aabb& lhs, const Aabb& rhs);
	/**
	 * @brief Compute aabb that would contain the obb of this aabb after transforming it.
	 * 
	 * @param transform 
	 */
	void Transform(const Matrix& transform);
	/**
	 * @brief Get the Min corner of the Aabb
	 * 
	 * @return Vector3 
	 */
	Vector3 GetMin() const;
	/**
	 * @brief Get the Max corner of the Aabb
	 * 
	 * @return Vector3 
	 */
	Vector3 GetMax() const;
	/**
	 * @brief Get the Center of the Aabb
	 * 
	 * @return Vector3 
	 */
	Vector3 GetCenter() const;
	/**
	 * @brief Get the Half Extent of the Aabb
	 * 
	 * @return Vector3 
	 */
	Vector3 GetHalfExtent() const;

#ifdef DEVELOPER
	void DebugDraw(AppRenderer* pAppRenderer, const Vector4& color) const override;

	void Print() const;
#endif

	Vector3 m_Min;
	Vector3 m_Max;
};
