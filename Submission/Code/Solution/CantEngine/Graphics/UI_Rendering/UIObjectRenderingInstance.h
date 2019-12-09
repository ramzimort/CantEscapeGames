#pragma once
#include "Graphics/UI_Rendering/UIObjectRendering.h"
#include "Graphics/AppRendererInstance.h"

class UIObjectRenderingInstance
{
public:
	UIObjectRenderingInstance(UIObjectRendering& uiObjectRendering);
	~UIObjectRenderingInstance();

	void Render(const AppRendererContext& appRendererContext);
	void Initialize(const AppRendererContext& appRendererContext);
	void LoadContent(const AppRendererContext& appRendererContext);
	void Release();
private:
	void AddUIObjectUniformBuffer();
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;

	UIObjectRendering& m_uiObjectRendering;
	std::deque<UIObjectUniformData> m_uiObjectUniformDataList;
	BuffersDeque m_uiObjectUniformBufferList;
	Matrix m_projectionMatrix;
};

