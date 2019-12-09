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

	inline Vector3 PiecewiseProd(const Vector3& v0, const Vector3& v1)
	{
		return Vector3(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z);
	}

	inline Vector2 PiecewiseProd(const Vector2& v0, const Vector2& v1)
	{
		return Vector2(v0.x * v1.x, v0.y * v1.y);
	}

	inline Vector4 PiecewiseProd(const Vector4& v0, const Vector4& v1)
	{
		return Vector4(v0.x * v1.x, v0.y * v1.y, v0.z * v1.z, v0.w * v1.w);
	}
	
	inline float GetSign(float val)
	{
		return (val < 0.0f) ? -1.0f : 1.0f;
	}

	inline float SquareLength(const Vector2& vec2)
	{
		return vec2.x * vec2.x + vec2.y * vec2.y;
	}

	inline float Length(const Vector2& vec2)
	{
		return sqrt(SquareLength(vec2));
	}

	inline float SquareLength(const Vector3& vec3)
	{
		return vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z;

	}

	inline float Length(const Vector3& vec3)
	{
		return sqrt(SquareLength(vec3));
	}

	inline float SquareLength(const Vector4& vec4)
	{
		return vec4.x * vec4.x + vec4.y * vec4.y + vec4.z * vec4.z + vec4.w * vec4.w;
	}

	inline float Length(const Vector4& vec4)
	{
		return sqrt(SquareLength(vec4));
	}


	inline float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}
	inline float RandF(float a, float b)
	{
		return a + RandF() * (b - a);
	}

	inline int32_t RandI(int32_t a, int32_t b)
	{
		return a + static_cast<int32_t>(RandF() * (static_cast<float>(b) - static_cast<float>(a)));
	}

	inline void  BuildTangentBitangent(const Vector3& vector, Vector3& tangent, Vector3& bitangent)
	{
		// Catto's approach
		if (vector.x >= 0.57735f)
			tangent = Vector3(vector.y, -vector.x, 0.0f);
		else
			tangent = Vector3(0.0f, vector.z, -vector.y);

		tangent.Normalize();
		bitangent = vector.Cross(tangent);
		bitangent.Normalize();
		/* // another way of doing this
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
		bitangent.Normalize();*/
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
		double sinr_cosp = +2.0f * (q.w * q.x + q.y * q.z);
		double cosr_cosp = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
		angles.x = static_cast<float>(atan2(sinr_cosp, cosr_cosp));

		// pitch (y-axis rotation)
		double sinp = +2.0f * (q.w * q.y - q.z * q.x);
		if (fabs(sinp) >= 1)
			angles.y = static_cast<float>(copysign(M_PI / 2, sinp)); // use 90 degrees if out of range
		else
			angles.y = static_cast<float>(asin(sinp));

		// yaw (z-axis rotation)
		double siny_cosp = +2.0f * (q.w * q.z + q.x * q.y);
		double cosy_cosp = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		angles.z = static_cast<float>(atan2(siny_cosp, cosy_cosp));

		return angles;
	}

	inline unsigned LeastSignificantComponent(const Vector3& vec)
	{
		const float absX = std::abs(vec.x);
		const float absY = std::abs(vec.y);
		const float absZ = std::abs(vec.z);


		float min = absX;
		unsigned minIndex = 0;

		if (absY < min)
		{
			minIndex = 1;
			min = absY;
		}
		if (absZ < min)
		{
			minIndex = 2;
		}

		return minIndex;
	}

	inline Matrix GetRotationMatrix(const Vector3& right, const Vector2& rotation)
	{
		DirectX::XMVECTOR rotation_vector = XMLoadFloat2(&rotation);
		DirectX::XMMATRIX pitch_mat = XMMatrixRotationAxis(right, DirectX::XMVectorGetY(rotation_vector));
		DirectX::XMMATRIX yaw_mat = DirectX::XMMatrixRotationY(DirectX::XMVectorGetX(rotation_vector));
		return DirectX::XMMatrixMultiply(pitch_mat, yaw_mat);
	}


	Vector3 MatrixToRadEulerAngles(const Matrix& matrix);
	Vector3 MatrixToDegreeEulerAngles(const Matrix& matrix);

	inline Matrix CreateRotationMatrixFromDegrees(float pitch, float yaw, float roll)
	{
		float xRad = DirectX::XMConvertToRadians(pitch);
		float yRad = DirectX::XMConvertToRadians(yaw);
		float zRad = DirectX::XMConvertToRadians(roll);
		Matrix R = Matrix::CreateRotationX(xRad) *
			Matrix::CreateRotationY(yRad) *
			Matrix::CreateRotationZ(zRad);
		return R;
	}

	//roll - around Z vector
	//yaw - around Y vector
	//pitch - around X vector
	inline Vector3 RotateVector(const Vector3& v, float roll, float yaw, float pitch)
	{
		float xRad = DirectX::XMConvertToRadians(pitch);
		float yRad = DirectX::XMConvertToRadians(yaw);
		float zRad = DirectX::XMConvertToRadians(roll);
		Matrix R = Matrix::CreateRotationX(xRad) *
			Matrix::CreateRotationY(yRad) *
			Matrix::CreateRotationZ(zRad);

		return Vector3::Transform(v, R);
	}

	inline void RadiansToDegree(Vector3& degrees)
	{
		degrees *= 180.0f / PhysicsUtils::Consts::pi;
	}
}
