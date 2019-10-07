#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/AppRenderer.h"

struct DepthPassContext
{
	DepthPassContext(RenderTarget*& depth_rt, CameraUniformData* camera_uniform_data,
		InstanceRenderList* inst_render_list) :depth_render_target(depth_rt), 
		depth_pass_camera_uniform_data(camera_uniform_data),
		instance_render_list(inst_render_list)
	{}
	RenderTarget*& depth_render_target;
	CameraUniformData* depth_pass_camera_uniform_data;
	InstanceRenderList* instance_render_list;
};

class DepthPassRendering{
public:
	DepthPassRendering(AppRenderer* app_renderer, uint32_t sample_count = SAMPLE_COUNT_1);
	~DepthPassRendering();

	void Release();

	void render_depth_pass(const DepthPassContext& depth_pass_context);

	void load_content(DXRenderer* dxrenderer);

private:
	void add_object_uniform_buffer();
private:
	uint32_t m_sample_count;
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_depth_pass_pipeline;
	Shader* m_depth_pass_shader;

	std::vector<Buffer*> m_object_uniform_buffer_list;
	//ObjectUniformData m_object_uniform_data;
	std::vector<ObjectUniformData> m_object_uniform_data_list;
};

