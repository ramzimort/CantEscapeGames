#include "MathUtil.h"


Vector3 MathUtil::MatrixToRadEulerAngles(const Matrix& matrix)
{
	//x = pitch
	//y = yaw
	//z = roll
	Vector3 euler;

	//heading = y
	//pitch = x
	//bank = z

	float h, p, b;

	static const float pi_over_2 = 1.570796f;
	float sp = -matrix._32;
	if (sp <= -1.f)
	{
		p = -pi_over_2; //-pi/2;
	}
	else if (sp >= 1.f)
	{
		p = pi_over_2;
	}
	else
	{
		p = asin(sp);
	}

	if (fabsf(sp) > 0.999f)
	{
		b = 0.f;
		h = atan2(-matrix._13, matrix._11);
	}
	else
	{
		h = atan2(matrix._31, matrix._33);
		b = atan2(matrix._12, matrix._22);
	}
	euler = Vector3(p, h, b);
	return euler;

}

Vector3 MathUtil::MatrixToDegreeEulerAngles(const Matrix& matrix)
{
	Vector3 radEulerAngle = MatrixToRadEulerAngles(matrix);
	return Vector3
	(
		DirectX::XMConvertToDegrees(radEulerAngle.x), 
		DirectX::XMConvertToDegrees(radEulerAngle.y), 
		DirectX::XMConvertToDegrees(radEulerAngle.z)
	);
}