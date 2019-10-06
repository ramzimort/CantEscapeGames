/******************************************************************************/
/*!
\file   Gjk.h
\author Aleksey Perfilev
\par    Copyright 2019, Digipen Institute of Technology
\brief
Narrow phase of the physics engine.
*/
/******************************************************************************/
#include "Gjk.h"
#include "Physics/PhysicsUtils.h"
#include "CollisionManifold.h"

//////////////// Voronoi Region Tests //////////////////
VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
	size_t& newSize, int newIndices[4],
	Vector3& closestPoint, Vector3& searchDirection)
{
	closestPoint = p0;
	newSize = 1;
	newIndices[0] = 0;
	searchDirection = q - closestPoint;

	return VoronoiRegion::Point0;
}

VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
	size_t& newSize, int newIndices[4],
	Vector3& closestPoint, Vector3& searchDirection)
{
	float u, v;
	PhysicsUtils::BarycentricCoordinates(q, p0, p1, u, v);
	// inside of the line
	//                  q
	//       p0 *-------*-----------* p1
	if (u > 0 && v > 0)
	{
		closestPoint = u * p0 + v * p1;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge01;
	}

	// on the right side
	//                                      q
	//       p0 *------------------* p1     *
	else if (u <= 0)
	{
		closestPoint = p1;
		newSize = 1;
		newIndices[0] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point1;
	}
	// on the left side
	//   q                                   
	//   *   p0 *------------------* p1     
	else
	{
		closestPoint = p0;
		newSize = 1;
		newIndices[0] = 0;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point0;
	}
}

VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
	size_t& newSize, int newIndices[4],
	Vector3& closestPoint, Vector3& searchDirection)
{
	float u, v, w;
	float p0p1u, p0p1v, p1p2u, p1p2v, p2p0u, p2p0v;

	PhysicsUtils::BarycentricCoordinates(q, p0, p1, p0p1u, p0p1v);
	PhysicsUtils::BarycentricCoordinates(q, p1, p2, p1p2u, p1p2v);
	PhysicsUtils::BarycentricCoordinates(q, p2, p0, p2p0u, p2p0v);
	PhysicsUtils::BarycentricCoordinates(q, p0, p1, p2, u, v, w);

	if (p0p1v <= 0 && p2p0u <= 0)
	{
		closestPoint = p0;
		newSize = 1;
		newIndices[0] = 0;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point0;
	}
	else if (p1p2v <= 0 && p0p1u <= 0)
	{
		closestPoint = p1;
		newSize = 1;
		newIndices[0] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point1;
	}
	else if (p2p0v <= 0 && p1p2u <= 0)
	{
		closestPoint = p2;
		newSize = 1;
		newIndices[0] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point2;
	}
	// inside of the triangle
	else if (u > 0 && v > 0 && w > 0 && u < 1 && v < 1 && w < 1)
	{
		closestPoint = u * p0 + v * p1 + w * p2;
		newSize = 3;
		for (int i = 0; i < newSize; i++)
		{
			newIndices[i] = i;
		}
		searchDirection = q - closestPoint;
		return VoronoiRegion::Triangle012;
	}
	else if (p0p1u > 0 && p0p1v > 0 && w < 0)
	{
		closestPoint = p0p1u * p0 + p0p1v * p1;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge01;
	}
	else if (p1p2u > 0 && p1p2v > 0 && u < 0)
	{
		closestPoint = p1p2u * p1 + p1p2v * p2;
		newSize = 2;
		newIndices[0] = 1;
		newIndices[1] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge12;
	}
	else if (p2p0u > 0 && p2p0v > 0 && v < 0)
	{
		closestPoint = p2p0u * p2 + p2p0v * p0;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge02;
	}
	return VoronoiRegion::Unknown;
}

VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
	size_t& newSize, int newIndices[4],
	Vector3& closestPoint, Vector3& searchDirection)
{
	float p012u, p012v, p012w,
		p123u, p123v, p123w,
		p023u, p023v, p023w,
		p013u, p013v, p013w;
	float p0p1u, p0p1v, p0p2u, p0p2v, p0p3u, p0p3v;
	float p1p2u, p1p2v, p1p3u, p1p3v;
	float p2p3u, p2p3v;

	PhysicsUtils::BarycentricCoordinates(q, p0, p1, p0p1u, p0p1v);
	PhysicsUtils::BarycentricCoordinates(q, p0, p2, p0p2u, p0p2v);
	PhysicsUtils::BarycentricCoordinates(q, p0, p3, p0p3u, p0p3v);
	PhysicsUtils::BarycentricCoordinates(q, p1, p2, p1p2u, p1p2v);
	PhysicsUtils::BarycentricCoordinates(q, p1, p3, p1p3u, p1p3v);
	PhysicsUtils::BarycentricCoordinates(q, p2, p3, p2p3u, p2p3v);
	PhysicsUtils::BarycentricCoordinates(q, p0, p1, p2, p012u, p012v, p012w);
	PhysicsUtils::BarycentricCoordinates(q, p1, p2, p3, p123u, p123v, p123w);
	PhysicsUtils::BarycentricCoordinates(q, p0, p2, p3, p023u, p023v, p023w);
	PhysicsUtils::BarycentricCoordinates(q, p0, p1, p3, p013u, p013v, p013w);

	// point regions
	if (p0p1v <= 0 && p0p2v <= 0 && p0p3v <= 0)
	{
		closestPoint = p0;
		newSize = 1;
		newIndices[0] = 0;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point0;
	}
	else if (p0p1u <= 0 && p1p2v <= 0 && p1p3v <= 0)
	{
		closestPoint = p1;
		newSize = 1;
		newIndices[0] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point1;
	}
	else if (p0p2u <= 0 && p1p2u <= 0 && p2p3v <= 0)
	{
		closestPoint = p2;
		newSize = 1;
		newIndices[0] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point2;
	}
	else if (p0p3u <= 0 && p1p3u <= 0 && p2p3u <= 0)
	{
		closestPoint = p3;
		newSize = 1;
		newIndices[0] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Point3;
	}
	// edge regions
	else if (p0p1u > 0 && p0p1v > 0 && p013w < 0 && p012w < 0)
	{
		closestPoint = p0p1u * p0 + p0p1v * p1;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 1;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge01;
	}
	else if (p0p2u > 0 && p0p2v > 0 && p012v < 0 && p023w < 0)
	{
		closestPoint = p0p2u * p0 + p0p2v * p2;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge02;
	}
	else if (p0p3u > 0 && p0p3v > 0 && p013v < 0 && p023v < 0)
	{
		closestPoint = p0p3u * p0 + p0p3v * p3;
		newSize = 2;
		newIndices[0] = 0;
		newIndices[1] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge03;
	}
	else if (p1p2u > 0 && p1p2v > 0 && p012u < 0 && p123w < 0)
	{
		closestPoint = p1p2u * p1 + p1p2v * p2;
		newSize = 2;
		newIndices[0] = 1;
		newIndices[1] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge12;
	}
	else if (p1p3u > 0 && p1p3v > 0 && p013u < 0 && p123v < 0)
	{
		closestPoint = p1p3u * p1 + p1p3v * p3;
		newSize = 2;
		newIndices[0] = 1;
		newIndices[1] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge13;
	}
	else if (p2p3u > 0 && p2p3v > 0 && p023u < 0 && p123u < 0)
	{
		closestPoint = p2p3u * p2 + p2p3v * p3;
		newSize = 2;
		newIndices[0] = 2;
		newIndices[1] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Edge23;
	}
	// triangle region
	else if (p012u > 0 && p012v > 0 && p012w > 0 && TriangleSideCheck(q, p0, p1, p2, p3) >= 0)
	{
		closestPoint = p012u * p0 + p012v * p1 + p012w * p2;
		newSize = 3;
		newIndices[0] = 0;
		newIndices[1] = 1;
		newIndices[2] = 2;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Triangle012;
	}
	else if (p013u > 0 && p013v > 0 && p013w > 0 && TriangleSideCheck(q, p0, p1, p3, p2) >= 0)
	{
		closestPoint = p013u * p0 + p013v * p1 + p013w * p3;
		newSize = 3;
		newIndices[0] = 0;
		newIndices[1] = 1;
		newIndices[2] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Triangle013;
	}
	else if (p023u > 0 && p023v > 0 && p023w > 0 && TriangleSideCheck(q, p0, p2, p3, p1) >= 0)
	{
		closestPoint = p023u * p0 + p023v * p2 + p023w * p3;
		newSize = 3;
		newIndices[0] = 0;
		newIndices[1] = 2;
		newIndices[2] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Triangle023;
	}
	else if (p123u > 0 && p123v > 0 && p123w > 0 && TriangleSideCheck(q, p1, p2, p3, p0) >= 0)
	{
		closestPoint = p123u * p1 + p123v * p2 + p123w * p3;
		newSize = 3;
		newIndices[0] = 1;
		newIndices[1] = 2;
		newIndices[2] = 3;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Triangle123;
	}
	// Tetrahedra
	else
	{
		closestPoint = q;
		newSize = 4;
		for (int i = 0; i < 4; i++)
			newIndices[i] = i;
		searchDirection = q - closestPoint;

		return VoronoiRegion::Tetrahedra0123;
	}
}

