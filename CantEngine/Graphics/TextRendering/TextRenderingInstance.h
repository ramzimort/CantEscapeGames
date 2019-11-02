#pragma once
#include "Graphics/TextRendering/TextRendering.h"
#include "Graphics/AppRendererInstance.h"
#include "DirectXTK/SpriteBatch.h"

class TextRenderingInstance
{
public:
	TextRenderingInstance(TextRendering& textRendering);
	~TextRenderingInstance();

	void Render(const AppRendererContext& appRendererContext);
	void Initialize(const AppRendererContext& appRendererContext);
	void LoadContent(const AppRendererContext& appRendererContext);
	void Release();
private:
	TextRendering& m_textRendering;
	DXRenderer* m_dxrenderer;
	AppRenderer* m_appRenderer;

	DirectX::SpriteBatch* m_spriteBatch;
};

