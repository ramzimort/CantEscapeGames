/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

#include "RayCant.h"
#include "Intersection.h"

RayCant::RayCant()
{
}

RayCant::RayCant(const Vector3& start, const Vector3& dir) : m_start(start), m_direction(dir)
{
}

RayCant RayCant::Transform(const Matrix& transform) const
{
	RayCant transformedRay;
	transformedRay.m_start = Vector3::Transform(m_start, transform);// TransformPoint(transform, mStart);
	transformedRay.m_direction = Vector3::Transform(m_direction, transform);// ::TransformDirection(transform, m_direction);
	return transformedRay;
}

Vector3 RayCant::GetPoint(float t) const
{
	return m_start + m_direction * t;
}

#ifdef DEVELOPER

void RayCant::DebugDraw(AppRenderer* pApprenderer, float t, const Vector4& color) const
{
	pApprenderer->GetDebugRendering().RegisterDebugLineInstance(m_start, m_start + t * m_direction, MathUtil::v4_to_v3(color));
}
#endif

