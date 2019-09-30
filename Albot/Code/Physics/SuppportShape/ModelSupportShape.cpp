/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "ModelSupportShape.h"

Vector3 ModelSupportShape::GetCenter() const
{
	
	return SupportShape::GetCenter(m_model->GetVerticesList(), m_modelToWorld);
}

Vector3 ModelSupportShape::Support(const Vector3& worldDirection) const
{
	return SupportShape::Support(worldDirection, m_model->GetVerticesList(), m_modelToWorld);
}

