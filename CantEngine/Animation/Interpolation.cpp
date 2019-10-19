///HEADER

#include "Interpolation.h"

#define EPSILON		0.05f


namespace MathUtil
{

	Vector3 Lerp(Vector3 const& origin, Vector3 const& destination, float alpha)
	{
		Vector3 interp;

		interp.x = (1 - alpha) * origin.x + alpha * destination.x;
		interp.y = (1 - alpha) * origin.y + alpha * destination.y;
		interp.z = (1 - alpha) * origin.z + alpha * destination.z;

		return interp;
	}


	Quaternion Slerp(MathUtil::Quaternion const& origin, MathUtil::Quaternion const& destination, float alpha)
	{
		MathUtil::Quaternion interp = origin;
		MathUtil::Quaternion orig_aux = origin;

		float d = origin.Dot(destination);
		float a;

		if (d >= (1.0f - EPSILON)) 
		{
			interp = origin * (1 - alpha) + destination * alpha;
			return interp.Normalize();
		}
		else if (d < 0) 
		{
			orig_aux = -origin;
			a = acosf(-d);
		}
		else 
			a = acosf(d);

		//Continue and return
		float sin_a = sinf(a);
		float sin_ta = sinf(alpha * a);
		interp = ((orig_aux * sinf((1 - alpha) * a) + destination * sin_ta)) / sin_a;
		return interp;
	}


	Matrix GenVQSMatrix(Vector3 const& v, MathUtil::Quaternion const& q, Vector3 const& s)
	{
		//SCALE
		Matrix S = Matrix::Identity;
		S._11 = s.x;
		S._22 = s.y;
		S._33 = s.z;

		//ROTATION
		Matrix Q = q.GetRotationMatrix();

		//TRANSLATION
		Matrix V = Matrix::Identity;
		V._14 = v.x;
		V._24 = v.y;
		V._34 = v.z;

		return V*Q*S;
	}
}