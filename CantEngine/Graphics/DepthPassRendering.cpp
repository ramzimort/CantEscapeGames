#include "stdafx.h"
#include "DepthPassRendering.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Material.h"


DepthPassRendering::DepthPassRendering(AppRenderer* app_renderer, uint32_t sample_count,
	DepthPassContextType context_type)
	:m_appRenderer(app_renderer),
	m_sample_count(sample_count),
	m_depth_pass_context_type(context_type)
{
}


DepthPassRendering::~DepthPassRendering()
{
}

void DepthPassRendering::RenderDepthPass(const DepthPassContext& depth_pass_context)
{
	RenderTarget*& depth_rt = depth_pass_context.depth_render_target;
	LoadActionsDesc load_actions_desc = {};
	load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;

	uint32_t color_rt_count = 0;

	if (depth_pass_context.color_depth_render_target != nullptr)
	{
		load_actions_desc.m_load_actions_color[0] = LoadActionType::CLEAR;
		load_actions_desc.m_clear_color_values[0] = depth_pass_context.color_depth_render_target->get_clear_value();
		color_rt_count = 1;
	}

	load_actions_desc.m_load_action_depth = LoadActionType::CLEAR;
	load_actions_desc.m_clear_depth_stencil.m_depth = depth_rt->get_clear_value().m_depth;
	load_actions_desc.m_clear_depth_stencil.m_stencil = depth_rt->get_clear_value().m_stencil;

	m_dxrenderer->cmd_bind_render_targets(&depth_pass_context.color_depth_render_target, color_rt_count, &(*depth_rt), load_actions_desc);
	m_dxrenderer->cmd_set_viewport(0, 0, depth_rt->get_texture()->get_desc().m_width, depth_rt->get_texture()->get_desc().m_height);

	RenderBasicMeshDepthPass(depth_pass_context);
	RenderAnimatedModelDepthPass(depth_pass_context);
}


