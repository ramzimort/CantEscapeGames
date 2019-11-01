#pragma once
#include "Common_Renderer.h"

class RenderTarget;

struct ClearValue
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		struct
		{
			float  m_depth;
			uint32_t m_stencil;
		};
	};
};


enum SampleCount
{
	SAMPLE_COUNT_1 = 1,
	SAMPLE_COUNT_2 = 2,
	SAMPLE_COUNT_4 = 4
};



struct TextureDesc
{
	bool m_isSRGB;
	uint32_t m_arraySize;
	uint32_t m_bindFlags;
	Usage_Type m_usageType;
	CPU_Access_Type m_cpuAccessType;
	uint32_t m_mipLevels;
	uint32_t m_depth;
	int32_t m_width;
	int32_t m_height;
	DXGI_FORMAT m_imageFormat;
	uint32_t m_miscFlags;
	ClearValue m_clearVal;
	SampleCount m_sampleCount = SAMPLE_COUNT_1;
};


struct TextureLoadDesc
{
	TextureDesc* m_tex_desc = nullptr;
	std::string m_file_name = "";
	bool m_useDXLoader = true;
	bool m_use_ex_flag = false;
	bool m_generateMipMap = false;
	bool m_isRawDataFromInternalMemory = false;
	uint32_t mRawDataByteTotalSize = 0;
	const void* m_rawData = nullptr;
	uint32_t m_rawDataOnePixelSize = 0;
};


class Texture {
public:
	friend class DXResourceLoader;
	friend class DXRenderer;
public:
	Texture(const TextureDesc& texture_desc);
    virtual ~Texture();

	void Release();
	const TextureDesc& get_desc() const { return m_texture_desc; };
	int32_t GetWidth() const { return m_texture_desc.m_width; };
	int32_t GetHeight() const { return m_texture_desc.m_height; };
private:
	TextureDesc m_texture_desc;
	ID3D11Resource* m_p_raw_resource;
	ID3D11ShaderResourceView* m_p_srv;
	ID3D11UnorderedAccessView** m_pp_uav;
};
