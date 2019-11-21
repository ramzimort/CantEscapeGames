
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
	RenderTarget* curMainRT = appRendererContext.m_appRendererInstance->m_curMainRT;
	int32_t rt_width, rt_height;
	rt_width = curMainRT->get_texture()->GetWidth();
	rt_height = curMainRT->get_texture()->GetHeight();

	m_dxrenderer->cmd_spritebatch_begin(m_spriteBatch);
	for (uint64_t index = 0; index < m_appRenderer->m_textFontInstanceRenderDataList.size(); ++index)
	{
		const TextFontInstanceRenderData& textFontInstanceRenderData = m_appRenderer->m_textFontInstanceRenderDataList[index];
		StringId fontID = m_textRendering.m_fontTypeToResourceStringId[textFontInstanceRenderData.m_fontType];
		DirectX::SpriteFont* curFont = m_appRenderer->m_resourceManager->GetFont(fontID);
		Vector2 finalPosition(textFontInstanceRenderData.m_position.x * (float)rt_width, textFontInstanceRenderData.m_position.y * (float)rt_height);
		m_dxrenderer->cmd_draw_font_text_string(m_spriteBatch, textFontInstanceRenderData.m_text, 
			curFont, finalPosition, textFontInstanceRenderData.m_color, textFontInstanceRenderData.m_scale);
	}
	m_dxrenderer->cmd_spritebatch_end(m_spriteBatch);
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