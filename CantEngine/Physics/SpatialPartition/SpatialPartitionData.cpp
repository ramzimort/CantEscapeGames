/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

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