void DepthPassRendering::RenderBasicMeshDepthPass(const DepthPassContext& depth_pass_context)
{
	m_dxrenderer->cmd_bind_pipeline(m_depth_pass_pipeline);
	const InstanceRenderList& instance_render_list = *depth_pass_context.instance_render_list;

	for (uint64_t i = 0; i < instance_render_list.size(); ++i)
	{
		const InstanceRenderData& inst_data = instance_render_list[i];
		Model* p_ref_model = inst_data.p_ref_model;

		assert(p_ref_model);

		if (i >= m_objectUniformBufferList.size())
		{
			AddObjectUniformBuffer();
		}

		Buffer* obj_uniform_buffer = m_objectUniformBufferList[i];

		m_objectUniformDataList[i] = {};
		m_objectUniformDataList[i].ModelViewProjectionMat = inst_data.model_mat * depth_pass_context.depth_pass_camera_uniform_data->ViewProjectionMat;


		BufferUpdateDesc update_object_uniform_desc = {};
		update_object_uniform_desc.m_buffer = obj_uniform_buffer;
		update_object_uniform_desc.m_pSource = &m_objectUniformDataList[i];
		update_object_uniform_desc.m_size = sizeof(ObjectUniformData);
		m_dxrenderer->cmd_update_buffer(update_object_uniform_desc);


		m_dxrenderer->cmd_bind_vertex_buffer(inst_data.p_ref_model->GetVertexBuffer());

		Buffer* index_buffer = p_ref_model->GetIndexBuffer();
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
			m_dxrenderer->cmd_draw_index(p_ref_model->GetIndexTotalCount(), 0, 0);
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

void DepthPassRendering::RenderAnimatedModelDepthPass(const DepthPassContext& depth_pass_context)
{
	m_dxrenderer->cmd_bind_pipeline(m_depthPassAnimModelPipeline);

	uint64_t latestObjectBufferIndex = m_appRenderer->m_basicInstances.size();

	for (uint64_t i = 0; i < m_appRenderer->m_boneMeshInstancesList.size(); ++i)
	{
		const InstanceRenderData& inst_data = m_appRenderer->m_boneMeshInstancesList[i].m_instanceRenderData;
		Model* p_ref_model = inst_data.p_ref_model;

		assert(p_ref_model);

		if (latestObjectBufferIndex >= m_objectUniformBufferList.size())
		{
			AddObjectUniformBuffer();
		}

		Buffer* obj_uniform_buffer = m_objectUniformBufferList[latestObjectBufferIndex];

		m_objectUniformDataList[latestObjectBufferIndex] = {};
		m_objectUniformDataList[latestObjectBufferIndex].ModelViewProjectionMat = inst_data.model_mat * depth_pass_context.depth_pass_camera_uniform_data->ViewProjectionMat;


		BufferUpdateDesc update_object_uniform_desc = {};
		update_object_uniform_desc.m_buffer = obj_uniform_buffer;
		update_object_uniform_desc.m_pSource = &m_objectUniformDataList[latestObjectBufferIndex];
		update_object_uniform_desc.m_size = sizeof(ObjectUniformData);
		m_dxrenderer->cmd_update_buffer(update_object_uniform_desc);

		++latestObjectBufferIndex;

		Buffer* boneUniformBuffer = m_appRenderer->m_boneTransformsUniformBufferList[i];

		m_dxrenderer->cmd_bind_vertex_buffer(inst_data.p_ref_model->GetVertexBuffer());

		Buffer* index_buffer = p_ref_model->GetIndexBuffer();
		if (index_buffer)
		{
			m_dxrenderer->cmd_bind_index_buffer(index_buffer);
		}

		const Model::MeshesList& meshes_list = p_ref_model->GetMeshesList();
		uint32_t mesh_instance_count = std::max(1u, (uint32_t)meshes_list.size());

		DescriptorData params[2] = {};

		params[0].m_binding_location = 0;
		params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
		params[0].m_buffers = &obj_uniform_buffer;

		params[1].m_binding_location = 1;
		params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
		params[1].m_shader_stages = Shader_Stages::VERTEX_STAGE;
		params[1].m_buffers = &boneUniformBuffer;

		m_dxrenderer->cmd_bind_descriptor(m_depthPassAnimModelPipeline, 2, params);

		if (meshes_list.size() <= 0)
		{
			m_dxrenderer->cmd_draw_index(p_ref_model->GetIndexTotalCount(), 0, 0);
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

	SafeReleaseDelete(m_depthPassAnimModelShader);
	SafeReleaseDelete(m_depthPassAnimModelPipeline);

	for (Buffer* buffer : m_objectUniformBufferList)
	{
		SafeReleaseDelete(buffer);
	}
}

void DepthPassRendering::AddObjectUniformBuffer()
{
	BufferLoadDesc object_uniform_buffer_desc = {};
	object_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	object_uniform_buffer_desc.m_desc.m_debugName = "Depth Pass Object Uniform Buffer";
	object_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	object_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;

	object_uniform_buffer_desc.m_rawData = nullptr;
	object_uniform_buffer_desc.m_size = sizeof(ObjectUniformData);

	Buffer* object_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, object_uniform_buffer_desc);
	m_objectUniformBufferList.push_back(object_uniform_buffer);

	m_objectUniformDataList.push_back(ObjectUniformData{});
}

void DepthPassRendering::LoadContent(DXRenderer* dxrenderer)
{
	m_dxrenderer = dxrenderer;

	ShaderMacro deferredPassShaderMacro = {};
	deferredPassShaderMacro.m_name = "ANIM_MODEL";
	deferredPassShaderMacro.m_definition = std::to_string(0);

	ShaderLoadDesc depth_pass_shader_desc = {};
	depth_pass_shader_desc.m_desc.m_vertex_shader_path = "depth_pass_vert.hlsl";
	depth_pass_shader_desc.m_shader_macro_count = 1;
	depth_pass_shader_desc.m_shader_macro = &deferredPassShaderMacro;

	if (m_depth_pass_context_type == DepthPassContextType::FOUR_MOMENT_Z_BUFFER)
	{
		depth_pass_shader_desc.m_desc.m_pixel_shader_path = "moment_depth_pass_frag.hlsl";
	}


	m_depth_pass_shader = DXResourceLoader::Create_Shader(m_dxrenderer, depth_pass_shader_desc);

	deferredPassShaderMacro.m_definition = std::to_string(1);

	m_depthPassAnimModelShader = DXResourceLoader::Create_Shader(m_dxrenderer, depth_pass_shader_desc);

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
	depth_pass_pipeline_desc.m_depth_state = m_appRenderer->m_less_equal_depth_state;
	depth_pass_pipeline_desc.m_shader = m_depth_pass_shader;
	depth_pass_pipeline_desc.m_vertex_layout = &pos3_layout;

	if (m_sample_count > 1)
	{
		depth_pass_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_ms_state;
	}
	else
	{
		depth_pass_pipeline_desc.m_rasterizer_state = m_appRenderer->m_cull_none_rasterizer_state;
	}

	m_depth_pass_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	VertexLayout pos_normal_tangent_bitangent_uv_bones_layout = {};
	pos_normal_tangent_bitangent_uv_bones_layout.m_atrrib_count = 7;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[1].m_semantic = Attrib_Semantic::NORMAL;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[2].m_semantic = Attrib_Semantic::TANGENT;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[3].m_semantic = Attrib_Semantic::BITANGENT;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_format = DXGI_FORMAT_R32G32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[4].m_semantic = Attrib_Semantic::TEXCOORD_0;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_format = DXGI_FORMAT_R32G32B32A32_SINT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[5].m_semantic = Attrib_Semantic::TEXCOORD_1;

	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_binding = 0;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	pos_normal_tangent_bitangent_uv_bones_layout.m_attribs[6].m_semantic = Attrib_Semantic::TEXCOORD_2;

	depth_pass_pipeline_desc.m_shader = m_depthPassAnimModelShader;
	depth_pass_pipeline_desc.m_vertex_layout = &pos_normal_tangent_bitangent_uv_bones_layout;

	m_depthPassAnimModelPipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);
}

