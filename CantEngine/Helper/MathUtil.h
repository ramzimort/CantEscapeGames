#pragma once
#include "Physics/PhysicsUtils.h"

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

	inline float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}
	inline float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	inline void  BuildTangentBitangent(const Vector3& vector, Vector3& tangent, Vector3& bitangent)
	{
		const Vector3 cross1 = vector.Cross(Vector3(0.0, 0.0, 1.0));
		const Vector3 cross2 = vector.Cross(Vector3(0.0, 1.0, 0.0));

		if (cross1.LengthSquared() > cross2.LengthSquared())
		{
			tangent = cross1;
		}
		else
		{
			tangent = cross2;
		}
		tangent.Normalize();

		bitangent = vector.Cross(tangent);
		bitangent.Normalize();
	}

	inline float Clamp(float input, float min, float max)
	{
		return PhysicsUtils::Min(PhysicsUtils::Max(input, min), max);
	}

	inline float PositiveMax()
	{
		return FLT_MAX;
	}

	inline float NegativeMin()
	{
		return -FLT_MAX;
	}

	inline Vector3 ToEulerAngles(Quaternion q)
	{
		Vector3 angles;

		// roll (x-axis rotation)
		double sinr_cosp = +2.0 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
		angles.x = static_cast<float>(atan2(sinr_cosp, cosr_cosp));

		// pitch (y-axis rotation)
		double sinp = +2.0 * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			angles.y = static_cast<float>(copysign(M_PI / 2, sinp)); // use 90 degrees if out of range
		else
			angles.y = static_cast<float>(asin(sinp));

		// yaw (z-axis rotation)
		double siny_cosp = +2.0 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
		angles.z = static_cast<float>(atan2(siny_cosp, cosy_cosp));

		return angles;
	}

	inline Matrix GetRotationMatrix(const Vector3& right, const Vector2& rotation)
	{
		DirectX::XMVECTOR rotation_vector = XMLoadFloat2(&rotation);
		DirectX::XMMATRIX pitch_mat = XMMatrixRotationAxis(right, DirectX::XMVectorGetY(rotation_vector));
		DirectX::XMMATRIX yaw_mat = DirectX::XMMatrixRotationY(DirectX::XMVectorGetX(rotation_vector));
		return DirectX::XMMatrixMultiply(pitch_mat, yaw_mat);
	}
}
