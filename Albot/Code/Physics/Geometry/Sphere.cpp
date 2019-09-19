/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "Sphere.h"
#include "Intersection.h"

///////////// PCA Helpers ////////////////////////////
//DirectX::SimpleMath::Matrix ComputeCovarianceMatrix(const std::vector<DirectX::SimpleMath::Vector3>& points)
//{
//	const size_t size = points.size();
//
//	// calculating mean
//	DirectX::SimpleMath::Vector3 u;
//	for (size_t i = 0; i < size; i++)
//	{
//		u += points[i];
//	}
//	u /= static_cast<float>(size);
//
//	DirectX::SimpleMath::Matrix totalC;
//
//	for (unsigned int k = 0; k < size; k++)
//	{
//		DirectX::SimpleMath::Vector3 v = points[k] - u;
//		DirectX::SimpleMath::Matrix c;
//		for (unsigned int i = 0; i < 3; i++)
//		{
//			for (unsigned int j = 0; j < 3; j++)
//			{
//				c.m[i][j] = v[i] * v[j];
//			}
//		}
//		totalC += c;
//	}
//	totalC /= static_cast<float>(size);
//
//	return totalC;
//}
//
//DirectX::SimpleMath::Matrix ComputeJacobiRotation(const DirectX::SimpleMath::Matrix& matrix)
//{
//	// Compute the jacobi rotation matrix that will turn the largest (magnitude) off-diagonal element of the input
//	// matrix into zero. Note: the input matrix should always be (near) symmetric.
//	float cosine, sine, tangent;
//
//	// findint indecies of the largest off diagnal element
//	float abs01 = abs(matrix.m[0][1]);
//	float abs02 = abs(matrix.m[0][2]);
//	float abs12 = abs(matrix.m[1][2]);
//	int i_pq, j_pq;
//	if (abs02 > abs01)
//	{
//		if (abs12 > abs02)
//		{
//			i_pq = 1;
//			j_pq = 2;
//		}
//		else
//		{
//			i_pq = 0;
//			j_pq = 2;
//		}
//	}
//	else
//	{
//		if (abs12 > abs01)
//		{
//			i_pq = 1;
//			j_pq = 2;
//		}
//		else
//		{
//			i_pq = 0;
//			j_pq = 1;
//		}
//	}
//
//	// computing cosine and sine
//	if (matrix[i_pq][j_pq] == 0)
//	{
//		cosine = 1.0f;
//		sine = 0.0f;
//	}
//	else
//	{
//		float b = (matrix.m[j_pq][j_pq] - matrix.m[i_pq][i_pq]) / (2 * matrix.m[i_pq][j_pq]);
//		float sign_b = (b < 0.0f) ? -1.0f : 1.0f;
//
//		tangent = sign_b / (abs(b) + std::sqrt(b * b + 1));
//		cosine = 1 / std::sqrt(1.0f + tangent * tangent);
//		sine = tangent * cosine;
//	}
//
//	// computing resulting matrix
//	DirectX::SimpleMath::Matrix result;
//	result.m[i_pq][i_pq] = cosine;
//	result.m[i_pq][j_pq] = sine;
//	result.m[j_pq][i_pq] = -sine;
//	result.m[j_pq][j_pq] = cosine;
//
//	return result;
//}
//
//void ComputeEigenValuesAndVectors(const DirectX::SimpleMath::Matrix& covariance, DirectX::SimpleMath::Vector3& eigenValues, DirectX::SimpleMath::Matrix& eigenVectors, int maxIterations)
//{
//	// Iteratively rotate off the largest off-diagonal elements until the resultant matrix is diagonal or maxIterations.
//	eigenVectors.Identity();
//	DirectX::SimpleMath::Matrix currentMatrix(covariance);
//	for (int i = 0; i < maxIterations; i++)
//	{
//		if (std::abs(currentMatrix.m[0][1]) < FLT_EPSILON && std::abs(currentMatrix.m[0][2]) < FLT_EPSILON && std::abs(currentMatrix.m[1][2]) < FLT_EPSILON)
//		{
//			break;
//		}
//		DirectX::SimpleMath::Matrix jacobiRotation = ComputeJacobiRotation(currentMatrix);
//
//		currentMatrix = jacobiRotation.Transpose() * currentMatrix * jacobiRotation;
//
//		eigenVectors = eigenVectors * jacobiRotation;
//	}
//
//	for (int i = 0; i < 3; i++)
//	{
//		eigenValues[i] = currentMatrix.m[i][i];
//	}
//}

//////////////////////////////////////// Sphere ////////////////////////////////////////
Sphere::Sphere() : mCenter(DirectX::SimpleMath::Vector3()), mRadius(0.0f)
{
}

Sphere::Sphere(const DirectX::SimpleMath::Vector3& center, float radius) : mCenter(center), mRadius(radius)
{
}

bool Sphere::IsIntersect(const Aabb& aabb) const
{
	return Intersection::AabbSphere(aabb.mMin, aabb.mMax, mCenter, mRadius);
}

bool Sphere::IsIntersect(const Sphere& sphere) const
{
	return Intersection::SphereSphere(mCenter, mRadius, sphere.mCenter, sphere.mRadius);
}

