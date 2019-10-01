/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Aleksey Perfilev
Other Authors :
-End Header--------------------------------------------------------*/

#include "DynamicAabbTree.h"
#include "../PhysicsUtils.h"
#include "../Geometry/Intersection.h"

/////////////////////////// DynamicAabbTreeNode /////////////////////////// 
#pragma region DynamicAabbTreeNode
DynamicAabbTreeNode::DynamicAabbTreeNode() : m_ClientData(nullptr), m_Left(nullptr), m_Right(nullptr), m_Parent(nullptr), m_Height(0), m_Id(0), m_LastAxis(0) {}
DynamicAabbTreeNode::DynamicAabbTreeNode(unsigned int key, const SpatialPartitionData& data) : m_Aabb(data.m_Aabb), m_ClientData(data.m_ClientData), m_Left(nullptr), m_Right(nullptr), m_Parent(nullptr), m_Height(0), m_Id(key), m_LastAxis(0)
{
	m_Aabb = m_Aabb.BuildFromCenterAndHalfExtents(m_Aabb.GetCenter(), m_Aabb.GetHalfExtent() * PhysicsUtils::Consts::fatteningFactor);
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetParent() const
{
	return m_Parent;
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetLeftChild() const
{
	return m_Left;
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetRightChild() const
{
	return m_Right;
}

Aabb DynamicAabbTreeNode::GetAabb() const
{
	return m_Aabb;
}

void* DynamicAabbTreeNode::GetClientData() const
{
	return m_ClientData;
}

int DynamicAabbTreeNode::GetHeight() const
{
	return m_Height;
}


bool DynamicAabbTreeNode::IsLeaf() const
{
	return (m_Left == nullptr) && (m_Right == nullptr) ? true : false;
}
#pragma endregion


///////////////////////////// DynamicAabbTree ///////////////////////
#pragma region DynamicAabbTree
DynamicAabbTree::DynamicAabbTree() : m_Root(nullptr), m_CurrentId(0)
{
}

void DynamicAabbTree::ClearRec(DynamicAabbTreeNode* node)
{
	if (node->IsLeaf())
	{
		delete node;
		return;
	}

	ClearRec(node->m_Left);
	ClearRec(node->m_Right);

	delete node;
}

DynamicAabbTree::~DynamicAabbTree()
{
	Clear();
}

void DynamicAabbTree::Clear()
{
	if (m_Root)
	{
		ClearRec(m_Root);
	}
	m_Data.clear();
	m_Root = nullptr;
}

DynamicAabbTreeNode* DynamicAabbTree::NewNode(unsigned int& key, const SpatialPartitionData& data)
{
	key = m_CurrentId;
	DynamicAabbTreeNode* newNode = new DynamicAabbTreeNode(key, data);
	m_Data[m_CurrentId] = newNode;
	++m_CurrentId;
	return newNode;
}

float DynamicAabbTree::SurfaceArea(const Aabb& aabb1, const Aabb& aabb2) const
{
	return Aabb::Combine(aabb1, aabb2).GetSurfaceArea();
}

DynamicAabbTreeNode* DynamicAabbTree::SelectNode(DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* leftNode, DynamicAabbTreeNode* rightNode, bool& isLeft) const
{
	if (SurfaceArea(newNode->m_Aabb, leftNode->m_Aabb) - leftNode->m_Aabb.GetSurfaceArea() <
		SurfaceArea(newNode->m_Aabb, rightNode->m_Aabb) - rightNode->m_Aabb.GetSurfaceArea())
	{
		isLeft = true;
		return leftNode;
	}
	isLeft = false;
	return rightNode;
}

void DynamicAabbTree::BalanceTree(DynamicAabbTreeNode* tree, bool isLeft)
{
	const int heightDiference = tree->m_Right->GetHeight() - tree->m_Left->GetHeight();

	// already balanced?
	if (PhysicsUtils::Abs(heightDiference) < 2)
		return;

	// identifying pivot node
	DynamicAabbTreeNode* pivotNode;
	if (heightDiference < 0) // left side is higher
	{
		pivotNode = tree->m_Left;
		tree->m_Left = nullptr;
	}
	else                     // right side is higher
	{
		pivotNode = tree->m_Right;
		tree->m_Right = nullptr;
	}

	// ataching pivot to grandnode
	DynamicAabbTreeNode* granny = tree->m_Parent;
	if (granny == nullptr)
	{
		m_Root = pivotNode;
		pivotNode->m_Parent = nullptr;
	}
	else
	{
		if (isLeft)
		{
			granny->m_Left = pivotNode;
		}
		else // right
		{
			granny->m_Right = pivotNode;
		}
		pivotNode->m_Parent = granny;
	}

	// identifying small and large children of the pivot node and ataching old parent to the pivot
	DynamicAabbTreeNode* smallChild;
	if (pivotNode->m_Left->GetHeight() < pivotNode->m_Right->GetHeight())
	{
		smallChild = pivotNode->m_Left;
		pivotNode->m_Left = tree;
		tree->m_Parent = pivotNode;
	}
	else
	{
		smallChild = pivotNode->m_Right;
		pivotNode->m_Right = tree;
		tree->m_Parent = pivotNode;
	}

	// attaching back small child of pivot to the old parent
	if (tree->m_Left == nullptr)
	{
		tree->m_Left = smallChild;
		smallChild->m_Parent = tree;
	}
	else
	{
		tree->m_Right = smallChild;
		smallChild->m_Parent = tree;
	}

	// update heights
	tree->m_Height = PhysicsUtils::Max(tree->m_Left->m_Height, tree->m_Right->m_Height) + 1;
	tree->m_Aabb = Aabb::Combine(tree->m_Left->m_Aabb, tree->m_Right->m_Aabb);
	pivotNode->m_Height = PhysicsUtils::Max(pivotNode->m_Left->m_Height, pivotNode->m_Right->m_Height) + 1;
	pivotNode->m_Aabb = Aabb::Combine(pivotNode->m_Left->m_Aabb, pivotNode->m_Right->m_Aabb);
}

int DynamicAabbTree::InsertDataRec(unsigned int& key, const SpatialPartitionData& data, DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* currentNode, bool is_left)
{
	// did we reach the bottom
	if (currentNode->GetHeight() == 0)
	{
		//SpatialPartitionData newParentData;
		DynamicAabbTreeNode* newParent = new DynamicAabbTreeNode(static_cast<unsigned int>(-1), SpatialPartitionData());

		// updating links
		newParent->m_Left = currentNode;
		newParent->m_Right = newNode;
		newParent->m_Parent = currentNode->m_Parent;

		newNode->m_Parent = newParent;
		currentNode->m_Parent = newParent;

		// updating data
		newParent->m_Height = currentNode->m_Height + 1;
		newParent->m_Aabb = Aabb::Combine(currentNode->m_Aabb, newNode->m_Aabb);

		if (newParent->m_Parent == nullptr)
		{
			m_Root = newParent;
		}
		else
		{
			if (is_left)
			{
				newParent->m_Parent->m_Left = newParent;
			}
			else
			{
				newParent->m_Parent->m_Right = newParent;
			}
		}
		return newParent->m_Height;
	}
	// not the bottom yet, keep going deeper
	bool isLeftChild;
	DynamicAabbTreeNode* nextNode = SelectNode(newNode, currentNode->m_Left, currentNode->m_Right, isLeftChild);
	currentNode->m_Height = PhysicsUtils::Max(currentNode->m_Height, 1 + InsertDataRec(key, data, newNode, nextNode, isLeftChild));
	currentNode->m_Aabb = Aabb::Combine(currentNode->m_Left->m_Aabb, currentNode->m_Right->m_Aabb);
	BalanceTree(currentNode, is_left);
	return currentNode->m_Height;

}

void DynamicAabbTree::InsertData(unsigned int& key, const SpatialPartitionData& data)
{
	DynamicAabbTreeNode* newNode = NewNode(key, data);
	if (m_Root == nullptr)
	{
		m_Root = newNode;
		return;
	}

	InsertDataRec(key, data, newNode, m_Root, true);
}

void DynamicAabbTree::UpdateData(unsigned int& key, const SpatialPartitionData& data)
{
	if (m_Data.count(key) == 0)
		return;

	DynamicAabbTreeNode* nodeToCheck = m_Data.at(key);

	if (nodeToCheck->m_Aabb.Contains(data.m_Aabb))
		return;

	RemoveData(key);
	InsertData(key, data);
}

void DynamicAabbTree::RemoveData(unsigned int& key)
{
	// invalid key?
	if (m_Data.count(key) == 0)
		return;

	DynamicAabbTreeNode* nodeToDel = m_Data.at(key);

	DynamicAabbTreeNode* parent = nodeToDel->m_Parent;
	if (parent == nullptr)
	{
		// TODO: check for existance of children
		delete nodeToDel;
		m_Root = nullptr;
		m_Data.clear();
		return;
	}

	DynamicAabbTreeNode* nodeToUpdate;
	if (static_cast<void*>(parent->m_Right) == static_cast<void*>(nodeToDel))
	{
		nodeToUpdate = parent->m_Left;
	}
	else
	{
		nodeToUpdate = parent->m_Right;
	}

	DynamicAabbTreeNode* granny = parent->m_Parent;
	if (granny == nullptr)
	{
		m_Root = nodeToUpdate;
		nodeToUpdate->m_Parent = nullptr;
	}
	else
	{
		if (static_cast<void*>(granny->m_Right) == static_cast<void*>(parent))
		{
			granny->m_Right = nodeToUpdate;
		}
		else
		{
			granny->m_Left = nodeToUpdate;
		}
		nodeToUpdate->m_Parent = granny;
	}
	m_Data.erase(nodeToDel->m_Id);
	delete nodeToDel;
	delete parent;

	nodeToUpdate = nodeToUpdate->m_Parent;
	while (nodeToUpdate)
	{
		nodeToUpdate->m_Height = PhysicsUtils::Max(nodeToUpdate->m_Left->m_Height, nodeToUpdate->m_Right->m_Height) + 1;
		parent = nodeToUpdate->m_Parent;

		bool nodeOnTheLeft = true;
		if (parent != nullptr)
		{
			if (static_cast<void*>(parent->m_Right) == static_cast<void*>(nodeToUpdate))
			{
				nodeOnTheLeft = false;
			}
		}
		BalanceTree(nodeToUpdate, nodeOnTheLeft);
		nodeToUpdate->m_Aabb = Aabb::Combine(nodeToUpdate->m_Left->m_Aabb, nodeToUpdate->m_Right->m_Aabb);
		nodeToUpdate = nodeToUpdate->m_Parent;
	}
}

#ifdef DEVELOPER
void DynamicAabbTree::DebugDrawRec(AppRenderer* pAppRenderer, int level, const Vector4& color, DynamicAabbTreeNode* node, int currentLevel)
{
	if (node == nullptr)
		return;

	// to far down in the tree
	if (level < currentLevel && level != -1)
	{
		return;
	}

	if (currentLevel == level || level == -1)
	{
		node->m_Aabb.DebugDraw(pAppRenderer, color);
	}

	if (currentLevel < level || level == -1)
	{
		DebugDrawRec(pAppRenderer, level, color, node->m_Left, currentLevel + 1);
		DebugDrawRec(pAppRenderer, level, color, node->m_Right, currentLevel + 1);
	}
}

void DynamicAabbTree::DebugDraw(AppRenderer* pAppRenderer, int level, const Vector4& color)
{
	DebugDrawRec(pAppRenderer, level, color, m_Root, 0);
}
#endif

//void DynamicAabbTree::CastRayRec(const Ray& ray, RayCastResults& results, DynamicAabbTreeNode* node) const
//{
//	if (node == nullptr)
//		return;
//
//	float t;
//	if (Intersection::RayAabb(ray.start, ray.direction, node->m_Aabb.GetMin(), node->m_Aabb.GetMax(), t))
//	{
//		RayCastResult result;
//		result.mClientData = node->mClientData;
//		result.mTime = t;
//		if (node->mLeft == nullptr)
//			results.AddResult(result);
//		CastRayRec(ray, results, node->m_Left);
//		CastRayRec(ray, results, node->m_Right);
//	}
//}
//
//void DynamicAabbTree::CastRay(const Ray& ray, RayCastResults& results)
//{
//	CastRayRec(ray, results, mRoot);
//}

//void DynamicAabbTree::FrustumAddResultsRec(CastResults& results, DynamicAabbTreeNode* node) const
//{
//    if (node == nullptr)
//        return;
//
//    if (node->mLeft == nullptr)
//    {
//        CastResult result;
//        result.mClientData = node->mClientData;
//        results.AddResult(result);
//    }
//    FrustumAddResultsRec(results, node->m_Left);
//    FrustumAddResultsRec(results, node->m_Right);
//}
//
//void DynamicAabbTree::CastFrustumRec(const Frustum& frustum, CastResults& results, DynamicAabbTreeNode* node) const
//{
//    if (node == nullptr)
//        return;
//
//    const unsigned test = FrustumAabb(frustum.GetPlanes(), node->m_Aabb.GetMin(), node->m_Aabb.GetMax(), node->mLastAxis);
//    if (Intersection::IntersectionType::Outside == test)
//        return;
//
//    if (Intersection::IntersectionType::Inside == test)
//    {
//        FrustumAddResultsRec(results, node);
//        return;
//    }
//
//    if (node->mLeft == nullptr)
//    {
//        CastResult result;
//        result.mClientData = node->mClientData;
//        results.AddResult(result);
//    }
//    CastFrustumRec(frustum, results, node->mLeft);
//    CastFrustumRec(frustum, results, node->mRight);
//}
//
//void DynamicAabbTree::CastFrustum(const Frustum& frustum, CastResults& results)
//{
//    CastFrustumRec(frustum, results, mRoot);
//}

void DynamicAabbTree::SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* node) const
{
	if (node == nullptr || node->m_Left == nullptr)
	{
		return;
	}

	SelfQueryRec(results, node->m_Left, node->m_Right);
	SelfQueryRec(results, node->m_Left);
	SelfQueryRec(results, node->m_Right);
}

void DynamicAabbTree::SplitNodes(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const
{
	if (right->GetAabb().GetVolume() < left->GetAabb().GetVolume())
	{
		SelfQueryRec(results, left->m_Left, right);
		SelfQueryRec(results, left->m_Right, right);
	}
	else
	{
		SelfQueryRec(results, left, right->m_Left);
		SelfQueryRec(results, left, right->m_Right);
	}
}

void DynamicAabbTree::SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const
{
	bool isColliding = Intersection::AabbAabb(left->m_Aabb.GetMin(), left->m_Aabb.GetMax(), right->m_Aabb.GetMin(), right->m_Aabb.GetMax());
	if (!isColliding)
		return;
	// both are leafs
	if (left->IsLeaf() && right->IsLeaf())
	{
		results.AddResult(QueryResult(left->m_ClientData, right->m_ClientData));
	}// one internal one leaf
	else if (left->IsLeaf())
	{
		SelfQueryRec(results, left, right->m_Left);
		SelfQueryRec(results, left, right->m_Right);
	}
	else if (right->IsLeaf())
	{
		SelfQueryRec(results, left->m_Left, right);
		SelfQueryRec(results, left->m_Right, right);
	}// Both are internal, split the nodes
	else
	{
		SplitNodes(results, left, right);
	}
}

void DynamicAabbTree::SelfQuery(QueryResults& results)
{
	SelfQueryRec(results, m_Root);
}

DynamicAabbTreeNode* DynamicAabbTree::GetRoot() const
{
	// Return the root of your tree so that unit tests can print out the contents
	return m_Root;
}
#pragma endregion