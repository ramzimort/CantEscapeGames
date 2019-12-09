/**
 * @file ModelSupportShape.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Support shape for a model
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
#pragma once

#include "SupportShape.h"
#include "Graphics/Models/Model.h"
#include "Components/TransformComponent.h"

/**
 * @brief Support shape for a model
 * 
 */
class ModelSupportShape : public SupportShape
{
public:
	ModelSupportShape(const Model& model, const TransformComponent& transform);

	/**
	 * @brief Get the Center of the support shape
	 * 
	 * @return Vector3 
	 */
	Vector3 GetCenter() const override;
	/**
	 * @brief Gets the furthest point in the direction in the support shape
	 * 
	 * @param worldDirection 
	 * @return Vector3 
	 */
	Vector3 Support(const Vector3& worldDirection) const override;

	const Model& m_model;
	const TransformComponent& m_transform;
};
