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
DynamicAabbTreeNode::DynamicAabbTreeNode() : mClientData(nullptr), mLeft(nullptr), mRight(nullptr), mParent(nullptr), mHeight(0), mId(0), mLastAxis(0) {}
DynamicAabbTreeNode::DynamicAabbTreeNode(unsigned int key, const SpatialPartitionData& data) : mAabb(data.mAabb), mClientData(data.mClientData), mLeft(nullptr), mRight(nullptr), mParent(nullptr), mHeight(0), mId(key), mLastAxis(0)
{
	mAabb = mAabb.BuildFromCenterAndHalfExtents(mAabb.GetCenter(), mAabb.GetHalfExtent() * PhysicsUtils::Consts::fatteningFactor);
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetParent() const
{
	return mParent;
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetLeftChild() const
{
	return mLeft;
}

DynamicAabbTreeNode* DynamicAabbTreeNode::GetRightChild() const
{
	return mRight;
}

Aabb DynamicAabbTreeNode::GetAabb() const
{
	return mAabb;
}

void* DynamicAabbTreeNode::GetClientData() const
{
	return mClientData;
}

int DynamicAabbTreeNode::GetHeight() const
{
	return mHeight;
}


bool DynamicAabbTreeNode::IsLeaf() const
{
	return (mLeft == nullptr) && (mRight == nullptr) ? true : false;
}
#pragma endregion


///////////////////////////// DynamicAabbTree ///////////////////////
#pragma region DynamicAabbTree
DynamicAabbTree::DynamicAabbTree() : mRoot(nullptr), mCurrentId(0)
{
}

void DynamicAabbTree::ClearRec(DynamicAabbTreeNode* node)
{
	if (node->IsLeaf())
	{
		delete node;
		//node = nullptr;
		return;
	}

	ClearRec(node->mLeft);
	ClearRec(node->mRight);

	delete node;
}

DynamicAabbTree::~DynamicAabbTree()
{
	Clear();
}

void DynamicAabbTree::Clear()
{
	if (mRoot)
	{
		ClearRec(mRoot);
	}
	mData.clear();
	mRoot = nullptr;
	// TODO: clear properly
}

DynamicAabbTreeNode* DynamicAabbTree::NewNode(unsigned int& key, const SpatialPartitionData& data)
{
	key = mCurrentId;
	DynamicAabbTreeNode* newNode = new DynamicAabbTreeNode(key, data);
	mData[mCurrentId] = newNode;
	++mCurrentId;
	return newNode;
}

float DynamicAabbTree::SurfaceArea(const Aabb& aabb1, const Aabb& aabb2) const
{
	return Aabb::Combine(aabb1, aabb2).GetSurfaceArea();
}

DynamicAabbTreeNode* DynamicAabbTree::SelectNode(DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* leftNode, DynamicAabbTreeNode* rightNode, bool& isLeft) const
{
	if (SurfaceArea(newNode->mAabb, leftNode->mAabb) - leftNode->mAabb.GetSurfaceArea() <
		SurfaceArea(newNode->mAabb, rightNode->mAabb) - rightNode->mAabb.GetSurfaceArea())
	{
		isLeft = true;
		return leftNode;
	} // else
	isLeft = false;
	return rightNode;
}

void DynamicAabbTree::BalanceTree(DynamicAabbTreeNode* tree, bool isLeft)
{
	const int heightDiference = tree->mRight->GetHeight() - tree->mLeft->GetHeight();

	// already balanced?
	if (PhysicsUtils::Abs(heightDiference) < 2)
		return;

	// identifying pivot node
	DynamicAabbTreeNode* pivotNode;
	if (heightDiference < 0) // left side is higher
	{
		pivotNode = tree->mLeft;
		tree->mLeft = nullptr;
	}
	else                     // right side is higher
	{
		pivotNode = tree->mRight;
		tree->mRight = nullptr;
	}

	// ataching pivot to grandnode
	DynamicAabbTreeNode* granny = tree->mParent;
	if (granny == nullptr)
	{
		mRoot = pivotNode;
		pivotNode->mParent = nullptr;
	}
	else
	{
		if (isLeft)
		{
			granny->mLeft = pivotNode;
		}
		else // right
		{
			granny->mRight = pivotNode;
		}
		pivotNode->mParent = granny;
	}

	// identifying small and large children of the pivot node and ataching old parent to the pivot
	DynamicAabbTreeNode* smallChild;
	if (pivotNode->mLeft->GetHeight() < pivotNode->mRight->GetHeight())
	{
		smallChild = pivotNode->mLeft;
		pivotNode->mLeft = tree;
		tree->mParent = pivotNode;
	}
	else
	{
		smallChild = pivotNode->mRight;
		pivotNode->mRight = tree;
		tree->mParent = pivotNode;
	}

	// attaching back small child of pivot to the old parent
	if (tree->mLeft == nullptr)
	{
		tree->mLeft = smallChild;
		smallChild->mParent = tree;
	}
	else
	{
		tree->mRight = smallChild;
		smallChild->mParent = tree;
	}

	// update heights
	tree->mHeight = PhysicsUtils::Max(tree->mLeft->mHeight, tree->mRight->mHeight) + 1;
	tree->mAabb = Aabb::Combine(tree->mLeft->mAabb, tree->mRight->mAabb);
	pivotNode->mHeight = PhysicsUtils::Max(pivotNode->mLeft->mHeight, pivotNode->mRight->mHeight) + 1;
	pivotNode->mAabb = Aabb::Combine(pivotNode->mLeft->mAabb, pivotNode->mRight->mAabb);
}

int DynamicAabbTree::InsertDataRec(unsigned int& key, const SpatialPartitionData& data, DynamicAabbTreeNode* newNode, DynamicAabbTreeNode* currentNode, bool is_left)
{
	// did we reach the bottom
	if (currentNode->GetHeight() == 0)
	{
		//SpatialPartitionData newParentData;
		DynamicAabbTreeNode* newParent = new DynamicAabbTreeNode(static_cast<unsigned int>(-1), SpatialPartitionData());

		// updating links
		newParent->mLeft = currentNode;
		newParent->mRight = newNode;
		newParent->mParent = currentNode->mParent;

		newNode->mParent = newParent;
		currentNode->mParent = newParent;

		// updating data
		newParent->mHeight = currentNode->mHeight + 1;
		newParent->mAabb = Aabb::Combine(currentNode->mAabb, newNode->mAabb);

		if (newParent->mParent == nullptr)
		{
			mRoot = newParent;
		}
		else
		{
			if (is_left)
			{
				newParent->mParent->mLeft = newParent;
			}
			else
			{
				newParent->mParent->mRight = newParent;
			}
		}
		return newParent->mHeight;
	}
	// not the bottom yet, keep going deeper
	bool isLeftChild;
	DynamicAabbTreeNode* nextNode = SelectNode(newNode, currentNode->mLeft, currentNode->mRight, isLeftChild);
	currentNode->mHeight = PhysicsUtils::Max(currentNode->mHeight, 1 + InsertDataRec(key, data, newNode, nextNode, isLeftChild));
	currentNode->mAabb = Aabb::Combine(currentNode->mLeft->mAabb, currentNode->mRight->mAabb);
	BalanceTree(currentNode, is_left);
	return currentNode->mHeight;

}

void DynamicAabbTree::InsertData(unsigned int& key, const SpatialPartitionData& data)
{
	DynamicAabbTreeNode* newNode = NewNode(key, data);
	// first node?
	if (mRoot == nullptr)
	{
		mRoot = newNode;
		return;
	}

	InsertDataRec(key, data, newNode, mRoot, true);
}

void DynamicAabbTree::UpdateData(unsigned int& key, const SpatialPartitionData& data)
{
	if (mData.count(key) == 0) // invalid key?
		return;

	DynamicAabbTreeNode* nodeToCheck = mData.at(key);

	if (nodeToCheck->mAabb.Contains(data.mAabb))
		return;

	// TODO: do not delete/new, just pop 
	RemoveData(key);
	InsertData(key, data);
}

void DynamicAabbTree::RemoveData(unsigned int& key)
{
	// invalid key?
	if (mData.count(key) == 0)
		return;

	DynamicAabbTreeNode* nodeToDel = mData.at(key);

	DynamicAabbTreeNode* parent = nodeToDel->mParent;
	if (parent == nullptr)
	{
		// TODO: check for existance of children
		delete nodeToDel;
		mRoot = nullptr;
		mData.clear();
		return;
	}

	DynamicAabbTreeNode* nodeToUpdate;
	if (static_cast<void*>(parent->mRight) == static_cast<void*>(nodeToDel))
	{
		nodeToUpdate = parent->mLeft;
	}
	else
	{
		nodeToUpdate = parent->mRight;
	}

	DynamicAabbTreeNode* granny = parent->mParent;
	if (granny == nullptr)
	{
		mRoot = nodeToUpdate;
		nodeToUpdate->mParent = nullptr;
	}
	else
	{
		if (static_cast<void*>(granny->mRight) == static_cast<void*>(parent))
		{
			granny->mRight = nodeToUpdate;
		}
		else
		{
			granny->mLeft = nodeToUpdate;
		}
		nodeToUpdate->mParent = granny;
	}
	// TODO nodeToDel had lost children, they needs to be relocated
	mData.erase(nodeToDel->mId);
	// mData.erase(parent->mId);
	delete nodeToDel;
	delete parent;

	nodeToUpdate = nodeToUpdate->mParent;
	while (nodeToUpdate)
	{
		nodeToUpdate->mHeight = PhysicsUtils::Max(nodeToUpdate->mLeft->mHeight, nodeToUpdate->mRight->mHeight) + 1;
		parent = nodeToUpdate->mParent;

		bool nodeOnTheLeft = true;
		if (parent != nullptr)
		{
			if (static_cast<void*>(parent->mRight) == static_cast<void*>(nodeToUpdate))
			{
				nodeOnTheLeft = false;
			}
		}
		BalanceTree(nodeToUpdate, nodeOnTheLeft);
		nodeToUpdate->mAabb = Aabb::Combine(nodeToUpdate->mLeft->mAabb, nodeToUpdate->mRight->mAabb);
		nodeToUpdate = nodeToUpdate->mParent;
	}
}

#ifdef _DEBUG
void DynamicAabbTree::DebugDrawRec(int level, const DirectX::SimpleMath::Vector4& color, DynamicAabbTreeNode* node, int currentLevel)
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
		node->mAabb.DebugDraw(color);
	}

	if (currentLevel < level || level == -1)
	{
		DebugDrawRec(level, color, node->mLeft, currentLevel + 1);
		DebugDrawRec(level, color, node->mRight, currentLevel + 1);
	}
}

