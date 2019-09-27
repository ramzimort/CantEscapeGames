#include "Texture.h"

Texture::Texture(const TextureDesc& texture_desc)
	:m_texture_desc(texture_desc),
	m_p_raw_resource(nullptr),
	m_p_srv(nullptr),
	m_pp_uav(nullptr)
{
	memcpy(&m_texture_desc, &texture_desc, sizeof(TextureDesc));
}

Texture::~Texture() 
{
}

void Texture::Release()
{
	SafeRelease(m_p_raw_resource);
	SafeRelease(m_p_srv);

	if (m_pp_uav)
	{
		for (uint32_t mip_index = 0; mip_index < m_texture_desc.m_mipLevels; ++mip_index)
		{
			SafeRelease(m_pp_uav[mip_index]);
		}
		free(m_pp_uav);
	}
}