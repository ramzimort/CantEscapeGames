/**
 * @file Triangle.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
#include "Triangle.h"

Triangle::Triangle()
{
}

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
	points[0] = p0;
	points[1] = p1;
	points[2] = p2;
}

bool Triangle::IsDegenerateTriangle() const
{	
	return Normal().LengthSquared() == 0;
}

Vector3 Triangle::Normal() const
{
	Vector3 normal;
	const Vector3 edge1 = points[1] - points[0];
	const Vector3 edge2 = points[2] - points[0];
	edge1.Cross(edge2, normal);
	
	return normal;
}