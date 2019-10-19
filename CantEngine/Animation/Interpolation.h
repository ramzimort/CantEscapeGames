#pragma once

#include "Quaternion.h"


namespace MathUtil
{
	Vector3 Lerp(Vector3 const& origin, Vector3 const& destination, float alpha);

	MathUtil::Quaternion Slerp(MathUtil::Quaternion const& origin, MathUtil::Quaternion const& destination, float alpha);

	Matrix GenVQSMatrix(Vector3 const& v, MathUtil::Quaternion const& q, Vector3 const& s);
}
