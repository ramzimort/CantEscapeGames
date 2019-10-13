#include "stdafx.h"
#include "DebugRenderingInstance.h"
#include "Helper/Hash.h"
#include "../Managers/ResourceManager.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/Model.h"

static const StringId cubeId = StringId(Constant::ModelsDir + "Cube.fbx");

DebugRenderingInstance::DebugRenderingInstance(DebugRendering& debugRendering)
	:m_debugRendering(debugRendering)
{
	m_appRenderer = m_debugRendering.m_appRenderer;
	m_dxrenderer = m_debugRendering.m_dxrenderer;
}


DebugRenderingInstance::~DebugRenderingInstance()
{
}


void DebugRenderingInstance::Render(const AppRendererContext& appRendererContext)
{
	RenderDebugLineScene(appRendererContext);
	RenderDebugAABBScene(appRendererContext);
}

void DebugRenderingInstance::Initialize()
{
	BufferLoadDesc debug_aabb_uniform_buffer = {};
	debug_aabb_uniform_buffer.m_desc.m_bindFlags = Bind_Flags::BIND_SHADER_RESOURCE;
	debug_aabb_uniform_buffer.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	debug_aabb_uniform_buffer.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	debug_aabb_uniform_buffer.m_desc.m_miscFlags = Misc_Flags::MISC_BUFFER_STRUCTURED;

	debug_aabb_uniform_buffer.m_desc.m_elementCount = MAX_WIREFRAME_DEBUG_DRAW;
	debug_aabb_uniform_buffer.m_desc.m_structureStride = sizeof(DebugInstanceUniformData);
	debug_aabb_uniform_buffer.m_size = debug_aabb_uniform_buffer.m_desc.m_elementCount *
		debug_aabb_uniform_buffer.m_desc.m_structureStride;
	debug_aabb_uniform_buffer.m_rawData = nullptr;

	m_debug_aabb_instance_uniform_buffer = DXResourceLoader::Create_Buffer(m_dxrenderer, debug_aabb_uniform_buffer);
}

void DebugRenderingInstance::LoadContent()
{

}

void DebugRenderingInstance::Release()
{
#ifdef DEVELOPER
	SafeReleaseDelete(m_debug_aabb_instance_uniform_buffer);
#endif
}


void DebugRenderingInstance::Update(const AppRendererContext& appRendererContext, float dt)
{
#ifdef DEVELOPER
	uint32_t debug_instance_count = static_cast<uint32_t>(m_debugRendering.m_debug_aabb_instance_list.size());
	if (debug_instance_count <= 0)
	{
		return;
	}
	for (uint32_t i = 0; i < debug_instance_count; ++i)
	{
		const DebugAABBInstance& debug_instance = m_debugRendering.m_debug_aabb_instance_list[i];

		Vector3 center = (debug_instance.m_min_bound
			+ debug_instance.m_max_bound) / 2.f;

		Vector3 size = debug_instance.m_max_bound - debug_instance.m_min_bound;

		Matrix model_matrix = Matrix::CreateScale(size.x, size.y, size.z) *
			Matrix::CreateTranslation(center.x, center.y, center.z);

		m_debug_aabb_instance_uniform_data[i].ModelViewProjectionMat = model_matrix * appRendererContext.m_appRendererInstance->m_camera_uniform_data.ViewProjectionMat;
		m_debug_aabb_instance_uniform_data[i].Color = MathUtil::v3_to_v4(debug_instance.m_color);
	}
#endif
}


void DebugRenderingInstance::RenderDebugLineScene(const AppRendererContext& appRendererContext)
{
#ifdef DEVELOPER

	m_dxrenderer->cmd_bind_pipeline(m_debugRendering.m_line_draw_pipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(m_debugRendering.m_debug_line_vertex_buffer);

	DescriptorData params[2] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_buffers = &appRendererContext.m_appRendererInstance->m_camera_uniform_buffer;

	params[1].m_binding_location = 0;
	params[1].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[1].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[1].m_buffers = &m_debugRendering.m_line_uniform_structured_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_debugRendering.m_line_draw_pipeline, 2, params);
	m_dxrenderer->cmd_draw(static_cast<uint32_t>(m_debugRendering.m_raw_line_vertices.size()), 0);
#endif
}

void DebugRenderingInstance::RenderDebugAABBScene(const AppRendererContext& appRendererContext)
{
#ifdef DEVELOPER
	uint32_t debug_instance_count = static_cast<uint32_t>(m_debugRendering.m_debug_aabb_instance_list.size());
	if (debug_instance_count <= 0)
	{
		return;
	}

	BufferUpdateDesc update_debug_instance_buffer_desc = {};
	update_debug_instance_buffer_desc.m_buffer = m_debug_aabb_instance_uniform_buffer;
	update_debug_instance_buffer_desc.m_pSource = &m_debug_aabb_instance_uniform_data[0];
	update_debug_instance_buffer_desc.m_size = sizeof(DebugInstanceUniformData) * (uint32_t)m_debugRendering.m_debug_aabb_instance_list.size();
	m_dxrenderer->cmd_update_buffer(update_debug_instance_buffer_desc);

	Model* cube_model = m_debugRendering.m_resourceManager->GetModel(cubeId);

	m_dxrenderer->cmd_bind_pipeline(m_debugRendering.m_debug_draw_pipeline);
	m_dxrenderer->cmd_bind_vertex_buffer(cube_model->get_vertex_buffer());
	m_dxrenderer->cmd_bind_index_buffer(cube_model->get_index_buffer());

	DescriptorData params[4] = {};
	params[0].m_binding_location = 0;
	params[0].m_descriptor_type = DescriptorType::DESCRIPTOR_BUFFER;
	params[0].m_shader_stages = Shader_Stages::VERTEX_STAGE;
	params[0].m_buffers = &m_debug_aabb_instance_uniform_buffer;

	m_dxrenderer->cmd_bind_descriptor(m_debugRendering.m_debug_draw_pipeline, 1, params);
	m_dxrenderer->cmd_draw_index_instanced(debug_instance_count, 0, cube_model->get_index_total_count(), 0, 0);

#endif
}