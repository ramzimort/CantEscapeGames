/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/
#pragma once

#include "Physics/Gjk/CollisionManifold.h"
#include "Physics/SuppportShape/ObbSupportShape.h"
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

	void MultiplyByJacobian(Jacobian& result, const Jacobian& rhs);
};

//      v1  w1  v2  w2
//  x [                ]
//  y [                ]
//  z [                ]
//  0 [                ]
class Jacobian
{
public:
	Vector3 m_velocity1;
	Vector3 m_angularVelocity1;
	Vector3 m_velocity2;
	Vector3 m_angularVelocity2;

	Jacobian();
	Jacobian(const Vector3& v1, const Vector3 w1, const Vector3 v2, const Vector3 w2);

	// this is used when multiplying J and V used in constraints calculation, should only return 1 number
	float operator*(const Jacobian& rhs);
	void MultiplyByFloat(Jacobian& result, float rhs);
	void AddJacobian(Jacobian& result, const Jacobian& rhs);
};

// class PhysicalObject;
class Constraint
{
public:
	Constraint();
	Constraint(RigidbodyComponent* obj1, RigidbodyComponent* obj2, float depth = 0.0f);
	~Constraint();

	// calculates coefficients for the Velocity constraint for normal constraint
	void CalculateNormalJacobian(const Vector3& normal, const Vector3& pointA, const Vector3& pointB);

	// calculates coefficients for the Velocity constraint for friction constraints, there are 2 of them
	static void CalculateFrictionJacobians(const Vector3& normal, const Vector3& pointA, const Vector3& pointB, Constraint& constraintFriction1, Constraint& constraintFriction2);
	static void ToMatrix4(Matrix& result, const Jacobian& jacobian);

	Jacobian m_jacobian;                  // coefficients for linear and angular velocity of both objects
	RigidbodyComponent* m_object1;		  // first object
	RigidbodyComponent* m_object2;		  // second object
	Vector3 m_normal;                     // collision normal
	float m_bias;                         // constraint bias
	float m_lambda;                       // undetermained multiplier
	float m_depthPen;                     // depth penetration


private:
	// helper function to buil jacobian for normal and friction constraint
	void CalculateJacobian(const Vector3& direction, const Vector3& radiusVector, const Vector3& radiusVector2);
};

class Contact
{
public:
	Contact();
	void BuildConstraints();

	std::vector<Constraint> m_constraints;

	RigidbodyComponent* m_objectA;
	RigidbodyComponent* m_objectB;
	ObbSupportShape m_supportShapeA;
	ObbSupportShape m_supportShapeB;
	Vector3 m_pALocal;				 // object space coords
	Vector3 m_pBLocal;				 // object space coords
	Vector3 m_pA;					 // world coords
	Vector3 m_pB;					 // world coords
	float m_depth;
	Vector3 m_normal;
};

class ContactManifold
{
public:
	ContactManifold(Contact& collision);
	void ProccessCollision(Contact& collision);
	void KeepOnlyFourContacts();

	RigidbodyComponent* m_object1;		  // first object
	RigidbodyComponent* m_object2;		  // second object
	std::vector<Contact> m_contacts;
};