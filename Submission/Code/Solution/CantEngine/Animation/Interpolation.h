/**
 * @file Interpolation.h
 * @author Jose Rosenbluth
 * @brief Contains Math interpolation functions
 * @date 2019-11-21
 */

#pragma once

#include "Quaternion.h"


namespace MathUtil
{
	/**
	 * @brief Linear interpolation between two vectors
	 * 
	 * @param origin 
	 * @param destination 
	 * @param alpha 
	 * @return Vector3 
	 */
	Vector3 Lerp(Vector3 const& origin, Vector3 const& destination, float alpha);

	/**
	 * @brief Spherical linear interpolation between two quaternions (orientation)
	 * 
	 * @param origin 
	 * @param destination 
	 * @param alpha 
	 * @return MathUtil::Quaternion 
	 */
	MathUtil::Quaternion Slerp(MathUtil::Quaternion const& origin, MathUtil::Quaternion const& destination, float alpha);

	/**
	 * @brief Generates a VQS matrix based on a position s, orientation q, and scale v
	 * 
	 * @param v Represents scale
	 * @param q Represents orientation
	 * @param s Represents position
	 * @return Matrix 
	 */
	Matrix GenVQSMatrix(Vector3 const& v, MathUtil::Quaternion const& q, Vector3 const& s);
}
