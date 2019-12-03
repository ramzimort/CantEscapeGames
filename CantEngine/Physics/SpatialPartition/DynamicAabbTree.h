/**
 * @file DynamicAabbTree.h
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @brief Defenition of the dynamic aabb tree
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#pragma once

#include "../Geometry/Aabb.h"
#include "../Geometry/RayCant.h"
#include "SpatialPartitionData.h"
#include "QueryResults.h"
#include "CastResults.h"

class RayCant;
class AppRenderer;
class DynamicAabbTree;
/**
 * @brief Defenition of a node in the Danmic Aabb tree
 * 
 */
class DynamicAabbTreeNode
{
	friend DynamicAabbTree;
public:
	DynamicAabbTreeNode();
	DynamicAabbTreeNode(unsigned int key, const SpatialPartitionData& data);

	/**
	 * @brief Get the Parent of this node
	 * 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* GetParent() const;
	/**
	 * @brief Get the Left Child of this node
	 * 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* GetLeftChild() const;
	/**
	 * @brief Get the Right Child of this node
	 * 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* GetRightChild() const;
	/**
	 * @brief Get the Aabb for this node
	 * 
	 * @return Aabb 
	 */
	Aabb GetAabb() const;
	/**
	 * @brief Get the Client Data associated with this node
	 * 
	 * @return void* 
	 */
	void* GetClientData() const;
	/**
	 * @brief Get the Height of this node inside of the tree
	 * 
	 * @return int 
	 */
	int GetHeight() const;
	/**
	 * @brief checks if this node is a leaf in the tree
	 * 
	 * @return true 
	 * @return false 
	 */
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


/**
 * @brief Defenition of the Dynamic Aabb tree
 * 
 */
class DynamicAabbTree
{
public:
	DynamicAabbTree();
	~DynamicAabbTree();
	/**
	 * @brief Destroys the tree
	 * 
	 */
	void Clear();
	/**
	 * @brief Adds new node into the tree
	 * 
	 * @param int 
	 * @param data 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* NewNode(unsigned int& key, const SpatialPartitionData& data);
	/**
	 * @brief Builds Aabb around two given aabb and computes its surface area
	 * 
	 * @param aabb1 
	 * @param aabb2 
	 * @return float 
	 */
	float SurfaceArea(const Aabb& aabb1, const Aabb& aabb2) const;
	/**
	 * @brief Selects the child which in combination with the newNodes aabb result in a smaller difference in the surface area of their combined aabb
	 * 
	 * @param newNode 
	 * @param leftNode 
	 * @param rightNode 
	 * @param isLeft 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* SelectNode(DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* leftNode, DynamicAabbTreeNode* rightNode, bool& isLeft) const;
	/**
	 * @brief Balances the tree
	 * 
	 * @param tree 
	 * @param isLeft 
	 */
	void BalanceTree(DynamicAabbTreeNode* tree, bool isLeft);
	/**
	 * @brief Creates a new Node and inserts it into the tree based on the data provided
	 * 
	 * @param int 
	 * @param data 
	 */
	void InsertData(unsigned int& key, const SpatialPartitionData& data);
	/**
	 * @brief Reinserts the node with the given key
	 * 
	 * @param int 
	 * @param data 
	 */
	void UpdateData(unsigned int& key, const SpatialPartitionData& data);
	/**
	 * @brief Removes the node with the give key from the tree
	 * 
	 * @param int 
	 */
	void RemoveData(unsigned int& key);
	/**
	 * @brief casts a ray agains the tree
	 * 
	 * @param ray 
	 * @param results 
	 */
	void CastRay(const RayCant& ray, CastResults& results);

#ifdef DEVELOPER
	void DebugDrawRec(AppRenderer* appRenderer, int level, const Vector4& color, DynamicAabbTreeNode* node, int currentLevel);
	void DebugDraw(AppRenderer* pAppRenderer, int level, const Vector4& color = Vector4(1));
#endif

/*
	void FrustumAddResultsRec(CastResults& results, DynamicAabbTreeNode* node) const;
	void CastFrustumRec(const Frustum& frustum, CastResults& results, DynamicAabbTreeNode* node) const;
	/*void CastFrustum(const Frustum& frustum, CastResults& results) override;
   */

	// helper functions
	void ClearRec(DynamicAabbTreeNode* node);
	void SplitNodes(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const;
	/**
	 * @brief Returns a query of all the colliding leafs in the tree
	 * 
	 * @param results 
	 */
	void SelfQuery(QueryResults& results);

	/**
	 * @brief Get the Root of the tree
	 * 
	 * @return DynamicAabbTreeNode* 
	 */
	DynamicAabbTreeNode* GetRoot() const;

private:
	DynamicAabbTreeNode* m_Root;
	unsigned int m_CurrentId;

	std::unordered_map<unsigned int, DynamicAabbTreeNode*> m_Data;
	
	int InsertDataRec(unsigned int& key, const SpatialPartitionData& data, DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* currentNode, bool is_left);
	void CastRayRec(const RayCant& ray, CastResults& results, DynamicAabbTreeNode* node) const;
	void SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* node) const;
	void SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const;
};
