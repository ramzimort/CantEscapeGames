#pragma once

#include "Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/InstanceRenderData.h"
#include "Managers/CameraManager.h"



class DeferredRenderingInstance;
class DebugRenderingInstance;
class AppRendererInstance;
class MSAAResolvePassInstance;
class ParticleRenderingInstance;



struct AppRendererContext
{
	AppRendererContext(const CameraInfo& cameraInfo) :m_cameraInfo(cameraInfo) {}
	AppRendererInstance* m_appRendererInstance;
	CameraInfo m_cameraInfo;
};

class AppRenderer;

class AppRendererInstance
{
public:
	friend class DeferredRenderingInstance;
	friend class DebugRenderingInstance;
	friend class MSAAResolvePassInstance;
	friend class ParticleRenderingInstance;
	friend class AppRenderer;
public:
	AppRendererInstance(AppRenderer* appRenderer,
		DXRenderer* dxrenderer, const CameraInfo& cameraInfo);
	~AppRendererInstance();

	

	void Update(float dt);
	void Render();

	void Initialize();
	void LoadContent();
	void Release();
private:
	void RenderBasicInstances(Pipeline* pipeline);
private:
	DXRenderer* m_dxrenderer;
	AppRenderer* m_appRenderer;
	AppRendererContext m_context;

	RenderTarget* m_finalOutputRT = nullptr;

	RenderTarget* m_curMainRT = nullptr;
	RenderTarget* m_msaaMainRT = nullptr;
	RenderTarget* m_depthRT = nullptr;

	DeferredRenderingInstance* m_deferredRenderingInstance;
	DebugRenderingInstance* m_debugRenderingInstance;
	MSAAResolvePassInstance* m_msaaResolvePassInstance;
	ParticleRenderingInstance* m_particleRenderingInstance;

	std::vector<ObjectUniformData> m_objectUniformDataList;
	BufferList m_objectUniformBufferList;

	CameraUniformData m_camera_uniform_data;
	Buffer* m_camera_uniform_buffer;

	Buffer* m_resolveUniformBuffer;
};

