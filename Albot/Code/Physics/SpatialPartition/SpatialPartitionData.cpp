/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "SpatialPartitionData.h"

SpatialPartitionData::SpatialPartitionData() : mClientData(nullptr)
{
}

SpatialPartitionData::SpatialPartitionData(void* clientData, const Aabb& aabb) : mClientData(clientData), mAabb(aabb)
{
}

bool SpatialPartitionData::operator<(const SpatialPartitionData& rhs) const
{
	return mClientData < rhs.mClientData;
}