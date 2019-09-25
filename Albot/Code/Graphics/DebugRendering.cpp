#include "DebugRendering.h"
#include "D3D11_Renderer.h"
#include "Model.h"
#include "Material.h"
#include "AppRenderer.h"
#include "../Managers/ResourceManager.h"

DebugRendering::DebugRendering(AppRenderer* app_renderer, ResourceManager* resourceManager)
	:m_app_renderer(app_renderer),
	m_resourceManager(resourceManager)
{
}


DebugRendering::~DebugRendering()
{
}


void DebugRendering::Release()
{
#ifdef DEVELOPER
	SafeReleaseDelete(m_debug_draw_pipeline);
	SafeReleaseDelete(m_debug_draw_shader);
	SafeReleaseDelete(m_debug_aabb_instance_uniform_buffer);

	SafeReleaseDelete(m_debug_line_vertex_buffer);
	SafeReleaseDelete(m_line_uniform_structured_buffer);
	SafeReleaseDelete(m_line_draw_pipeline);
	SafeReleaseDelete(m_line_draw_shader);
#endif
}


void DebugRendering::LoadContent(DXRenderer* dxrenderer)
{
#ifdef DEVELOPER
	m_dxrenderer = dxrenderer;

	ShaderLoadDesc debug_draw_shader_desc = {};
	debug_draw_shader_desc.m_desc.m_vertex_shader_path = "debug_instance_vert.hlsl";
	debug_draw_shader_desc.m_desc.m_pixel_shader_path = "debug_frag.hlsl";

	m_debug_draw_shader = DXResourceLoader::Create_Shader(m_dxrenderer, debug_draw_shader_desc);

	VertexLayout pos3_layout = {};

	pos3_layout.m_atrrib_count = 1;
	pos3_layout.m_attribs[0].m_binding = 0;
	pos3_layout.m_attribs[0].m_format = DXGI_FORMAT_R32G32B32_FLOAT;
	pos3_layout.m_attribs[0].m_semantic = Attrib_Semantic::POSITION;

	PipelineDesc pipeline_desc = {};
	pipeline_desc.m_pipeline_type = PipelineType::GRAPHICS;

	pipeline_desc.m_graphics_desc = {};
	GraphicsPipelineDesc& debug_draw_pipeline_desc = pipeline_desc.m_graphics_desc;
	debug_draw_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_TRIANGLE_LIST;
	debug_draw_pipeline_desc.m_render_target_count = 1;
	debug_draw_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_none_wireframe_state;
	debug_draw_pipeline_desc.m_depth_state = m_app_renderer->m_disabled_depth_state;
	debug_draw_pipeline_desc.m_vertex_layout = &pos3_layout;
	debug_draw_pipeline_desc.m_shader = m_debug_draw_shader;

	m_debug_draw_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);


	BufferLoadDesc debug_aabb_uniform_buffer = {};
	debug_aabb_uniform_buffer.m_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	debug_aabb_uniform_buffer.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	debug_aabb_uniform_buffer.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	debug_aabb_uniform_buffer.m_desc.m_miscFlags = Misc_Flags::MISC_BUFFER_STRUCTURED;
	//TODO:
	debug_aabb_uniform_buffer.m_desc.m_elementCount = 900;
	debug_aabb_uniform_buffer.m_desc.m_structureStride = sizeof(DebugInstanceUniformData);
	debug_aabb_uniform_buffer.m_size = debug_aabb_uniform_buffer.m_desc.m_elementCount *
		debug_aabb_uniform_buffer.m_desc.m_structureStride;
	debug_aabb_uniform_buffer.m_rawData = nullptr;

	m_debug_aabb_instance_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, debug_aabb_uniform_buffer);


	ShaderLoadDesc debug_line_shader_desc = {};
	debug_line_shader_desc.m_desc.m_vertex_shader_path = "debug_line_vert.hlsl";
	debug_line_shader_desc.m_desc.m_pixel_shader_path = "debug_frag.hlsl";

	m_line_draw_shader = DXResourceLoader::Create_Shader(m_dxrenderer, debug_line_shader_desc);


	pipeline_desc.m_graphics_desc = {};

	GraphicsPipelineDesc& debug_line_pipeline_desc = pipeline_desc.m_graphics_desc;
	debug_line_pipeline_desc.m_primitive_topo_type = Primitive_Topology::TOPOLOGY_LINE_LIST;
	debug_line_pipeline_desc.m_render_target_count = 1;
	debug_line_pipeline_desc.m_rasterizer_state = m_app_renderer->m_cull_none_rasterizer_state;
	debug_line_pipeline_desc.m_depth_state = m_app_renderer->m_disabled_depth_state;
	debug_line_pipeline_desc.m_vertex_layout = &pos3_layout;
	debug_line_pipeline_desc.m_shader = m_line_draw_shader;

	m_line_draw_pipeline = DXResourceLoader::Create_Pipeline(m_dxrenderer, pipeline_desc);

	BufferLoadDesc debug_line_vertex_buffer_desc = {};
	debug_line_vertex_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_VERTEX_BUFFER;
	debug_line_vertex_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	debug_line_vertex_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	debug_line_vertex_buffer_desc.m_desc.m_vertexStride = sizeof(Vector3);
	debug_line_vertex_buffer_desc.m_size = debug_line_vertex_buffer_desc.m_desc.m_vertexStride * MAX_LINE_DEBUG_DRAW;
	debug_line_vertex_buffer_desc.m_rawData = nullptr;

	m_debug_line_vertex_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer,
		debug_line_vertex_buffer_desc);

	BufferLoadDesc debug_line_uniform_buffer_desc = {};
	debug_line_uniform_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	debug_line_uniform_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	debug_line_uniform_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	debug_line_uniform_buffer_desc.m_desc.m_miscFlags = Misc_Flags::MISC_BUFFER_STRUCTURED;
	debug_line_uniform_buffer_desc.m_desc.m_structureStride = sizeof(LineVertexUniformData);
	debug_line_uniform_buffer_desc.m_desc.m_elementCount = MAX_LINE_DEBUG_DRAW;
	debug_line_uniform_buffer_desc.m_size = sizeof(LineVertexUniformData) * MAX_LINE_DEBUG_DRAW;
	debug_line_uniform_buffer_desc.m_rawData = nullptr;

	m_line_uniform_structured_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer,
		debug_line_uniform_buffer_desc);
