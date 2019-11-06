/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "SupportShape.h"
#include "Graphics/Models/Model.h"
#include "Components/TransformComponent.h"


class ModelSupportShape : public SupportShape
{
public:
	ModelSupportShape(const Model& model, const TransformComponent& transform);

	Vector3 GetCenter() const override;
	Vector3 Support(const Vector3& worldDirection) const override;

	const Model& m_model;
	const TransformComponent& m_transform;
};