void DynamicAabbTree::DebugDraw(int level, const DirectX::SimpleMath::Vector4& color)
{
	DebugDrawRec(level, color, mRoot, 0);
}
#endif

//void DynamicAabbTree::CastRayRec(const Ray& ray, RayCastResults& results, DynamicAabbTreeNode* node) const
//{
//	if (node == nullptr)
//		return;
//
//	float t;
//	if (Intersection::RayAabb(ray.start, ray.direction, node->mAabb.GetMin(), node->mAabb.GetMax(), t))
//	{
//		RayCastResult result;
//		result.mClientData = node->mClientData;
//		result.mTime = t;
//		if (node->mLeft == nullptr)
//			results.AddResult(result);
//		CastRayRec(ray, results, node->mLeft);
//		CastRayRec(ray, results, node->mRight);
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
//    FrustumAddResultsRec(results, node->mLeft);
//    FrustumAddResultsRec(results, node->mRight);
//}
//
//void DynamicAabbTree::CastFrustumRec(const Frustum& frustum, CastResults& results, DynamicAabbTreeNode* node) const
//{
//    if (node == nullptr)
//        return;
//
//    const unsigned test = FrustumAabb(frustum.GetPlanes(), node->mAabb.GetMin(), node->mAabb.GetMax(), node->mLastAxis);
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
	if (node == nullptr || node->mLeft == nullptr)
	{
		return;
	}

	SelfQueryRec(results, node->mLeft, node->mRight);
	SelfQueryRec(results, node->mLeft);
	SelfQueryRec(results, node->mRight);
}

