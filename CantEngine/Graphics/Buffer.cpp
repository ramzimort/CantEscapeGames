#include "Buffer.h"
#include "DXResourceLoader.h"


Buffer::Buffer(const BufferDesc& buffer_desc)
	:m_desc(buffer_desc),
	m_mapped_address(nullptr)
{
}


Buffer::~Buffer()
{
}

const BufferDesc& Buffer::get_desc() const
{
	return m_desc;
}

void Buffer::Release()
{
	SafeRelease(m_p_buffer);
	SafeRelease(m_srv);
	SafeRelease(m_uav);
}