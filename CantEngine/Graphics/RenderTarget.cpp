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

ID3D11RenderTargetView* RenderTarget::get_rtv(uint32_t mip_slice)
{
	uint32_t final_mip_slice = mip_slice >= m_desc.m_texture_desc.m_mipLevels ? (m_desc.m_texture_desc.m_mipLevels - 1) : mip_slice;
	return m_pp_rendertargetview[final_mip_slice];
}

void RenderTarget::Release()
{
	if (m_texture)
	{
		bool is_depth = DXResourceLoader::is_depth_format(m_texture->get_desc().m_imageFormat);

		for (uint32_t i = 0; i < m_desc.m_texture_desc.m_mipLevels; ++i)
		{
			if (is_depth)
			{
				SafeRelease(m_pp_depth_stencil_view[i]);
			}
			else
			{
				SafeRelease(m_pp_rendertargetview[i]);
			}
		}
		if (is_depth)
		{
			free(m_pp_depth_stencil_view);
		}
		else
		{
			free(m_pp_rendertargetview);
		}
		SafeReleaseDelete(m_texture);
	}
}