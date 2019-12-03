/**
 * @file SupportShape.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Base class for any support shape
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */


#pragma once
#include "Graphics/Models/Model.h"

/**
 * @brief Base class for any shape
 * 
 */
class SupportShape
{
public:
	virtual ~SupportShape() {};
	/**
	 * @brief Every suport shape needs to return its center
	 * 
	 * @return Vector3 
	 */
	virtual Vector3 GetCenter() const = 0;
	/**
	 * @brief Every support shape need to be able to calculate furthest point away in the given direction in that shape
	 * 
	 * @param worldDirection 
	 * @return Vector3 
	 */
	virtual Vector3 Support(const Vector3& worldDirection) const = 0;
	
	/**
	 * @brief give a collection of points in local space, GetCenter will calculate their center and transform it to world space
	 * 
	 * @param localPoints 
	 * @param localToWorldTransform 
	 * @return Vector3 
	 */
	Vector3 GetCenter(const std::vector<VertexData>& localPoints, const Matrix& localToWorldTransform) const;
	/**
	 * @brief Calculatess furthest point in the direction in the local space and transfoorms it to world space
	 * 
	 * @param worldDirection 
	 * @param localPoints 
	 * @param localToWorldTransform 
	 * @return Vector3 
	 */
	Vector3 Support(const Vector3& worldDirection, const std::vector<VertexData>& localPoints, const Matrix& localToWorldTransform) const;


};
