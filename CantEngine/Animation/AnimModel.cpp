#include "AnimModel.h"
#include "Graphics/DXResourceLoader.h"


AnimModel::AnimModel()
{
	//TODO
}


AnimModel::~AnimModel()
{
	//TODO
}


void AnimModel::ResizeBoneDataList()
{
	this->m_boneDataList.resize(this->GetVerticesList().size());
}


void AnimModel::PassIndicesAndWeightsPerMesh( std::vector<std::vector<int>> const& indices,
	std::vector<std::vector<float>> const& weights)
{
	for (int i = 0; i < indices.size(); ++i) 
	{
		BoneVertexData data;

		if (indices[i].size() >= 1) 
		{
			data.m_boneIndices[0] = indices[i][0];
			data.m_boneWeights[0] = weights[i][0];

			if (indices[i].size() >= 2)
			{
				data.m_boneIndices[1] = indices[i][1];
				data.m_boneWeights[1] = weights[i][1];

				if (indices[i].size() >= 3)
				{
					data.m_boneIndices[2] = indices[i][2];
					data.m_boneWeights[2] = weights[i][2];

					if (indices[i].size() == 4)
					{
						data.m_boneIndices[3] = indices[i][3];
						data.m_boneWeights[3] = weights[i][3];
					}
				}
			}
		}

		//----------NORMALIZING-------------------------------
		float weight = data.m_boneWeights[0] + data.m_boneWeights[1] +
			data.m_boneWeights[2] + data.m_boneWeights[3];
		if (fabs(1.0f - weight) > 0.001f)
		{
			data.m_boneWeights[0] /= weight;
			data.m_boneWeights[1] /= weight;
			data.m_boneWeights[2] /= weight;
			data.m_boneWeights[3] /= weight;
		}
		//----------------------------------------------------

		this->m_boneDataList.push_back(data);
	}
}


void AnimModel::InitBuffer(DXRenderer* dxrenderer)
{
	std::vector<AnimVertexData> animVerticesList;
	animVerticesList.reserve(m_boneDataList.size());

	const VerticesList& verticesList = GetVerticesList();

	for (uint64_t i = 0; i < m_boneDataList.size(); ++i)
	{
		animVerticesList.push_back(AnimVertexData(verticesList[i]));
		AnimVertexData& animVertexData = animVerticesList.back();

		for (int boneIndex = 0; boneIndex < 4; ++boneIndex)
		{
			animVertexData.m_boneIndices[boneIndex] = m_boneDataList[i].m_boneIndices[boneIndex];
			animVertexData.m_boneWeights[boneIndex] = m_boneDataList[i].m_boneWeights[boneIndex];
		}

	}

	if (!animVerticesList.empty())
	{
		BufferLoadDesc vb_buffer_desc = {};
		vb_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
		vb_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
		vb_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_IMMUTABLE;
		vb_buffer_desc.m_desc.m_vertexStride = sizeof(AnimVertexData);
		vb_buffer_desc.m_rawData = &animVerticesList[0];
		vb_buffer_desc.m_size = static_cast<uint32_t>(vb_buffer_desc.m_desc.m_vertexStride * animVerticesList.size());

		m_vertex_buffer = DXResourceLoader::Create_Buffer(dxrenderer, vb_buffer_desc);
	}

	if (!m_triangle_indices.empty())
	{
		BufferLoadDesc indx_buffer_desc = {};
		indx_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_INDEX_BUFFER;
		indx_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
		indx_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_IMMUTABLE;
		indx_buffer_desc.m_desc.m_indexType = IndexType::INDEX_UINT32;
		indx_buffer_desc.m_rawData = &m_triangle_indices[0];
		indx_buffer_desc.m_size = static_cast<uint32_t>(sizeof(Triangle) * m_triangle_indices.size());

		m_index_buffer = DXResourceLoader::Create_Buffer(dxrenderer, indx_buffer_desc);
	}
}

