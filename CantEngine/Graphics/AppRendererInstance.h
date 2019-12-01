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
class UIObjectRenderingInstance;
class TextRenderingInstance;

struct SkyboxUniformData
{
	//no camera position included here
	Matrix ModelViewProjectionMat;
};

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
	friend class UIObjectRenderingInstance;
	friend class TextRenderingInstance;
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
	void RenderBoneMeshInstances(Pipeline* pipeline);
	void RenderSkybox();
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
	UIObjectRenderingInstance* m_uiObjectRenderingInstance;
	TextRenderingInstance* m_textRenderingInstance;

	std::deque<ObjectUniformData> m_objectUniformDataList;
	BuffersDeque m_objectUniformBufferList;
	std::deque<ObjectUniformData> m_boneMeshObjectUniformDataList;
	BuffersDeque m_boneMeshObjectUniformBufferList;

	CameraUniformData m_camera_uniform_data;
	Buffer* m_camera_uniform_buffer;

	Buffer* m_resolveUniformBuffer;

	uint32_t m_lastMaterialIndex;

	SkyboxUniformData m_skyboxUniformData;
	Buffer* m_skyboxUniformBuffer;
};

