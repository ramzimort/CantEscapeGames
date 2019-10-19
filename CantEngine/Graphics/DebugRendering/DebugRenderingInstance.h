#pragma once
#include "Graphics/DebugRendering/DebugRendering.h"
#include "Graphics/AppRendererInstance.h"

class DebugRenderingInstance
{
public:
	DebugRenderingInstance(DebugRendering& debugRendering);
	~DebugRenderingInstance();


	void Update(const AppRendererContext& appRendererContext, float dt);
	void Render(const AppRendererContext& appRendererContext);
	void Initialize();
	void LoadContent();
	void Release();
private:
	void RenderDebugAABBScene(const AppRendererContext& appRendererContext);
	void RenderDebugLineScene(const AppRendererContext& appRendererContext);
	void RenderDebugSphereScene(const AppRendererContext& appRendererContext);
private:
	DebugRendering& m_debugRendering;
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	Buffer* m_debugSphereInstanceUniformBuffer;
	DebugInstanceUniformData m_debugSphereInstanceUniformData[MAX_WIREFRAME_DEBUG_DRAW];

	Buffer* m_debug_aabb_instance_uniform_buffer;
	DebugInstanceUniformData m_debug_aabb_instance_uniform_data[MAX_WIREFRAME_DEBUG_DRAW];
};

