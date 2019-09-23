#include "Model.h"
#include <iostream>
#include <limits>
#include "DXResourceLoader.h"


Model::Model()
	:m_has_tangent(false)
{
	
}


Model::~Model()
{
	
}

Buffer* Model::get_vertex_buffer()
{
	return m_vertex_buffer;
}

Buffer* Model::get_index_buffer()
{
	return m_index_buffer;
}

uint32_t Model::get_index_total_count() const
{
	return m_triangle_indices.size() * 3;
}

void Model::InitBuffer(DXRenderer* dxrenderer)
{
	if (!m_vertices.empty())
	{
		BufferLoadDesc vb_buffer_desc = {};
		vb_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
		vb_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_NONE;
		vb_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_IMMUTABLE;
		vb_buffer_desc.m_desc.m_vertexStride = sizeof(VertexData);
		vb_buffer_desc.m_rawData = &m_vertices[0];
		vb_buffer_desc.m_size = sizeof(VertexData) * m_vertices.size();

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
		indx_buffer_desc.m_size = sizeof(Triangle) * m_triangle_indices.size();

		m_index_buffer = DXResourceLoader::Create_Buffer(dxrenderer, indx_buffer_desc);
	}
}

void Model::Release()
{
	SafeReleaseDelete(m_vertex_buffer);
	SafeReleaseDelete(m_index_buffer);
}

const Model::MeshesList& Model::GetMeshesList() const
{
	return m_meshes;
}

const Model::VerticesList& Model::GetVerticesList() const
{
	return m_vertices;
}

const Aabb& Model::GetAABB() const
{
	return m_aabb;
}