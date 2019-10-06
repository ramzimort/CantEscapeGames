#include "stdafx.h"
#include "DepthPassRendering.h"
#include "Graphics/Model.h"
#include "Graphics/Material.h"


DepthPassRendering::DepthPassRendering(AppRenderer* app_renderer, uint32_t sample_count)
	:m_app_renderer(app_renderer),
	m_sample_count(sample_count)
{
}


DepthPassRendering::~DepthPassRendering()
{
}

void DepthPassRendering::render_depth_pass(const DepthPassContext& depth_pass_context)
{
	RenderTarget*& depth_rt = depth_pass_context.depth_render_target;
	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;
	load_actions_desc.m_load_action_depth = LoadActionType::CLEAR;
	load_actions_desc.m_clear_depth_stencil.m_depth = depth_rt->get_clear_value().m_depth;
	load_actions_desc.m_clear_depth_stencil.m_stencil = depth_rt->get_clear_value().m_stencil;

	m_dxrenderer->cmd_bind_render_targets(nullptr, 0, &(*depth_rt), load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, depth_rt->get_texture()->get_desc().m_width, depth_rt->get_texture()->get_desc().m_height);

	m_dxrenderer->cmd_bind_pipeline(m_depth_pass_pipeline);
	const InstanceRenderList& instance_render_list = *depth_pass_context.instance_render_list;

	for (uint64_t i = 0; i < instance_render_list.size(); ++i)
	{
		const InstanceRenderData& inst_data = instance_render_list[i];
		Model* p_ref_model = inst_data.p_ref_model;

		if (!p_ref_model)
		{
			continue;
		}

		if (i >= m_object_uniform_buffer_list.size())
		{
			add_object_uniform_buffer();
		}

		Buffer* obj_uniform_buffer = m_object_uniform_buffer_list[i];

		m_object_uniform_data_list[i] = {};
		//m_object_uniform_data = {};
		/*
		Commented for now since this variable won't be used, probably in the parallel universe ?
		m_object_uniform_data.ModelMat = inst_data.model_mat;
		m_object_uniform_data.InvModelMat = DirectX::XMMatrixInverse(nullptr, inst_data.model_mat);
		m_object_uniform_data.NormalMat = inst_data.normal_mat;*/
		m_object_uniform_data_list[i].ModelViewProjectionMat = inst_data.model_mat * depth_pass_context.depth_pass_camera_uniform_data->ViewProjectionMat;
		

		BufferUpdateDesc update_object_uniform_desc = {};
		update_object_uniform_desc.m_buffer = obj_uniform_buffer;
		update_object_uniform_desc.m_pSource = &m_object_uniform_data_list[i];
		update_object_uniform_desc.m_size = sizeof(ObjectUniformData);
		m_dxrenderer->cmd_update_buffer(update_object_uniform_desc);


		m_dxrenderer->cmd_bind_vertex_buffer(inst_data.p_ref_model->get_vertex_buffer());

		Buffer* index_buffer = p_ref_model->get_index_buffer();
		if (index_buffer)
		{
			m_dxrenderer->cmd_bind_index_buffer(index_buffer);
		}

		const Model::MeshesList& meshes_list = p_ref_model->GetMeshesList();
		uint32_t mesh_instance_count = std::max(1u, (uint32_t)meshes_list.size());

		DescriptorData params[1] = {};

		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
		params[0].m_buffers = &obj_uniform_buffer;

		m_dxrenderer->cmd_bind_descriptor(m_depth_pass_pipeline, 1, params);

		if (meshes_list.size() <= 0)
		{
			m_dxrenderer->cmd_draw_index(p_ref_model->get_index_total_count(), 0, 0);
		}
		else
		{
			for (uint32_t mesh_index = 0; mesh_index < mesh_instance_count; ++mesh_index)
			{
				const Mesh& mesh_instance = meshes_list[mesh_index];
				m_dxrenderer->cmd_draw_index(mesh_instance.get_index_count(), mesh_instance.get_start_index(), mesh_instance.get_start_vertex());
			}
		}
	}

}

void DepthPassRendering::Release()
{
	SafeReleaseDelete(m_depth_pass_shader);
	SafeReleaseDelete(m_depth_pass_pipeline);

	for (Buffer* buffer : m_object_uniform_buffer_list)
	{
		SafeReleaseDelete(buffer);
	}
}

void DepthPassRendering::add_object_uniform_buffer()
{
	BufferLoadDesc object_uniform_buffer_desc = {};
	object_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	object_uniform_buffer_desc.m_desc.m_debugName = "Depth Pass Object Uniform Buffer";
	object_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	object_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;

	object_uniform_buffer_desc.m_rawData = nullptr;
	object_uniform_buffer_desc.m_size = sizeof(ObjectUniformData);

	Buffer* object_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, object_uniform_buffer_desc);
	m_object_uniform_buffer_list.push_back(object_uniform_buffer);

	m_object_uniform_data_list.push_back(ObjectUniformData{});
}

void DepthPassRendering::load_content(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	m_object_uniform_data_list.reserve(100);

	ShaderLoadDesc depth_pass_shader_desc = {};
	depth_pass_shader_desc.m_desc.m_vertex_shader_path = "depth_pass_vert.hlsl";
	
	m_depth_pass_shader = DXResourceLoader::Create_Shader(m_dxrenderer, depth_pass_shader_desc);


	VertexLayout pos3_layout = {};
	pos3_layout.m_atrrib_count = 1;

	pos3_layout.m_attribs[0].m_binding = 0;
	pos3_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos3_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;


	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;

	pipeline_desc.m_graphics_desc = {};

	GraphicsPipelineDesc& depth_pass_pipeline_desc = pipeline_desc.m_graphics_desc;
	depth_pass_pipeline_desc.m_render_target_count = 1;
	depth_pass_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	depth_pass_pipeline_desc.m_blend_state = nullptr;
	depth_pass_pipeline_desc.m_depth_state = m_app_renderer->m_less_equal_depth_state;
	depth_pass_pipeline_desc.m_shader = m_depth_pass_shader;
	depth_pass_pipeline_desc.m_vertex_layout = &pos3_layout;

	if (m_sample_count > 1)
	{
		depth_pass_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_back_rasterizer_ms_state;
	}
	else
	{
		depth_pass_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_back_rasterizer_state;
	}

	m_depth_pass_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

}

