/**
 * @file PhysicsUtils.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Constants, settings, helper functions for physics 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once



#include "CantDebug/CantDebug.h"

namespace PhysicsUtils
{
	/**
	 * @brief Generates uniformly distributed vectors from origin to the surface of a unit sphere
	 * 
	 * @return std::vector<Vector3> 
	 */
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
		const float fatteningFactor = 1.0f;
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
		extern int dynamicAabbLevelDraw;
		extern bool Draw_Dynamic_AABB_Tree;
		extern bool isDrawTriggersTree;
	}

	/**
	 * @brief Minimum between two objects. Note: Objects have to have operator<
	 * 
	 * @tparam T 
	 * @param lhs 
	 * @param rhs 
	 * @return T 
	 */
	template <typename T>
	inline T Min(const T lhs, const T rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	/**
	 * @brief Maximum between two objects. Note: Objects have to have operator<
	 * 
	 * @tparam T 
	 * @param lhs 
	 * @param rhs 
	 * @return T 
	 */
	template <typename T>
	inline T Max(const T lhs, const T rhs)
	{
		return lhs < rhs ? rhs : lhs;
	}
	/**
	 * @brief Maximum between two Vector3 component by component and returns Vector3 in which each entry is the result on that axis
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return Vector3 
	 */
	inline Vector3 Max(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Max(lhs.x, rhs.x), Max(lhs.y, rhs.y), Max(lhs.z, rhs.z));
	}
	/**
	 * @brief Minimum between two Vector3 component by component and returns Vector3 in which each entry is the result on that axis
	 * 
	 * @param lhs 
	 * @param rhs 
	 * @return Vector3 
	 */
	inline Vector3 Min(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Min(lhs.x, rhs.x), Min(lhs.y, rhs.y), Min(lhs.z, rhs.z));
	}
	/**
	 * @brief Absulute value of the two class objects. Note: Objects have to have operator< and operator- (as a unari negate operator)
	 * 
	 * @tparam T 
	 * @param rhs 
	 * @return T 
	 */
	template <typename  T>
	inline T Abs(T rhs)
	{
		return rhs < 0 ? -rhs : rhs;
	}
	/**
	 * @brief Calculates Square of the value. Note: has to has operator* (binary multiplication)
	 * 
	 * @tparam T 
	 * @param rhs 
	 * @return T 
	 */
	template <typename T>
	inline T Sqr(T rhs)
	{
		return rhs * rhs;
	}
	/**
	 * @brief Barrycentric coordinates of a point in respect to two points
	 * 
	 * @param point 
	 * @param a 
	 * @param b 
	 * @param u 
	 * @param v 
	 * @param expansionEpsilon 
	 * @return true 
	 * @return false 
	 */
	bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b,
		float& u, float& v, float expansionEpsilon = 0.0f);
	/**
	 * @brief Barrycentric coordinates of a point in respect to three points
	 * 
	 * @param point 
	 * @param a 
	 * @param b 
	 * @param c 
	 * @param u 
	 * @param v 
	 * @param w 
	 * @param expansionEpsilon 
	 * @return true 
	 * @return false 
	 */
	bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c,
		float& u, float& v, float& w, float expansionEpsilon = 0.0f);
	/**
	 * @brief Transforms a vector in a model space to world space
	 * 
	 * @param modelToWorld 
	 * @param vec 
	 * @return Vector3 
	 */
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
