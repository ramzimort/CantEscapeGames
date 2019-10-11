/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Aleksey Perfilev
- End Header --------------------------------------------------------*/

#pragma once
#include "Physics/SuppportShape/SupportShape.h"
#include "Physics/Geometry/Triangle.h"
#include "CollisionManifold.h"

namespace VoronoiRegion
{
	enum Type {
		Point0, Point1, Point2, Point3,
		Edge01, Edge02, Edge03, Edge12, Edge13, Edge23,
		Triangle012, Triangle013, Triangle023, Triangle123,
		Tetrahedra0123,
		Unknown
	};
}
namespace Framework
{
	class CollisionManifold;
}


class Gjk
{
public:

	// Point Test
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	// Edge Test
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	// Triangle Test
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	// Tetrahedron Tests
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	// Simple structure that contains all information for a point in Gjk.
	struct CsoPoint
	{
		Vector3 m_PointA;
		Vector3 m_PointB;
		Vector3 m_CsoPoint;

		bool operator==(const CsoPoint rhs);
	};

	struct CsoTriangle
	{
		union
		{
			struct
			{
				CsoPoint m_A, m_B, m_C;
			};
			CsoPoint points[3];
		};

		Vector3 m_normal; // normal

		CsoTriangle(const CsoPoint& a_, const CsoPoint& b_, const CsoPoint& c_);

	};

	struct CsoEdge
	{
		union
		{
			struct
			{
				CsoPoint m_A, m_B;
			};
			CsoPoint m_points[2];
		};
		CsoEdge(const CsoPoint& a, const CsoPoint& b);
	};

	Gjk();

	// Returns true if the shapes intersect. If the shapes don't intersect then closestPoint is filled out with the closest points
	// on each object as well as the cso point. Epsilon should be used for checking if sufficient progress has been made at any step.
	// The debugging values are for your own use (make sure they don't interfere with the unit tests).
	bool Intersect(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, CsoPoint& closestPoint, float epsilon, AppRenderer* pAppRenderer, bool isDebugDraw = false);

	// Finds the point furthest in the given direction on the CSO (and the relevant points from each object)
	CsoPoint ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB, const Vector3& direction);

	static float TriangleSideCheck(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);

	bool Epa(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, CollisionManifold& collision, float epsilon);
	void FindClosestFace(Triangle& result, std::vector<Gjk::CsoPoint>& simplex);

private:
	// process the specified edge, if another edge with the same points in the
	// opposite order exists then it is removed and the new point is also not added
	// this ensures only the outermost ring edges of a cluster of triangles remain
	// in the list
	void AddEdge(std::list<CsoEdge>& list, const CsoPoint& a, const CsoPoint& b);

	const unsigned int m_maxIterations = 100;

	void FillSimplexInDirection(std::vector<Gjk::CsoPoint>& simplex, const Vector3& direction, const SupportShape* shapeA, const SupportShape* shapeB);
};
