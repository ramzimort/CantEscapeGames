
#include "Constraint.h"

MassMatrix::MassMatrix()
{
}

MassMatrix::MassMatrix(const Matrix& mass1, const Matrix& inertiaTensor1,
	const Matrix& mass2, const Matrix& inertiaTensor2) :
	m_mass1(mass1), m_inertiaTensor1(inertiaTensor1), m_mass2(mass2), m_inertiaTensor2(inertiaTensor2)
{
}

void MassMatrix::MultiplyByJacobian(Jacobian& result, const Jacobian& rhs)
{
	result.m_velocity1 = Vector3::Transform(rhs.m_velocity1, m_mass1);
	result.m_angularVelocity1 = Vector3::Transform(rhs.m_angularVelocity1, m_inertiaTensor1);
	result.m_velocity2 = Vector3::Transform(rhs.m_velocity2, m_mass2);
	result.m_angularVelocity2 = Vector3::Transform(rhs.m_angularVelocity2, m_inertiaTensor2);
}

Jacobian::Jacobian()
{
}

Jacobian::Jacobian(const Vector3& v1, const Vector3 w1, const Vector3 v2, const Vector3 w2) :
	m_velocity1(v1), m_angularVelocity1(w1), m_velocity2(v2), m_angularVelocity2(w2)
{
}

float Jacobian::operator*(const Jacobian& rhs)
{
	return m_velocity1.Dot(rhs.m_velocity1) +
		m_angularVelocity1.Dot(rhs.m_angularVelocity1) +
		m_velocity2.Dot(rhs.m_velocity2) +
		m_angularVelocity2.Dot(rhs.m_angularVelocity2);
}

void Jacobian::MultiplyByFloat(Jacobian& result, float rhs)
{
	result.m_velocity1 = m_velocity1 * rhs;
	result.m_angularVelocity1 = m_angularVelocity1 * rhs;
	result.m_velocity2 = m_velocity2 * rhs;
	result.m_angularVelocity2 = m_angularVelocity2 * rhs;
}

void Jacobian::AddJacobian(Jacobian& result, const Jacobian& rhs)
{
	result.m_velocity1 = m_velocity1 + rhs.m_velocity1;
	result.m_angularVelocity1 = m_angularVelocity1 + rhs.m_angularVelocity1;
	result.m_velocity2 = m_velocity2 + rhs.m_velocity2;
	result.m_angularVelocity2 = m_angularVelocity2 + rhs.m_angularVelocity2;
}

Constraint::Constraint() : m_object1(nullptr), m_object2(nullptr), m_bias(0.0f), m_lambda(0.0f), m_minLambda(0.0f), m_maxLambda(FLT_MAX), m_depthPen(0.0f)
{
}

Constraint::Constraint(RigidbodyComponent* obj1, RigidbodyComponent* obj2, float depth) : m_object1(obj1), m_object2(obj2), m_depthPen(depth)
{
}

Constraint::~Constraint()
{
}

void Constraint::CalculateNormalJacobian(const CollisionManifold& collision)
{
	normal = collision.m_normal;
	normal.Normalize();
	Vector3 r1 = collision.m_pA - m_object1->GetPosition();
	Vector3 r2 = collision.m_pB - m_object2->GetPosition();

	CalculateJacobian(normal, r1, r2);

	/*Math::Matrix4 result;
	ToMatrix4(result, jacobian);
	object1->constraints.push_back(result);
	object2->constraints.push_back(result);*/
}

void Constraint::CalculateFrictionJacobians(const CollisionManifold& collision, Constraint& constraintFriction1, Constraint& constraintFriction2)
{
	// calculate the tangent vectors to normal to simulate both frictions
	Vector3 normal = collision.m_normal;
	normal.Normalize();

	Vector3 u1; // tangent to normal, direction unit vector for friction in 1 of the directions
	Vector3 u2; // bitangent to normal, direction unit vector for friction in 2 of the directions
	MathUtil::BuildTangentBitangent(normal, u1, u2);

	const Vector3 r1 = collision.m_pA - constraintFriction1.m_object1->GetPosition();
	const Vector3 r2 = collision.m_pB - constraintFriction1.m_object2->GetPosition();

	constraintFriction1.CalculateJacobian(u1, r1, r2);
	constraintFriction2.CalculateJacobian(u2, r1, r2);
}

void Constraint::ToMatrix4(Matrix& result, const Jacobian& jacobian)
{
	result = Matrix(Vector4(jacobian.m_velocity1.x, jacobian.m_velocity1.y, jacobian.m_velocity1.z, 0), 
					Vector4(jacobian.m_angularVelocity1.x, jacobian.m_angularVelocity1.y, jacobian.m_angularVelocity1.z, 0), 
					Vector4(jacobian.m_velocity2.x, jacobian.m_velocity2.y, jacobian.m_velocity2.z, 0), 
					Vector4(jacobian.m_angularVelocity2.x, jacobian.m_angularVelocity2.y, jacobian.m_angularVelocity2.z, 1));
}

void Constraint::CalculateJacobian(const Vector3& direction, const Vector3& radiusVector1, const Vector3& radiusVector2)
{
	const Vector3 r1CrossDir = radiusVector1.Cross(direction);
	const Vector3 r2CrossDir = radiusVector2.Cross(direction);

	m_jacobian.m_velocity1 = -direction;
	m_jacobian.m_angularVelocity1 = -r1CrossDir;
	m_jacobian.m_velocity2 = direction;
	m_jacobian.m_angularVelocity2 = r2CrossDir;
}