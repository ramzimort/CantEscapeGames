#pragma once

namespace MathUtil
{
	inline Vector4 v3_to_v4(const Vector3& v3, float w = 1.0)
	{
		return Vector4(v3.x, v3.y, v3.z, 1.0f);
	}

	inline Vector3 v4_to_v3(const Vector4& v4)
	{
		return Vector3(v4.x, v4.y, v4.z);
	}
	
	inline float GetSign(float val)
	{
		return (val < 0.0f) ? -1.0f : 1.0f;
	}
}