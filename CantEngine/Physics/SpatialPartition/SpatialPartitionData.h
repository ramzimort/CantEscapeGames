/**
 * @file SpatialPartitioData.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of spatial partitioning data, dynamic aabb will be populated with this 
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */


#pragma once

#include "../Geometry/Aabb.h"

/**
 * @brief defenition of the spatial partitioning data
 * 
 */
class SpatialPartitionData
{
public:
	SpatialPartitionData();
	SpatialPartitionData(void* clientData, const Aabb& aabb);
	/**
	 * @brief compares clients data by simply checking their address
	 * 
	 * @param rhs 
	 * @return true 
	 * @return false 
	 */
	bool operator<(const SpatialPartitionData& rhs) const;

	// data that user wants to get back when querying the spatial partition. For example: rigidbody*.
	void* m_ClientData;
	Aabb m_Aabb;
};