bool Gjk::CsoPoint::operator==(const CsoPoint rhs)
{
	return m_CsoPoint == rhs.m_CsoPoint;
}

Gjk::CsoTriangle::CsoTriangle(const CsoPoint& a, const CsoPoint& b, const CsoPoint& c) : m_A(a), m_B(b), m_C(c)
{
	const Vector3 ab = b.m_CsoPoint - a.m_CsoPoint;
	ab.Cross(c.m_CsoPoint - a.m_CsoPoint, m_normal);
	m_normal.Normalize();
}



Gjk::CsoEdge::CsoEdge(const CsoPoint& a, const CsoPoint& b) : m_A(a), m_B(b)
{
}

Gjk::Gjk()
{
}

bool Gjk::Intersect(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, CsoPoint& closestPoint, float epsilon/*, int debuggingIndex, bool debugDraw*/)
{
	Vector3 q(0.0f, 0.0f, 0.0f);
	// 1. Initialize the simplex(to one point for us) by searching 
	//    in a random direction(difference of centers)

	Vector3 direction = shapeA->GetCenter() - shapeB->GetCenter();
	if (direction == Vector3(0, 0, 0))
		direction = Vector3(-1, 0, 0);
	simplex.push_back(ComputeSupport(shapeA, shapeB, direction));
	direction = q - simplex[0].m_CsoPoint;
	unsigned iteration = 1;
	while (iteration <= m_maxIterations)
	{
		// TODO: debug draw
		/*if (debugDraw && debuggingIndex == iteration)
		{
			PointsSupportShape shape;
			for (auto it : simplex)
			{
				shape.mLocalSpacePoints.push_back(it.mCsoPoint);
			}
			shape.DebugDraw(Math::Vector4(0, 1, 0, 1));
		}*/
		// 2. Determine which voronoi region Q is in and reduce to the smallest simplex
		// 3. Compute P(closestPoint.mCsoPoint) by projecting Q onto the new simplex
		size_t size = simplex.size();
		int indices[4] = { 0 };
		switch (size)
		{
		case 1:
			IdentifyVoronoiRegion(q, simplex[0].m_CsoPoint, size, indices, closestPoint.m_CsoPoint, direction);
			break;
		case 2:
			IdentifyVoronoiRegion(q, simplex[0].m_CsoPoint, simplex[1].m_CsoPoint, size, indices, closestPoint.m_CsoPoint, direction);
			break;
		case 3:
			IdentifyVoronoiRegion(q, simplex[0].m_CsoPoint, simplex[1].m_CsoPoint, simplex[2].m_CsoPoint, size, indices, closestPoint.m_CsoPoint, direction);
			break;
		case 4:
			IdentifyVoronoiRegion(q, simplex[0].m_CsoPoint, simplex[1].m_CsoPoint, simplex[2].m_CsoPoint, simplex[3].m_CsoPoint, size, indices, closestPoint.m_CsoPoint, direction);
			break;
		default:
			VoronoiRegion::Unknown;
			break;
		}


		std::vector<CsoPoint> newSimplex;
		for (size_t i = 0; i < size; i++)
		{
			newSimplex.push_back(simplex[indices[i]]);
		}
		simplex = newSimplex;

		// 4. If P is equal to Q then terminate
		if (q == closestPoint.m_CsoPoint)
		{
			if (simplex.size() < 4)
			{
				direction.Normalize();
				Vector3 perpendicular = direction.Cross(Vector3(0.0, 0.0, 1.0));
				CsoPoint newPoint = ComputeSupport(shapeA, shapeB, perpendicular);
				simplex.push_back(newPoint);

				if (simplex.size() < 4)
				{
					perpendicular *= -1.0f;
					CsoPoint newPoint = ComputeSupport(shapeA, shapeB, perpendicular);
					simplex.push_back(newPoint);
				}
			}
			return true;
		}

		direction.Normalize();
		// 5. Compute the new search direction(Q - P) and search for new point
		CsoPoint newPoint = ComputeSupport(shapeA, shapeB, direction);
		// 6. If new point is no further than P in the search direction then
		//    terminate. The length of the vector(Q - P) is the separation distance
		
		if (direction.Dot(newPoint.m_CsoPoint - closestPoint.m_CsoPoint) <= epsilon)//Math::Length(q - closestPoint.mCsoPoint) <= epsilon)
		{
			break;
		}
		// 7. Add the new point to the simplex and go to 2.
		simplex.push_back(newPoint);
		++iteration;
	}

	// computing points in original shapes 
	float u, v, w;
	switch (simplex.size())
	{
	case 1:
		closestPoint.m_PointA = simplex[0].m_PointA;
		closestPoint.m_PointB = simplex[0].m_PointB;
		break;
	case 2:
		PhysicsUtils::BarycentricCoordinates(closestPoint.m_CsoPoint, simplex[0].m_CsoPoint, simplex[1].m_CsoPoint, u, v);
		closestPoint.m_PointA = u * simplex[0].m_PointA + v * simplex[1].m_PointA;
		closestPoint.m_PointB = u * simplex[0].m_PointB + v * simplex[1].m_PointB;
		break;
	case 3:
		PhysicsUtils::BarycentricCoordinates(closestPoint.m_CsoPoint, simplex[0].m_CsoPoint, simplex[1].m_CsoPoint, simplex[2].m_CsoPoint, u, v, w);
		closestPoint.m_PointA = u * simplex[0].m_PointA + v * simplex[1].m_PointA + w * simplex[2].m_PointA;
		closestPoint.m_PointB = u * simplex[0].m_PointB + v * simplex[1].m_PointB + w * simplex[2].m_PointB;
		break;
	}
	//if (debugDraw)
	//{
	//	const Vector4 red(1.0f, 0.0f, 0.0f, 1.0f);
	//	Graphics::Get().AddLine(closestPoint.mPointA, closestPoint.mPointB, red, red);
	//}
	return false;
}

