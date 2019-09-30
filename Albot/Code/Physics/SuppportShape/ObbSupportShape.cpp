/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "ObbSupportShape.h"

Vector3 ObbSupportShape::GetCenter() const 
{
	return m_translation;
}

Vector3 ObbSupportShape::Support(const Vector3& worldDirection) const
{
	// A unit obb spans from [-0.5, to 0.5]
	Vector3 result = m_translation;
	Vector3 localDir = Vector3::Transform(worldDirection, m_rotation.Invert());
	
	result += MathUtil::GetSign(localDir.x) * m_scale.x * 0.5f * m_rotation.Right();
	result += MathUtil::GetSign(localDir.y) * m_scale.y * 0.5f * m_rotation.Up();
	result += MathUtil::GetSign(localDir.z) * m_scale.z * 0.5f * m_rotation.Backward();
	
	return result;
}
