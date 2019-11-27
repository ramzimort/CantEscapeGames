/**
 * @file SpatialPartitioData.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#include "SpatialPartitionData.h"

SpatialPartitionData::SpatialPartitionData() : m_ClientData(nullptr)
{
}

SpatialPartitionData::SpatialPartitionData(void* clientData, const Aabb& aabb) : m_ClientData(clientData), m_Aabb(aabb)
{
}

bool SpatialPartitionData::operator<(const SpatialPartitionData& rhs) const
{
	return m_ClientData < rhs.m_ClientData;
}