Gjk::CsoPoint Gjk::ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB, const Vector3& direction)
{
	CsoPoint result;
	result.m_PointA = shapeA->Support(direction);
	result.m_PointB = shapeB->Support(-direction);
	result.m_CsoPoint = result.m_PointA - result.m_PointB;

	return result;
}

float Gjk::TriangleSideCheck(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
{
	const Vector3 p0p1 = p1 - p0;
	Vector3 normal;
	p0p1.Cross(p2 - p0, normal);
	if (normal.Dot(p3 - p0) > 0)
	{
		normal *= -1;
	}
	return normal.Dot(q - p0);
}

bool Gjk::Epa(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, CollisionManifold& collision, float epsilon)
{
	//Expanding Polytope Algorithm (EPA) as described here: http://hacktank.net/blog/?p=119
	const unsigned MAX_ITERATIONS = 5;
	unsigned iteration = 0;

	std::list<CsoTriangle> triangles;
	std::list<CsoEdge> edges;

	// add the GJK simplex triangles to the list
	triangles.emplace_back(simplex[0], simplex[1], simplex[2]);
	triangles.emplace_back(simplex[0], simplex[2], simplex[3]);
	triangles.emplace_back(simplex[0], simplex[3], simplex[1]);
	triangles.emplace_back(simplex[1], simplex[3], simplex[2]);

	while (true)
	{
		if (iteration++ >= MAX_ITERATIONS)
		{
			return false;
		}

		// find closest triangle to origin
		std::list<CsoTriangle>::iterator currentTriangleIterator = triangles.begin();
		float currentDist = FLT_MAX;
		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			float dst = fabs(it->m_normal.Dot(it->points[0].m_CsoPoint));
			if (dst < currentDist)
			{
				currentDist = dst;
				currentTriangleIterator = it;
			}
		}

		// get the next support point in front of the current triangle, away from the origin
		const CsoPoint currentSuport = ComputeSupport(shapeA, shapeB, currentTriangleIterator->m_normal);

		// checks how much further this new point will takes us from the origin
		// if it is not far enough then we assume we have found the closest triangle
		// on the hull from the origin
		if (currentTriangleIterator->m_normal.Dot(currentSuport.m_CsoPoint) - currentDist < epsilon)
		{
			// I am storing an iterator to the closest triangle to the origin
			// rather than copying it for efficiency (currentTriangleIterator)
			// currentSuport is the new support point in the direction of
			// the current triangle's normal, epsilon is needed for shapes
			// with infinite points like a sphere, as we will never find the absolute
			// furthest triangle

			// calculate the barycentric coordinates of the closest triangle with respect to
			// the projection of the origin onto the triangle
			float u, v, w;
			PhysicsUtils::BarycentricCoordinates(currentTriangleIterator->m_normal * currentDist,
												 currentTriangleIterator->m_A.m_CsoPoint,
												 currentTriangleIterator->m_B.m_CsoPoint,
												 currentTriangleIterator->m_C.m_CsoPoint,
												 u, v, w);

			// collision point on object a in world space
			collision.m_pA = Vector3(u * currentTriangleIterator->m_A.m_PointA +
				v * currentTriangleIterator->m_B.m_PointA +
				w * currentTriangleIterator->m_C.m_PointA);
			collision.m_pB = collision.m_pA - currentTriangleIterator->m_normal * currentDist;

			// collision normal
			collision.m_normal = currentTriangleIterator->m_normal;

			// penetration depth
			collision.m_depth = currentDist;

			/*if (PhysicsSpace::Globals::EpaGlobals::isDebugDraw)
			{
				Graphics::Get().AddLine(currentTriangleIterator->mA.mPointA, currentTriangleIterator->mB.mPointA, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));
				Graphics::Get().AddLine(currentTriangleIterator->mA.mPointA, currentTriangleIterator->mC.mPointA, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));
				Graphics::Get().AddLine(currentTriangleIterator->mB.mPointA, currentTriangleIterator->mC.mPointA, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));

				Graphics::Get().AddLine(currentTriangleIterator->mA.mPointB, currentTriangleIterator->mB.mPointB, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));
				Graphics::Get().AddLine(currentTriangleIterator->mA.mPointB, currentTriangleIterator->mC.mPointB, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));
				Graphics::Get().AddLine(currentTriangleIterator->mB.mPointB, currentTriangleIterator->mC.mPointB, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));

				Graphics::Get().AddLine(collision.pA, collision.pA + collision.depth * collision.n, Math::Vector4(1, 0, 0, 1), Math::Vector4(1, 0, 0, 1));
				Graphics::Get().AddLine(collision.pA, collision.pB, Math::Vector4(0, 1, 0, 1), Math::Vector4(0, 1, 0, 1));
			}*/

			break;
		}

		for (auto it = triangles.begin(); it != triangles.end();) {
			// can this face be 'seen' by currentSuport?
			if (it->m_normal.Dot(currentSuport.m_CsoPoint - it->points[0].m_CsoPoint) > 0) {
				AddEdge(edges, it->points[0], it->points[1]);
				AddEdge(edges, it->points[1], it->points[2]);
				AddEdge(edges, it->points[2], it->points[0]);
				it = triangles.erase(it);
				continue;
			}
			it++;
		}

		// create new triangles from the edges in the edge list
		for (auto it = edges.begin(); it != edges.end(); it++) {
			triangles.emplace_back(currentSuport, it->m_points[0], it->m_points[1]);
		}

		edges.clear();
	}

	return true;
}

void Gjk::FindClosestFace(Triangle& closest, std::vector<Gjk::CsoPoint>& simplex)
{

}

void Gjk::AddEdge(std::list<CsoEdge>& list, const CsoPoint& a, const CsoPoint& b)
{
	for (auto it = list.begin(); it != list.end(); it++) {
		if (it->m_points[0] == b && it->m_points[1] == a) {
			//opposite edge found, remove it and do not add new one
			list.erase(it);
			return;
		}
	}
	list.emplace_back(a, b);
}

