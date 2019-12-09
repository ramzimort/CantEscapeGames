/**
 * @file Triangle.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of a triangle
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
 #pragma once

/**
 * @brief defenition of the triangle
 * 
 */
class Triangle
{
public:
	Triangle();
	Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	/**
	 * @brief Checks if the triangle is degenerate(two or all 3 points are the same, or all three points lay on a line)
	 * 
	 * @return true 
	 * @return false 
	 */
	bool IsDegenerateTriangle() const;
	/**
	 * @brief Calculates a normal of the triangles
	 * 
	 * @return Vector3 
	 */
	Vector3 Normal() const;

	Vector3 points[3];
};