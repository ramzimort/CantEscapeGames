#pragma once

#include "VertexLayout.h"
#include "Common_Renderer.h"

class Shader;
class RasterizerState;
class Buffer;
class BlendState;
class DepthState;


enum PipelineType
{
	COMPUTE,
	GRAPHICS,
	MAX_PIPELINE_COUNT
};


struct ComputePipelineDesc
{
	Shader* m_shader;
};


struct GraphicsPipelineDesc
{
	Primitive_Topology  m_primitive_topo_type;
	uint32_t m_render_target_count;
	VertexLayout* m_vertex_layout;
	Shader* m_shader;
	RasterizerState* m_rasterizer_state;
	BlendState* m_blend_state;
	DepthState* m_depth_state;
};

struct PipelineDesc
{
	PipelineType m_pipeline_type;
	union
	{
		GraphicsPipelineDesc m_graphics_desc;
		ComputePipelineDesc m_compute_desc;
	};
};


class Pipeline
{
public:
	friend class DXRenderer;
	friend class DXResourceLoader;
public:
	Pipeline(const PipelineDesc& desc);
	~Pipeline();

	void Release();
	const PipelineDesc& get_desc() const;
private:
	PipelineDesc m_desc;
	ID3D11InputLayout* m_input_layout;
	D3D_PRIMITIVE_TOPOLOGY m_d3d_primitive_topo_type;
};

