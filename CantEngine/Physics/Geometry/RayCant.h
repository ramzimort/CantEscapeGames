/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

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

	// Transforms this ray to another space by the given matrix 4
	RayCant Transform(const Matrix& transform) const;
	// Returns the a point at the given t value.
	Vector3 GetPoint(float t) const;

#ifdef DEVELOPER
	// Draws this ray up to a given t value.
	void DebugDraw(AppRenderer* pApprenderer, float t, const Vector4& color) const;
#endif

	Vector3 m_start;
	Vector3 m_direction;
};
