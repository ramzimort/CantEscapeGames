#include "Texture.h"

Texture::Texture(const TextureDesc& texture_desc)
	:m_texture_desc(texture_desc),
	m_p_raw_resource(nullptr),
	m_p_srv(nullptr),
	m_p_uav(nullptr)
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
	SafeRelease(m_p_uav);
}