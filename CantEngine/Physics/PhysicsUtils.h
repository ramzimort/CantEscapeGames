/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once



#include "CantDebug/CantDebug.h"

namespace PhysicsUtils
{
	std::vector<Vector3> UniformDistibutionInSphere();

	namespace Consts
	{
		// math
		const float pi = 4 * atanf(1);
		const float pi_2 = pi / 2;
		const float pi_3 = pi / 3;
		const float pi_4 = pi / 4;
		const float pi_6 = pi / 6;
		const float e = 2.71828182845904523536028747135266249775724709369995f;
		const double sqrt2 = std::sqrt(2.0);

		//physics
		const int maxPhysicsIterationsPerFrame = 1;
		const float fixedTimeStep = 1.0f / (60.0f * maxPhysicsIterationsPerFrame);
		const float fatteningFactor = 1.1f;
		const float gravity = -9.8f;
		const float minMass = 0.000001f;
		const float penetrationEpsilon = 0.001f;

		const std::vector<Vector3> directionsInUnitSphere = PhysicsUtils::UniformDistibutionInSphere();

		namespace Constraints
		{
			const int numGaussSeidelIterations = 10;
			const float bias = 0.2f;                   // damping term for position
			const float restitution = 0.4f;            // damping term for velocity
			const float friction = 0.2f;               // friction coefficient
		}
		
		// path
		const std::string Assets_Dir = "Assets\\";
		const std::string Physics_Dir = Assets_Dir + "Physics\\";
		const std::string Collision_Matrix_Dir = Physics_Dir + "Collision_Matrix\\";
	}

	namespace Settings
	{
		extern bool isDrawContactPoints;
		extern bool isDrawGJKResult;
		extern bool isDrawEPAFinalTriangle;
		extern bool isDrawConstraints;
		extern int dynamicAabbLevelDraw;
		extern bool Draw_Dynamic_AABB_Tree;
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
	
	Vector3 WorldToModel(const Matrix& modelToWorld, const Vector3& vec);


#ifdef DEVELOPER
	namespace MathPrinter
	{
		inline void PrintVector3(const Vector3& vec)
		{
			DEBUG_LOG("[%f, %f, %f]\n", vec.x, vec.y, vec.z);
		}
		inline void PrintVector4(const Vector4& vec)
		{
			DEBUG_LOG("[%f, %f, %f, %f]\n", vec.x, vec.y, vec.z, vec.w);
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