#endif
}

void DebugRendering::RegisterDebugLineInstance(const Vector3& start_pos, const Vector3& end_pos, const Vector3& color)
{
#ifdef DEVELOPER
	RegisterDebugLineInstance(DebugLineInstance{ start_pos, end_pos, color });
#endif
}

void DebugRendering::RegisterDebugLineInstance(const DebugLineInstance& line_instance)
{
#ifdef DEVELOPER
	m_debug_line_instance_list.push_back(line_instance);
#endif
}

void DebugRendering::RegisterDebugAABB(const DebugAABBInstance& debug_aabb_instance)
{
#ifdef DEVELOPER
	m_debug_aabb_instance_list.push_back(debug_aabb_instance);
#endif
}

void DebugRendering::RegisterDebugSphere(const DebugSphereInstance& debug_sphere_instance)
{
}

void DebugRendering::RenderDebugScene()
{
#ifdef DEVELOPER
	//TODO: only for testing
	/*DebugAABBInstance aabb_instance = {};
	aabb_instance.m_color = Vector3(1.0, 0.0, 0.0);
	Vector3 size(10.f);

	aabb_instance.m_min_bound = Vector3(-size.x, -size.y, -size.z) / 2.f;
	aabb_instance.m_max_bound = Vector3(size.x, size.y, size.z) / 2.f;


	RegisterDebugAABB(aabb_instance);

	aabb_instance.m_color = Vector3(0.0, 1.0, 0.0);
	aabb_instance.m_min_bound = Vector3(-size.x, -size.y, -size.z);
	aabb_instance.m_max_bound = Vector3(size.x, size.y, size.z) ;


	RegisterDebugLineInstance(Vector3(0, 0.0, 0.0), Vector3(10, 20.0, -10.0), Vector3(1.0, 1.0, 0.0));
	RegisterDebugLineInstance(Vector3(0, 20.0, -10.0), Vector3(100.0, -10.0, 20.0), Vector3(1.0, 0.0, 1.0));

	RegisterDebugAABB(aabb_instance);*/
	//only for testing

	render_debug_aabb_scene();
	render_debug_line_scene();

	m_debug_aabb_instance_list.clear();
	m_debug_sphere_list.clear();
	m_debug_line_instance_list.clear();
	m_raw_line_vertices.clear();
#endif
}