void DynamicAabbTree::SplitNodes(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const
{
	if (right->GetAabb().GetVolume() < left->GetAabb().GetVolume())
	{
		SelfQueryRec(results, left->mLeft, right);
		SelfQueryRec(results, left->mRight, right);
	}
	else
	{
		SelfQueryRec(results, left, right->mLeft);
		SelfQueryRec(results, left, right->mRight);
	}
}

void DynamicAabbTree::SelfQueryRec(QueryResults& results, DynamicAabbTreeNode* left, DynamicAabbTreeNode* right) const
{
	bool isColliding = Intersection::AabbAabb(left->mAabb.GetMin(), left->mAabb.GetMax(), right->mAabb.GetMin(), right->mAabb.GetMax());
	if (!isColliding)
		return;
	// both are leafs
	if (left->IsLeaf() && right->IsLeaf())
	{
		results.AddResult(QueryResult(left->mClientData, right->mClientData));
	}// one internal one leaf
	else if (left->IsLeaf())
	{
		SelfQueryRec(results, left, right->mLeft);
		SelfQueryRec(results, left, right->mRight);
	}
	else if (right->IsLeaf())
	{
		SelfQueryRec(results, left->mLeft, right);
		SelfQueryRec(results, left->mRight, right);
	}// Both are internal, split the nodes
	else
	{
		SplitNodes(results, left, right);
	}
}

void DynamicAabbTree::SelfQuery(QueryResults& results)
{
	SelfQueryRec(results, mRoot);
}

DynamicAabbTreeNode* DynamicAabbTree::GetRoot() const
{
	// Return the root of your tree so that unit tests can print out the contents
	return mRoot;
}
#pragma endregion