//
//void Sphere::ComputeCentroid(const std::vector<DirectX::SimpleMath::Vector3>& points)
//{
//	// The centroid method is roughly describe as: find the centroid (not mean) of all
//	// points and then find the furthest away point from the centroid.
//	unsigned size = points.size();
//	Aabb aabb;
//	aabb.Compute(points);
//
//	mCenter = aabb.GetCenter();
//	mRadius = 0;
//	for (unsigned i = 0; i < size; i++)
//	{
//		mRadius = Math::Max(mRadius, Math::LengthSq(points[i] - mCenter));
//	}
//
//	mRadius = sqrt(mRadius);
//}
//
//void IterativeExpand(Vector3& c, float& r, const std::vector<Vector3>& points)
//{
//	for (auto it : points)
//	{
//		Vector3 pc = it - c;
//		float length = Math::LengthSq(pc);
//		if (length == 0 || length <= r * r)
//			continue;
//		length = Math::Sqrt(length);
//		float r_new = 0.5f * (length + r);
//		c = c + (pc / length) * (r_new - r);
//		r = r_new;
//	}
//}
//
//void Sphere::ComputeRitter(const std::vector<Vector3>& points)
//{
//	// The ritter method:
//	// Find the largest spread on each axis.
//	// Find which axis' pair of points are the furthest (euclidean distance) apart.
//	// Choose the center of this line as the sphere center. Now incrementally expand the sphere.
//	unsigned int size = points.size();
//
//	// getting min, max on each axis
//	Vector3 minPoints[3];
//	Vector3 maxPoints[3];
//	for (unsigned i = 0; i < 3; i++)
//	{
//		minPoints[i].Splat(Math::PositiveMax());
//		maxPoints[i].Splat(Math::NegativeMin());
//	}
//	for (unsigned int i = 0; i < size; i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			if (points[i][j] < minPoints[j][j])
//			{
//				minPoints[j] = points[i];
//			}
//			if (points[i][j] > maxPoints[j][j])
//			{
//				maxPoints[j] = points[i];
//			}
//		}
//	}
//
//	// figuring out which axis has the bigest spread
//	unsigned axis;
//	Vector3 lengthSq;
//	Vector3 dir[3];
//	for (int i = 0; i < 3; i++)
//	{
//		dir[i] = maxPoints[i] - minPoints[i];
//		lengthSq[i] = Math::LengthSq(dir[i]);
//	}
//
//	if (lengthSq.y > lengthSq.x)
//	{
//		if (lengthSq.z > lengthSq.y)
//		{
//			axis = 2;
//		}
//		else
//		{
//			axis = 1;
//		}
//	}
//	else
//	{
//		if (lengthSq.z > lengthSq.x)
//		{
//			axis = 2;
//		}
//		else
//		{
//			axis = 0;
//		}
//	}
//
//	// computing initial center and radius
//	float r = Math::Sqrt(lengthSq[axis]) / 2.0f;
//	Vector3 c = minPoints[axis] + dir[axis] / 2.0f;
//
//	//Iterative expend
//	IterativeExpand(c, r, points);
//
//	mCenter = c;
//	mRadius = r;
//}
//
//void Sphere::ComputePCA(const std::vector<Vector3>& points, int maxIterations)
//{
//	// The PCA method:
//	// Compute the eigen values and vectors. Take the largest eigen vector as the axis of largest spread.
//	// Compute the sphere center as the center of this axis then expand by all points.
//	unsigned int size = points.size();
//	Matrix3 covarianceM = ComputeCovarianceMatrix(points);
//	Vector3 eigenValues;
//	Matrix3 eigenVectors;
//	ComputeEigenValuesAndVectors(covarianceM, eigenValues, eigenVectors, maxIterations);
//
//	// figuring out which axis has the bigest spread
//	unsigned axis;
//
//	if (eigenValues.y > eigenValues.x)
//	{
//		if (eigenValues.z > eigenValues.y)
//		{
//			axis = 2;
//		}
//		else
//		{
//			axis = 1;
//		}
//	}
//	else
//	{
//		if (eigenValues.z > eigenValues.x)
//		{
//			axis = 2;
//		}
//		else
//		{
//			axis = 0;
//		}
//	}
//
//	// getting min, max on axis
//	Vector3 dir = eigenVectors.Basis(axis).Normalized();
//	float  minDot = Math::PositiveMax();
//	float  maxDot = Math::NegativeMin();
//	Vector3 minPoint;
//	Vector3 maxPoint;
//	for (unsigned int i = 0; i < size; i++)
//	{
//		float axis_dot_p = Math::Dot(points[i], dir);
//		if (axis_dot_p < minDot)
//		{
//			minPoint = points[i];
//			minDot = axis_dot_p;
//		}
//		if (axis_dot_p > maxDot)
//		{
//			maxPoint = points[i];
//			maxDot = axis_dot_p;
//		}
//	}
//
//	// computing initial center and radius
//	float dist = Math::Length(maxPoint - minPoint);
//	float r = dist / 2.0f;
//	Vector3 c = minPoint + r * (maxPoint - minPoint) / dist;
//
//	//Iterative expend
//	IterativeExpand(c, r, points);
//
//	mCenter = c;
//	mRadius = r;
//}

bool Sphere::ContainsPoint(const DirectX::SimpleMath::Vector3& point)
{
	return Intersection::PointSphere(point, mCenter, mRadius);
}

DirectX::SimpleMath::Vector3 Sphere::GetCenter() const
{
	return mCenter;
}

float Sphere::GetRadius() const
{
	return mRadius;
}

bool Sphere::Compare(const Sphere& rhs, float epsilon) const
{
	float distSqr = (mCenter - rhs.mCenter).LengthSquared();
	float radiusDiff = std::abs(mRadius - rhs.mRadius);

	return distSqr < epsilon * epsilon && radiusDiff < epsilon;
}

#ifdef DEVELOPER
void Sphere::DebugDraw(const DirectX::SimpleMath::Vector4& /*color*/) const
{
	// TODO
}

void Sphere::Print() const
{
	std::cout << "center = ";
	std::cout << "[" << mCenter.x << ", " << mCenter.y << ", " << mCenter.z << "]" << std::endl;
	std::cout << "radius = " << mRadius << std::endl;
}
#endif