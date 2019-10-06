/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/
#pragma once

class Triangle
{
public:
	Triangle();
	Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);

	bool IsDegenerateTriangle() const;
	Vector3 Normal() const;

	Vector3 points[3];
};