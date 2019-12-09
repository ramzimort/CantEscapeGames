/**
 * @file ObbSupportShape.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Obb support shape 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

#include "SupportShape.h"

/**
 * @brief Obb support shape
 * 
 */
class ObbSupportShape : public SupportShape
{
public:
	ObbSupportShape();
	ObbSupportShape(const Vector3& position, const Vector3& scale, const Matrix& rotation, const Aabb& localAabb);
	/**
	 * @brief Get the Center of the support shape
	 * 
	 * @return Vector3 
	 */
	Vector3 GetCenter() const override;
	/**
	 * @brief Gets furthest away point in the Obb in the given dirrection
	 * 
	 * @param worldDirection 
	 * @return Vector3 
	 */
	Vector3 Support(const Vector3& worldDirection) const override;
	
	Vector3 m_translation;
	Vector3 m_scale;
	Vector3 m_scaleLocal;
	Matrix m_rotation;
};
