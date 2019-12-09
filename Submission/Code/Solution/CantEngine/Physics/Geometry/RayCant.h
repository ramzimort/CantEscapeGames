/**
 * @file RayCant.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of the ray 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

#ifdef DEVELOPER
#include "Graphics/AppRenderer.h"
#endif
#include "Shape.h"
#include "Aabb.h"
#include "Sphere.h"

class RayCant
{
public:
	RayCant();
	RayCant(const Vector3& start, const Vector3& dir);

	/**
	 * @brief Transforms this ray to another space by the given matrix
	 * 
	 * @param transform 
	 * @return RayCant 
	 */
	RayCant Transform(const Matrix& transform) const;
	/**
	 * @brief Calculates a point at a given time t
	 * 
	 * @param t 
	 * @return Vector3 
	 */
	Vector3 GetPoint(float t) const;

#ifdef DEVELOPER
	// Draws this ray up to a given t value.
	void DebugDraw(AppRenderer* pApprenderer, float t, const Vector4& color) const;
#endif

	Vector3 m_start;
	Vector3 m_direction;
};
