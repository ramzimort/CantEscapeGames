#pragma once
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/AppRenderer.h"

struct DepthPassContext
{
	DepthPassContext(RenderTarget*& color_depth_rt,
		RenderTarget*& depth_rt, CameraUniformData* camera_uniform_data,
		InstanceRenderList* inst_render_list) :
		color_depth_render_target(color_depth_rt),
		depth_render_target(depth_rt),
		depth_pass_camera_uniform_data(camera_uniform_data),
		instance_render_list(inst_render_list)
	{}

	RenderTarget*& color_depth_render_target;
	RenderTarget*& depth_render_target;
	CameraUniformData* depth_pass_camera_uniform_data;
	InstanceRenderList* instance_render_list;
};


enum DepthPassContextType
{
	ONE_Z_BUFFER,
	//for moment shadow map
	FOUR_MOMENT_Z_BUFFER
};



class DepthPassRendering{
public:
	DepthPassRendering(AppRenderer* app_renderer, uint32_t sample_count = SAMPLE_COUNT_1, 
		DepthPassContextType context_type = DepthPassContextType::ONE_Z_BUFFER);
	~DepthPassRendering();

	void Release();

	void RenderDepthPass(const DepthPassContext& depth_pass_context);

	void LoadContent(DXRenderer* dxrenderer);
private:
	void RenderBasicMeshDepthPass(const DepthPassContext& depth_pass_context);
	void RenderAnimatedModelDepthPass(const DepthPassContext& depth_pass_context);
private:
	void AddObjectUniformBuffer();
private:
	uint32_t m_sample_count;
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Pipeline* m_depth_pass_pipeline;
	Shader* m_depth_pass_shader;

	Pipeline* m_depthPassAnimModelPipeline;
	Shader* m_depthPassAnimModelShader;

	std::deque<Buffer*> m_objectUniformBufferList;
	//ObjectUniformData m_object_uniform_data;
	std::deque<ObjectUniformData> m_objectUniformDataList;

	DepthPassContextType m_depth_pass_context_type;
	std::deque<Buffer*> m_boneTransformsUniformBufferList;
};

