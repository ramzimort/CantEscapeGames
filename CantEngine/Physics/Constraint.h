/**
 * @file Constraint.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Everything that is related to constraints
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
#pragma once

#include "Physics/Gjk/CollisionManifold.h"
#include "Physics/SuppportShape/ObbSupportShape.h"
#include "Physics/SuppportShape/ModelSupportShape.h"
#include "Physics/Gjk/Gjk.h"

class RigidbodyComponent;
class Jacobian;
class CollisionManifold;

// [m1         ]
// [   I1      ]
// [      m2   ]
// [         I2]
// where m1 and m2 just mass * identity3 for both objects
// and I1, I2 are inertia tensors for both objects
// so each entry is 3 by 3 matrix
/**
 * @brief Mass matrix 12x12, with 3x3 matrices on the diagonal and zeros everywhere else
 * 
 */
class MassMatrix
{
public:
	Matrix m_mass1;
	Matrix m_inertiaTensor1;
	Matrix m_mass2;
	Matrix m_inertiaTensor2;

	MassMatrix();
	MassMatrix(const Matrix& mass1, const Matrix& inertiaTensor1,
		const Matrix& mass2, const Matrix& inertiaTensor2);

	/**
	 * @brief Multiplies each 3x3 matrix in the diagonal by each 3x1 vector in the Jacobian vector
	 * 
	 * @param result 
	 * @param rhs 
	 */
	void MultiplyByJacobian(Jacobian& result, const Jacobian& rhs);
};

//      v1  w1  v2  w2
//  x [                ]
//  y [                ]
//  z [                ]
//  0 [                ]
/**
 * @brief Stores 12x1 Jacobian vector where every 3 entry is a 3x1 vector
 * 
 */
class Jacobian
{
public:
	Vector3 m_velocity1;
	Vector3 m_angularVelocity1;
	Vector3 m_velocity2;
	Vector3 m_angularVelocity2;

	Jacobian();
	Jacobian(const Vector3& v1, const Vector3 w1, const Vector3 v2, const Vector3 w2);

	/**
	 * @brief Dot product of two 12x1 vectors
	 * 
	 * @param rhs 
	 * @return float 
	 */
	float operator*(const Jacobian& rhs);
	/**
	 * @brief Multiplication by a scalar
	 * 
	 * @param result 
	 * @param rhs 
	 */
	void MultiplyByFloat(Jacobian& result, float rhs);
	/**
	 * @brief Adding another 12x1 vector to this vector
	 * 
	 * @param result 
	 * @param rhs 
	 */
	void AddJacobian(Jacobian& result, const Jacobian& rhs);
};

/**
 * @brief Stores and computes all the constraint information
 * 
 */
class Constraint
{
public:
	Constraint();
	Constraint(RigidbodyComponent* obj1, RigidbodyComponent* obj2, float depth = 0.0f);
	~Constraint();

	/**
	 * @brief Calculates coefficients for the Velocity constraint for normal constraint
	 * 
	 * @param normal 
	 * @param pointA 
	 * @param pointB 
	 */
	void CalculateNormalJacobian(const Vector3& normal, const Vector3& pointA, const Vector3& pointB);
	/**
	 * @brief Calculates coefficients for the Velocity constraint for friction constraints, there are 2 of them
	 * 
	 * @param normal 
	 * @param pointA 
	 * @param pointB 
	 * @param constraintFriction1 
	 * @param constraintFriction2 
	 */
	static void CalculateFrictionJacobians(const Vector3& normal, const Vector3& pointA, const Vector3& pointB, Constraint& constraintFriction1, Constraint& constraintFriction2);
	/**
	 * @brief Represents of a 12x1 Jacobian vector as a 4x4 matrix
	 * 
	 * @param result 
	 * @param jacobian 
	 */
	static void ToMatrix4(Matrix& result, const Jacobian& jacobian);

	Jacobian m_jacobian;                  // coefficients for linear and angular velocity of both objects
	RigidbodyComponent* m_object1;		  // first object
	RigidbodyComponent* m_object2;		  // second object
	Vector3 m_normal;                     // collision normal
	float m_bias;                         // constraint bias
	float m_lambda;                       // undetermained multiplier
	float m_depthPen;                     // depth penetration


private:
	/**
	 * @brief helper function to buil jacobian for normal and friction constraint
	 * 
	 * @param direction 
	 * @param radiusVector 
	 * @param radiusVector2 
	 */
	void CalculateJacobian(const Vector3& direction, const Vector3& radiusVector, const Vector3& radiusVector2);
};

/**
 * @brief Stores all the information relative to the contact
 * 
 */
class Contact
{
public:
	Contact();
	/**
	 * @brief Builds the 3 constraints for the contact from the previously initialized data
	 * 
	 */
	void BuildConstraints();

	std::vector<Constraint> m_constraints;

	RigidbodyComponent* m_objectA;
	RigidbodyComponent* m_objectB;
	Vector3 m_pALocal;				 // object space coords
	Vector3 m_pBLocal;				 // object space coords
	Vector3 m_pA;					 // world coords
	Vector3 m_pB;					 // world coords
	float m_depth;
	Vector3 m_normal;
};

/**
 * @brief Stores up to 4 contacts per collision between 2 rigid bodies
 * 
 */
class ContactManifold
{
public:
	ContactManifold(Contact& collision);
	/**
	 * @brief Proccesses a new contact into the manifold
	 * 
	 * @param collision 
	 */
	void ProccessCollision(Contact& collision);
	/**
	 * @brief Drops unnecessary contacts from the manifold
	 * 
	 */
	void KeepOnlyFourContacts();

	RigidbodyComponent* m_object1;		  // first object
	RigidbodyComponent* m_object2;		  // second object
	std::vector<Contact> m_contacts;
};