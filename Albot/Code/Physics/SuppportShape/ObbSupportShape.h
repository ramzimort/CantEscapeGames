/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "SupportShape.h"

class ObbSupportShape : public SupportShape
{
public:

	Vector3 GetCenter() const override;
	Vector3 Support(const Vector3& worldDirection) const override;
	
	Vector3 m_scale;
	Matrix m_rotation;
	Vector3 m_translation;
};