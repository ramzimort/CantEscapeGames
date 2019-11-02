#include "stdafx.h"
#include "TextRenderingInstance.h"
#include "Graphics/GraphicsSettings.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Material.h"
#include "Managers/ResourceManager.h"

TextRenderingInstance::TextRenderingInstance(TextRendering& textRendering)
	:m_textRendering(textRendering)
{
	m_appRenderer = m_textRendering.m_appRenderer;
	m_dxrenderer = m_textRendering.m_dxrenderer;
}


TextRenderingInstance::~TextRenderingInstance()
{
}

void TextRenderingInstance::Render(const AppRendererContext& appRendererContext)
{
	m_spriteBatch->Begin();
	for (uint64_t index = 0; index < m_appRenderer->m_textFontInstanceRenderDataList.size(); ++index)
	{
		const TextFontInstanceRenderData& textFontInstanceRenderData = m_appRenderer->m_textFontInstanceRenderDataList[index];
		StringId fontID = m_textRendering.m_fontTypeToResourceStringId[textFontInstanceRenderData.m_fontType];
		DirectX::SpriteFont* curFont = m_appRenderer->m_resourceManager->GetFont(fontID);
		curFont->DrawString(m_spriteBatch, textFontInstanceRenderData.m_text.c_str(),
			textFontInstanceRenderData.m_position, textFontInstanceRenderData.m_color, 
			0.f, Vector3(0.f), textFontInstanceRenderData.m_scale);
	}
	m_spriteBatch->End();
}

void TextRenderingInstance::Initialize(const AppRendererContext& appRendererContext)
{
	m_spriteBatch = new DirectX::SpriteBatch(m_dxrenderer->get_device_context());
}

void TextRenderingInstance::LoadContent(const AppRendererContext& appRendererContext)
{

}

void TextRenderingInstance::Release()
{
	delete m_spriteBatch;
}