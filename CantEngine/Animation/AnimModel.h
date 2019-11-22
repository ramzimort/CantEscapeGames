#pragma once
#include "Helper/Hash.h"
#include "Graphics/Models/Model.h"
#include "Animation/Bone.h"
#include "Animation/Animation.h"


/**
 * @file AnimModel.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief Defines 3D Model data
 */

struct BoneVertexData
{
	int32_t m_boneIndices[4];
	float m_boneWeights[4];

	BoneVertexData()
	{
		for (int i = 0; i < 4; ++i)
		{
			m_boneIndices[i] = -1;
			m_boneWeights[i] = 0.0f;
		}
	}
};

/**
 * @brief Define 3D mesh data for animated model with bone hierarchy
 * 
 */
struct AnimVertexData
{
	VertexData m_vertexData;
	int32_t m_boneIndices[4];
	float m_boneWeights[4];

	AnimVertexData(const VertexData& vertexData) : m_vertexData(vertexData)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_boneIndices[i] = -1;
			m_boneWeights[i] = 0.0f;
		}
	}
};


/**
 * @brief Define the interface for interacting with Animated Model
 * 
 */
class AnimModel : public Model 
{
public:
	AnimModel();
	virtual ~AnimModel();

	void ResizeBoneDataList();
	void PassIndicesAndWeightsPerMesh(std::vector<std::vector<int>> const& indices, 
		std::vector<std::vector<float>> const& weights);
private:
	void InitBuffer(DXRenderer* dxrenderer) override;
public:
	//Important data for the animations TODO - make private
	/**
	 * @brief Contains the list of bone with their name
	 * 
	 */
	std::unordered_map<std::string, Bone> boneMap;
private:
	std::vector<BoneVertexData> m_boneDataList;
};