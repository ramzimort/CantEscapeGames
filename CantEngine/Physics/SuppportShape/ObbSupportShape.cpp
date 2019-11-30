/**
 * @file ObbSupportShape.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#include "ObbSupportShape.h"

ObbSupportShape::ObbSupportShape()
{
}

ObbSupportShape::ObbSupportShape(const Vector3& position, const Vector3& scale, const Matrix& rotation, const Aabb& localAabb) : m_translation(position),  m_scale(scale), m_rotation(rotation)
{
	m_scaleLocal = localAabb.GetMax() - localAabb.GetMin();
}

Vector3 ObbSupportShape::GetCenter() const 
{
	return m_translation;
}

Vector3 ObbSupportShape::Support(const Vector3& worldDirection) const
{
	Vector3 result = m_translation;
	Vector3 localDir = Vector3::Transform(worldDirection, m_rotation.Invert());
	
	result += MathUtil::GetSign(localDir.x) * m_scale.x * 0.5f * m_rotation.Right();
	result += MathUtil::GetSign(localDir.y) * m_scale.y * 0.5f * m_rotation.Up();
	result += MathUtil::GetSign(localDir.z) * m_scale.z * 0.5f * m_rotation.Backward();
	
	return result;
}
