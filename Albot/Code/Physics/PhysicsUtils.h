/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

namespace PhysicsUtils
{
	namespace Consts
	{
		// math
		const float pi = 4 * atanf(1);
		const float e = 2.71828182845904523536028747135266249775724709369995f;
		const double sqrt2 = std::sqrt(2.0);

		//physics
		const int maxPhysicsIterationsPerFrame = 5;
		const float fixedTimeStep = 1.0f / (60.0f * maxPhysicsIterationsPerFrame);
		const float fatteningFactor = 1.1f;
		const Vector3 gravity(0.0f, -9.8f, 0.0f);
		
		// path
		const std::string Assets_Dir = "Assets\\";
		const std::string Physics_Dir = Assets_Dir + "Physics\\";
		const std::string Collision_Matrix_Dir = Physics_Dir + "Collision_Matrix\\";
	}

	template <typename T>
	inline T Min(const T lhs, const T rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	template <typename T>
	inline T Max(const T lhs, const T rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}

	inline Vector3 Max(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Max(lhs.x, rhs.x), Max(lhs.y, rhs.y), Max(lhs.z, rhs.z));
	}

	inline Vector3 Min(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Min(lhs.x, rhs.x), Min(lhs.y, rhs.y), Min(lhs.z, rhs.z));
	}

	template <typename  T>
	inline T Abs(T rhs)
	{
		return rhs < 0 ? -rhs : rhs;
	}

	template <typename T>
	inline T Sqr(T rhs)
	{
		return rhs * rhs;
	}

	bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b,
		float& u, float& v, float expansionEpsilon = 0.0f);

	bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c,
		float& u, float& v, float& w, float expansionEpsilon = 0.0f);


	
#ifdef DEVELOPER
	namespace MathPrinter
	{
		inline void PrintVector3(const Vector3& vec)
		{
			std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]" << std::endl;
		}
		inline void PrintVector4(const Vector4& vec)
		{
			std::cout << "[" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]" << std::endl;
		}

		inline void PrintMatrix(const Matrix& m)
		{
			const DirectX::XMFLOAT4X4& mXMFloat(m);

			for (int i = 0; i < 4; ++i)
			{
				PrintVector4(Vector4(mXMFloat.m[i]));
			}
		}
	};
#endif
}
