#include "RenderTarget.h"
#include "DXResourceLoader.h"


/*RenderTarget::RenderTarget(const RenderTargetDesc& desc)
	:m_render_target_desc(desc),
	m_out_texture(nullptr, desc.m_texture_desc)
{
}*/

RenderTarget::RenderTarget(const RenderTargetDesc& desc)
	:m_desc(desc)
{}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::Init(RenderTargetDesc& desc)
{
}

const Texture* RenderTarget::get_texture() const
{
	return m_texture;
}

Texture* RenderTarget::get_texture()
{
	return m_texture;
}

const RenderTargetDesc& RenderTarget::get_desc() const
{
	return m_desc;
}

const ClearValue& RenderTarget::get_clear_value() const
{
	return m_desc.m_texture_desc.m_clearVal;
}

ID3D11RenderTargetView* RenderTarget::get_rtv()
{
	return m_p_render_target_view;
}

void RenderTarget::Release()
{
	//TODO:
	if (m_texture)
	{
		if (DXResourceLoader::is_depth_format(m_texture->get_desc().m_imageFormat))
		{
			SafeRelease(m_p_depth_stencil_view);
		}
		else
		{
			SafeRelease(m_p_render_target_view);
		}
		SafeReleaseDelete(m_texture);
	}
}