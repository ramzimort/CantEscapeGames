#pragma once
#include "Helper/Hash.h"
#include "Graphics/Models/Model.h"
#include "Animation/Bone.h"
#include "Animation/Animation.h"



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
	std::unordered_map<std::string, Bone> boneMap;

private:
	std::vector<BoneVertexData> m_boneDataList;

};