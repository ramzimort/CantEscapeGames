/**
 * @file Gjk.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Narrow phase collision detection algorithms - GJK and EPA
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once
#include "Physics/SuppportShape/SupportShape.h"
#include "Physics/Geometry/Triangle.h"
#include "Physics/Constraint.h"

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

class Contact;

/**
 * @brief Narrow phase collision detection algorithm
 * 
 */
class Gjk
{
public:

	/**
	 * @brief Idenifies voronoi region of a point in respect to a point (this is for consistancy with the rest of the interface)
	 * 
	 * @param q 
	 * @param p0 
	 * @param newSize 
	 * @param newIndices 
	 * @param closestPoint 
	 * @param searchDirection 
	 * @return VoronoiRegion::Type 
	 */
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	/**
	 * @brief Identifies voronoi region of a point with respect to an edge
	 * 
	 * @param q 
	 * @param p0 
	 * @param p1 
	 * @param newSize 
	 * @param newIndices 
	 * @param closestPoint 
	 * @param searchDirection 
	 * @return VoronoiRegion::Type 
	 */
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	/**
	 * @brief Identifies voronoi region of a point with respect to a triangle
	 * 
	 * @param q 
	 * @param p0 
	 * @param p1 
	 * @param p2 
	 * @param newSize 
	 * @param newIndices 
	 * @param closestPoint 
	 * @param searchDirection 
	 * @return VoronoiRegion::Type 
	 */
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	/**
	 * @brief Identifies voronoi region of a point with respect to tetrahedron
	 * 
	 * @param q 
	 * @param p0 
	 * @param p1 
	 * @param p2 
	 * @param p3 
	 * @param newSize 
	 * @param newIndices 
	 * @param closestPoint 
	 * @param searchDirection 
	 * @return VoronoiRegion::Type 
	 */
	static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
		size_t& newSize, int newIndices[4],
		Vector3& closestPoint, Vector3& searchDirection);

	/**
	 * @brief representation of a point in Minkowski difference space
	 * 
	 */
	struct CsoPoint
	{
		Vector3 m_PointA;
		Vector3 m_PointB;
		Vector3 m_CsoPoint;
		/**
		 * @brief Checks if the point in minkowski difference space are the same 
		 * 
		 * @param rhs 
		 * @return true 
		 * @return false 
		 */
		bool operator==(const CsoPoint& rhs) const;
	};
	/**
	 * @brief Representation of a triangle in Minkowski difference space
	 * 
	 */
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
	/**
	 * @brief Representation of an edge in a Minkowski difference space
	 * 
	 */
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
	/**
	 * @brief Runs the GJK algorithm to see if two shapes are intersecting
	 * 
	 * @param simplex 
	 * @param shapeA 
	 * @param shapeB 
	 * @param closestPoint 
	 * @param epsilon 
	 * @param pAppRenderer 
	 * @param isDebugDraw 
	 * @return true 
	 * @return false 
	 */
	bool Intersect(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, CsoPoint& closestPoint, float epsilon, AppRenderer* pAppRenderer, bool isDebugDraw = false);
	/**
	 * @brief Finds the point furthest in the given direction on the CSO (and the relevant points from each object)
	 * 
	 * @param shapeA 
	 * @param shapeB 
	 * @param direction 
	 * @return CsoPoint 
	 */
	CsoPoint ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB, const Vector3& direction);
	/**
	 * @brief helper function that determines on which side the point is in respect to a triangle in a tetrahedron
	 * 
	 * @param q 
	 * @param p0 
	 * @param p1 
	 * @param p2 
	 * @param p3 
	 * @return float 
	 */
	static float TriangleSideCheck(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);
	/**
	* @brief Calculates collision information for the collision. This function should be called if Intersection function returns true 
	* 
	* @param simplex 
	* @param shapeA 
	* @param shapeB 
	* @param contact 
	* @param pAppRenderer 
	* @param epsilon 
	* @return true 
	* @return false 
	 */
	bool Epa(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB, Contact& contact, AppRenderer* pAppRenderer, float epsilon);
	
private:
	// process the specified edge, if another edge with the same points in the
	// opposite order exists then it is removed and the new point is also not added
	// this ensures only the outermost ring edges of a cluster of triangles remain
	// in the list
	void AddEdge(std::list<CsoEdge>& list, const CsoPoint& a, const CsoPoint& b);

	const unsigned int m_maxIterations = 100;

	void FillSimplexInDirection(std::vector<Gjk::CsoPoint>& simplex, const Vector3& direction, const SupportShape* shapeA, const SupportShape* shapeB);

	void FillSimplex(std::vector<Gjk::CsoPoint>& simplex, const SupportShape* shapeA, const SupportShape* shapeB);
};
