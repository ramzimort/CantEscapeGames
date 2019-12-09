/**
 * @file PhysicsUtils.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
 #include "PhysicsUtils.h"
#include "Shaders\Shading.h"

extern bool PhysicsUtils::Settings::isDrawContactPoints = false;
extern bool PhysicsUtils::Settings::isDrawGJKResult = false;
extern bool PhysicsUtils::Settings::isDrawEPAFinalTriangle = false;
extern bool PhysicsUtils::Settings::Draw_Dynamic_AABB_Tree = false;
extern int PhysicsUtils::Settings::dynamicAabbLevelDraw = -1;
extern bool PhysicsUtils::Settings::isDrawTriggersTree = false;

std::vector<Vector3> PhysicsUtils::UniformDistibutionInSphere()
{
	std::vector<Vector3> uniformVectors; // uniformly distributed vectors from O to a surface of a unit sphere

	const float angleStep = PI / 4.0f;
	uniformVectors.emplace_back(0.0f, 1.0f, 0.0f);
	for (float theta = 0; theta < 2 * PI; theta += angleStep)
	{
		for (float phi = angleStep; phi < PI; phi += angleStep)
		{
			float x = sin(phi) * cos(theta);
			float y = cos(phi);
			float z = sin(phi) * sin(theta);
			Vector3 dir(x, y, z);
			dir.Normalize();
			uniformVectors.push_back(dir);
		}
	}
	uniformVectors.emplace_back(0.0f, -1.0f, 0.0f);
	return uniformVectors;
}

bool PhysicsUtils::BarycentricCoordinates(const Vector3& p, const Vector3& a, const Vector3& b, float& u, float& v, float expansionEpsilon)
{
	// check if the line is degenerate
	if (a == b)
	{
		u = 0.0f;
		v = 0.0f;

		return false;
	}

	// geometric approach
	const Vector3 ab = b - a;
	const Vector3 ap = p - a;
	const float abLenSq = ab.LengthSquared();

	v = ap.Dot(ab) / abLenSq;
	u = 1 - v;

	// is the point outside the line?
	if (u < -expansionEpsilon || u > 1 + expansionEpsilon || v < -expansionEpsilon || v > 1 + expansionEpsilon)
	{
		return false;
	}
	return true;
}

bool PhysicsUtils::BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, float& u, float& v, float& w, float expansionEpsilon)
{
	const Vector3 ab = b - a;
	const Vector3 ac = c - a;
	
	Vector3 normalAbc;
	ab.Cross(ac, normalAbc);
	float normalAbcLengthSq = normalAbc.LengthSquared(); // precompute
	if (normalAbcLengthSq == 0)	// degenerate triangle?
	{
		u = 0;
		v = 0;
		w = 0;
		return false;
	}

	// signed area approach and simplified
	Vector3 point_c = c - point; // precompute
	Vector3 point_b = b - point; // precompute
	Vector3 normalPbc;
	point_b.Cross(point_c, normalPbc);
	Vector3 normalPca;
	point_c.Cross(a - point, normalPca);
	//computing u,v,w
	u = normalPbc.Dot(normalAbc) / normalAbcLengthSq;
	v = normalPca.Dot(normalAbc) / normalAbcLengthSq;
	w = 1 - u - v;

	// is the point outside the triangle?
	if (u < -expansionEpsilon || u > 1 + expansionEpsilon ||
		v < -expansionEpsilon || v > 1 + expansionEpsilon ||
		w < -expansionEpsilon || w > 1 + expansionEpsilon)
	{
		return false;
	}
	// else the point inside of the triangle
	return true;
}

Vector3 PhysicsUtils::WorldToModel(const Matrix& modelToWorld, const Vector3& vec)
{
	const Matrix worldToModel = modelToWorld.Invert();
	return Vector3::Transform(vec, worldToModel);
}