/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once
#include "Graphics/Models/Model.h"

class SupportShape
{
public:
	virtual ~SupportShape() {};
	virtual Vector3 GetCenter() const = 0;
	virtual Vector3 Support(const Vector3& worldDirection) const = 0;
	
	Vector3 GetCenter(const std::vector<VertexData>& localPoints, const Matrix& localToWorldTransform) const;
	Vector3 Support(const Vector3& worldDirection, const std::vector<VertexData>& localPoints, const Matrix& localToWorldTransform) const;


};
