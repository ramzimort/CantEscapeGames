/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "../Geometry/Aabb.h"

class SpatialPartitionData
{
public:
	SpatialPartitionData();
	SpatialPartitionData(void* clientData, const Aabb& aabb);

	bool operator<(const SpatialPartitionData& rhs) const;

	// Some data that the user wants to get back when querying the spatial partition.
	void* mClientData;
	Aabb mAabb;
};