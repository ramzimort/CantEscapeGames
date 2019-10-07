/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#pragma once

#include "../Geometry/Aabb.h"
#include "../Geometry/RayCant.h"
#include "SpatialPartitionData.h"
#include "QueryResults.h"
#include "CastResults.h"

class RayCant;
class AppRenderer;
class DynamicAabbTree;
class DynamicAabbTreeNode
{
	friend DynamicAabbTree;
public:
	DynamicAabbTreeNode();
	DynamicAabbTreeNode(unsigned int key, const SpatialPartitionData& data);

	DynamicAabbTreeNode* GetParent() const;
	DynamicAabbTreeNode* GetLeftChild() const;
	DynamicAabbTreeNode* GetRightChild() const;

	Aabb GetAabb() const;
	void* GetClientData() const;
	int GetHeight() const;

	bool IsLeaf() const;

private:
	Aabb m_Aabb;
	void* m_ClientData;

	DynamicAabbTreeNode* m_Left;
	DynamicAabbTreeNode* m_Right;
	DynamicAabbTreeNode* m_Parent;
	int m_Height;
	unsigned int m_Id;

	size_t m_LastAxis;
};


////////// DynamicAabbTree ////////////
class DynamicAabbTree
{
public:
	DynamicAabbTree();
	~DynamicAabbTree();

	void Clear();

	DynamicAabbTreeNode* NewNode(unsigned int& key, const SpatialPartitionData& data);
	float SurfaceArea(const Aabb& aabb1, const Aabb& aabb2) const;
	DynamicAabbTreeNode* SelectNode(DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* leftNode, DynamicAabbTreeNode* rightNode, bool& isLeft) const;
	void BalanceTree(DynamicAabbTreeNode* tree, bool isLeft);
	int InsertDataRec(unsigned int& key, const SpatialPartitionData& data, DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* currentNode, bool is_left);

	// Spatial Partition Interface
	void InsertData(unsigned int& key, const SpatialPartitionData& data);
	void UpdateData(unsigned int& key, const SpatialPartitionData& data);
	void RemoveData(unsigned int& key);
	void CastRay(const RayCant& ray, CastResults& results);

#ifdef DEVELOPER
	void DebugDrawRec(AppRenderer* appRenderer, int level, const Vector4& color, DynamicAabbTreeNode* node, int currentLevel);
	void DebugDraw(AppRenderer* pAppRenderer, int level, const Vector4& color = Vector4(1));
#endif

	void CastRayRec(const RayCant& ray, CastResults& results, DynamicAabbTreeNode* node) const;
/*
	void FrustumAddResultsRec(CastResults& results, DynamicAabbTreeNode* node) const;
	void CastFrustumRec(const Frustum& frustum, CastResults& results, DynamicAabbTreeNode* node) const;
	/*void CastFrustum(const Frustum& frustum, CastResults& results) override;
   */

	void ClearRec(DynamicAabbTreeNode* node);

	void SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* node) const;
	void SplitNodes(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const;
	void SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const;
	void SelfQuery(QueryResults& results);

	DynamicAabbTreeNode* GetRoot() const;

private:
	DynamicAabbTreeNode* m_Root;
	unsigned int m_CurrentId;

	std::unordered_map<unsigned int, DynamicAabbTreeNode*> m_Data;
};
