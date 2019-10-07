#pragma once
#include "Common_Renderer.h"
#include <memory>

class Buffer;

enum IndexType
{
	INDEX_UINT16,
	INDEX_UINT32,
	INDEX_TOTAL_COUNT
};

struct BufferDesc
{
	uint32_t m_bindFlags;
	Usage_Type m_usageType;
	CPU_Access_Type m_cpuAccessType;
	Misc_Flags m_miscFlags;
	uint32_t m_vertexStride;
	IndexType m_indexType;
	uint32_t m_firstElement;
	uint32_t m_elementCount;
	uint32_t m_structureStride;
	//applicatable for Buffer<T> in unordered access
	DXGI_FORMAT m_bufferFormat;
	std::string m_debugName;
};

struct BufferLoadDesc
{
	BufferLoadDesc(const void* raw_data = nullptr)
		: m_desc(),
		m_rawData(raw_data) {};
	BufferDesc m_desc;
	const void* m_rawData;
	uint32_t m_size;
};

struct BufferUpdateDesc
{
	Buffer* m_buffer;
	const void* m_pSource;
	uint32_t m_sourceOffset;
	uint32_t m_destOffset;
	uint32_t m_size;
};

class Buffer
{
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	~Buffer();

	
	void Release();
private:
	Buffer(const BufferDesc& buffer_desc);
	const BufferDesc& get_desc() const;
private:
	ID3D11Buffer* m_p_buffer;
	ID3D11ShaderResourceView* m_srv;
	ID3D11UnorderedAccessView* m_uav;
	DXGI_FORMAT m_dxgi_index_format;
	BufferDesc m_desc;
	uint32_t m_buffer_size;
	void* m_mapped_address;
};