void DebugRendering::render_debug_line_scene()
{
#ifdef DEVELOPER
	uint32_t debug_line_count = m_debug_line_instance_list.size();
	if (debug_line_count <= 0)
	{
		return;
	}

	for (uint32_t i = 0; i < debug_line_count; ++i)
	{
		const DebugLineInstance& line_instance = m_debug_line_instance_list[i];
		m_line_vertex_uniform_data[i].Color = MathUtil::v3_to_v4(line_instance.m_color);
		m_raw_line_vertices.push_back(line_instance.m_startpos);
		m_raw_line_vertices.push_back(line_instance.m_endpos);
	}

	BufferUpdateDesc line_uniform_buffer_update_desc = {};
	line_uniform_buffer_update_desc.m_buffer = m_line_uniform_structured_buffer;
	line_uniform_buffer_update_desc.m_pSource = &m_line_vertex_uniform_data[0];
	line_uniform_buffer_update_desc.m_size = static_cast<uint32_t>(sizeof(debug_line_count) * sizeof(LineVertexUniformData));
	m_dxrenderer->cmd_update_buffer(line_uniform_buffer_update_desc);

	BufferUpdateDesc line_vertex_buffer_update_desc = {};
	line_vertex_buffer_update_desc.m_buffer = m_debug_line_vertex_buffer;
	line_vertex_buffer_update_desc.m_pSource = &m_raw_line_vertices[0];
	line_vertex_buffer_update_desc.m_size = static_cast<uint32_t>(sizeof(Vector3) * m_raw_line_vertices.size());
	m_dxrenderer->cmd_update_buffer(line_vertex_buffer_update_desc);
	
	
	m_dxrenderer->cmd_bind_pipeline(m_line_draw_pipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(m_debug_line_vertex_buffer);

	DescriptorData params[2] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_buffers = &m_app_renderer->m_camera_uniform_buffer;

	params[1].m_binding_location = 0;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[1].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[1].m_buffers = &m_line_uniform_structured_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_line_draw_pipeline, 2, params);
	m_dxrenderer->cmd_draw(m_raw_line_vertices.size(), 0);
#endif
}

void DebugRendering::render_debug_aabb_scene()
{
#ifdef DEVELOPER
	uint32_t debug_instance_count = m_debug_aabb_instance_list.size();
	if (debug_instance_count <= 0)
	{
		return;
	}
	for (uint32_t i = 0; i < debug_instance_count; ++i)
	{
		const DebugAABBInstance& debug_instance = m_debug_aabb_instance_list[i];

		Vector3 center = (debug_instance.m_min_bound
			+ debug_instance.m_max_bound) / 2.f;

		Vector3 size = debug_instance.m_max_bound - debug_instance.m_min_bound;

		Matrix model_matrix = Matrix::CreateScale(size.x, size.y, size.z) *
			Matrix::CreateTranslation(center.x, center.y, center.z);

		m_debug_aabb_instance_uniform_data[i].ModelViewProjectionMat = model_matrix * m_app_renderer->m_camera_uniform_data.ViewProjectionMat;
		m_debug_aabb_instance_uniform_data[i].Color = MathUtil::v3_to_v4(debug_instance.m_color);
	}

	Model* cube_model = m_resourceManager->GetModel(SID("Cube.fbx"));

	BufferUpdateDesc update_debug_instance_buffer_desc = {};
	update_debug_instance_buffer_desc.m_buffer = m_debug_aabb_instance_uniform_buffer;
	update_debug_instance_buffer_desc.m_pSource = &m_debug_aabb_instance_uniform_data[0];
	update_debug_instance_buffer_desc.m_size = sizeof(DebugInstanceUniformData) * debug_instance_count;
	m_dxrenderer->cmd_update_buffer(update_debug_instance_buffer_desc);


	m_dxrenderer->cmd_bind_pipeline(m_debug_draw_pipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(cube_model->get_vertex_buffer());
	m_dxrenderer->cmd_bind_index_buffer(cube_model->get_index_buffer());

	DescriptorData params[4] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_buffers = &m_debug_aabb_instance_uniform_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_debug_draw_pipeline, 1, params);
	m_dxrenderer->cmd_draw_index_instanced(debug_instance_count, 0, cube_model->get_index_total_count(), 0, 0);

#endif
}
