#pragma once

#include "Renderer_Includes.h"
#include "Shaders/Shading.h"
#include "Graphics/InstanceRenderData.h"
#include "Managers/CameraManager.h"



class DeferredRenderingInstance;
class DebugRenderingInstance;
class AppRendererInstance;





struct AppRendererContext
{
	AppRendererInstance* m_appRendererInstance;
	CameraInfo m_cameraInfo;
	DeferredRenderingInstance* m_deferredRenderingInstance;
	DebugRenderingInstance* m_debugRenderingInstance;
};

class AppRenderer;

class AppRendererInstance
{
public:
	friend class DeferredRenderingInstance;
	friend class DebugRenderingInstance;
	friend class MSAAResolvePassInstance;
	friend class AppRenderer;
public:
	AppRendererInstance(AppRenderer* appRenderer,
		DXRenderer* dxrenderer, const AppRendererContext& context);
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

	std::vector<ObjectUniformData> m_objectUniformDataList;
	BufferList m_objectUniformBufferList;

	CameraUniformData m_camera_uniform_data;
	Buffer* m_camera_uniform_buffer;
};
