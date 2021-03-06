
#include "DXResourceLoader.h"
#include "Buffer.h"
#include "Texture.h"
#include "stb_image.h"
#include "RenderTarget.h"
#include "d3dUtils.h"
#include "D3D11_Renderer.h"
#include "Memory/CantMemory.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


D3D11_BLEND_OP g_d3d11_blend_operator_converter[BlendOperator::BO_TOTAL_COUNTS] =
{
	D3D11_BLEND_OP_ADD,
	D3D11_BLEND_OP_SUBTRACT,
	D3D11_BLEND_OP_REV_SUBTRACT,
	D3D11_BLEND_OP_MIN,
	D3D11_BLEND_OP_MAX,
};

D3D11_BLEND g_d3d11_blend_factor_converter[BlendFactor::BF_TOTAL_COUNTS] =
{
	D3D11_BLEND_ZERO,
	D3D11_BLEND_ONE,
	D3D11_BLEND_SRC_COLOR,
	D3D11_BLEND_INV_SRC_COLOR,
	D3D11_BLEND_DEST_COLOR,
	D3D11_BLEND_INV_DEST_COLOR,
	D3D11_BLEND_SRC_ALPHA,
	D3D11_BLEND_INV_SRC_ALPHA,
	D3D11_BLEND_DEST_ALPHA,
	D3D11_BLEND_INV_DEST_ALPHA,
	D3D11_BLEND_SRC_ALPHA_SAT,
	D3D11_BLEND_BLEND_FACTOR,
	D3D11_BLEND_INV_BLEND_FACTOR,
};


//uint32_t ImageFormatToBytesValue(DXGI_FORMAT imageFormat)
//{
//	if (imageFormat >= 1 && imageFormat <= 4)
//	{
//		return sizeof(float) * 4;
//	}
//	else if(imageFormat >= )
//}

uint32_t DXResourceLoader::ImageFormatToElementCount(DXGI_FORMAT imageFormat)
{
	if (imageFormat >= 1 && imageFormat <= 4)
	{
		return 4;
	}
	else if (imageFormat >= 5 && imageFormat <= 8)
	{
		return 3;
	}
	else if (imageFormat >= 9 && imageFormat <= 14)
	{
		return 4;
	}
	else if (imageFormat >= 15 && imageFormat <= 18)
	{
		return 2;
	}
	else if (imageFormat >= 27 && imageFormat <= 32)
	{
		return 4;
	}
	else if (imageFormat >= 33 && imageFormat <= 38)
	{
		return 2;
	}
	else if (imageFormat >= 40 && imageFormat <= 43)
	{
		return 1;
	}
	else if (imageFormat >= 53 && imageFormat <= 59)
	{
		return 1;
	}
	return 0;
}



bool DXResourceLoader::is_depth_format(DXGI_FORMAT image_format)
{
	return image_format >= DXGI_FORMAT_D32_FLOAT && image_format <= DXGI_FORMAT_D24_UNORM_S8_UINT;
}

D3D11_FILTER DXResourceLoader::SamplerDesc_To_D3D11_Filter(const SamplerDesc& sampler_desc)
{
	bool has_compare_func_enabled = (sampler_desc.m_compare_func != CompareFunc::CMP_NEVER);

	if (sampler_desc.m_max_aniso > 0)
	{
		return has_compare_func_enabled ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;
	}

	//based on https://github.com/ConfettiFX/The-Forge/blob/master/Common_3/DXRenderer/Direct3D11/Direct3D11.cpp
	// control bit : minFilter  magFilter   mipMapMode
	//   point   :   00	  00	   00
	//   linear  :   01	  01	   01
	// ex : trilinear == 010101
	int filter = (sampler_desc.m_min_filter << 4) | (sampler_desc.m_mag_filter << 2) | sampler_desc.m_mipmap_mode;
	int base_filter = has_compare_func_enabled ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D11_FILTER_MIN_MAG_MIP_POINT;
	return (D3D11_FILTER)(base_filter + filter);
}

D3D11_COMPARISON_FUNC DXResourceLoader::CompareFunc_To_D3D11_ComparisonFunc(CompareFunc compare_func)
{
	D3D11_COMPARISON_FUNC d3d_comparison_func = D3D11_COMPARISON_NEVER;
	switch (compare_func)
	{
	case CompareFunc::CMP_LESS: d3d_comparison_func = D3D11_COMPARISON_LESS; break;
	case CompareFunc::CMP_EQUAL: d3d_comparison_func = D3D11_COMPARISON_EQUAL; break;
	case CompareFunc::CMP_LEQUAL: d3d_comparison_func = D3D11_COMPARISON_LESS_EQUAL; break;
	case CompareFunc::CMP_GREATER: d3d_comparison_func = D3D11_COMPARISON_GREATER; break;
	case CompareFunc::CMP_NOTEQUAL: d3d_comparison_func = D3D11_COMPARISON_NOT_EQUAL; break;
	case CompareFunc::CMP_GEQUAL: d3d_comparison_func = D3D11_COMPARISON_GREATER_EQUAL; break;
	case CompareFunc::CMP_ALWAYS: d3d_comparison_func = D3D11_COMPARISON_ALWAYS; break;
	default:break;
	}

	return d3d_comparison_func;
}

D3D11_TEXTURE_ADDRESS_MODE DXResourceLoader::AddressMode_To_D3D11_AddressMode(SamplerAddressMode sampler_address_mode)
{
	D3D11_TEXTURE_ADDRESS_MODE d3d_texture_address_mode = (D3D11_TEXTURE_ADDRESS_MODE)(0);

	switch (sampler_address_mode)
	{
	case SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_BORDER: d3d_texture_address_mode = D3D11_TEXTURE_ADDRESS_BORDER;  break;
	case SamplerAddressMode::ADDRESS_MODE_CLAMP_TO_EDGE: d3d_texture_address_mode = D3D11_TEXTURE_ADDRESS_CLAMP;  break;
	case SamplerAddressMode::ADDRESS_MODE_MIRROR: d3d_texture_address_mode = D3D11_TEXTURE_ADDRESS_MIRROR; break;
	case SamplerAddressMode::ADDRESS_MODE_REPEAT: d3d_texture_address_mode = D3D11_TEXTURE_ADDRESS_WRAP; break;
	default:break;
	}

	return d3d_texture_address_mode;
}

D3D11_BIND_FLAG DXResourceLoader::Bind_Flags_To_D3D11_Bind_Flags(uint32_t bind_flags)
{
	uint32_t d3d11_final_bind_flag = 0;

	if (bind_flags & Bind_Flags::BIND_DEPTH_STENCIL)
	{
		d3d11_final_bind_flag |= D3D11_BIND_DEPTH_STENCIL;
	}
	if (bind_flags & Bind_Flags::BIND_VERTEX_BUFFER)
	{
		d3d11_final_bind_flag |= D3D11_BIND_VERTEX_BUFFER;
	}
	if (bind_flags & Bind_Flags::BIND_CONSTANT_BUFFER)
	{
		d3d11_final_bind_flag |= D3D11_BIND_CONSTANT_BUFFER;
	}
	if (bind_flags & Bind_Flags::BIND_INDEX_BUFFER)
	{
		d3d11_final_bind_flag |= D3D11_BIND_INDEX_BUFFER;
	}
	if (bind_flags & Bind_Flags::BIND_SHADER_RESOURCE)
	{
		d3d11_final_bind_flag |= D3D11_BIND_SHADER_RESOURCE;
	}
	if (bind_flags & Bind_Flags::BIND_UNORDERED_ACCESS)
	{
		d3d11_final_bind_flag |= D3D11_BIND_UNORDERED_ACCESS;
	}
	if (bind_flags & Bind_Flags::BIND_RENDER_TARGET)
	{
		d3d11_final_bind_flag |= D3D11_BIND_RENDER_TARGET;
	}

	if (bind_flags & Bind_Flags::BIND_STREAM_OUTPUT)
	{
		d3d11_final_bind_flag |= D3D11_BIND_STREAM_OUTPUT;
	}


	return static_cast<D3D11_BIND_FLAG>(d3d11_final_bind_flag);
}



D3D11_USAGE DXResourceLoader::Usage_Type_To_D3D11_Usage(Usage_Type usage_type)
{
	D3D11_USAGE final_usage = D3D11_USAGE_DEFAULT;
	switch (usage_type)
	{
	case Usage_Type::USAGE_DYNAMIC: final_usage = D3D11_USAGE_DYNAMIC; break;
	case Usage_Type::USAGE_IMMUTABLE: final_usage = D3D11_USAGE_IMMUTABLE; break;
	case Usage_Type::USAGE_STAGING: final_usage = D3D11_USAGE_STAGING; break;
	default:break;
	}
	return final_usage;
}

UINT DXResourceLoader::CPU_Access_To_D3D11_CPU_Access(CPU_Access_Type cpu_access_type)
{
	UINT final_access_flag = 0;

	switch (cpu_access_type)
	{
	case CPU_Access_Type::ACCESS_READ: final_access_flag = D3D11_CPU_ACCESS_READ; break;
	case CPU_Access_Type::ACCESS_WRITE: final_access_flag = D3D11_CPU_ACCESS_WRITE; break;
	case CPU_Access_Type::ACCESS_WRITE_READ: final_access_flag = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE; break;
	default:break;
	}
	return final_access_flag;
}


D3D11_RESOURCE_MISC_FLAG DXResourceLoader::misc_flags_to_d3d11_misc_flags(uint32_t misc_flags)
{
	uint32_t d3d_misc_flag = 0;

	if (misc_flags & MISC_TEXTURE_CUBE)
	{
		d3d_misc_flag |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	}
	if (misc_flags & MISC_GENERATE_MIPS)
	{
		d3d_misc_flag |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}

	if (misc_flags & MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		d3d_misc_flag |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	}

	if (misc_flags & MISC_BUFFER_STRUCTURED)
	{
		d3d_misc_flag |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	}

	return static_cast<D3D11_RESOURCE_MISC_FLAG>(d3d_misc_flag);
}


DXResourceLoader::DXResourceLoader()
{
}


DXResourceLoader::~DXResourceLoader()
{
}



void DXResourceLoader::Add_RenderTarget_View(DXRenderer* renderer,
	RenderTarget* render_target,
	uint32_t mip_slice,
	ID3D11RenderTargetView** d3d_render_target_view)
{
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
	rtv_desc.Format = render_target->m_desc.m_texture_desc.m_imageFormat;

	D3D11_RESOURCE_DIMENSION resource_dim_type = {};
	render_target->m_texture->m_p_raw_resource->GetType(&resource_dim_type);

	switch (resource_dim_type)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		//TODO:
		break;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		D3D11_TEXTURE2D_DESC d3d_tex2d_desc = {};
		static_cast<ID3D11Texture2D*>(render_target->m_texture->m_p_raw_resource)->GetDesc(&d3d_tex2d_desc);

		if (d3d_tex2d_desc.SampleDesc.Count > 1)
		{
			if (d3d_tex2d_desc.ArraySize > 1)
			{
				//TODO: add array MS support
			}
			else
			{
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
		}
		else
		{
			//TODO: add support for multi sample texture2d render target
			rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtv_desc.Texture2D.MipSlice = mip_slice;
		}
		break;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		//TODO
		break;
	}
	default:
		break;

	}
	HRESULT hr = renderer->get_device()->CreateRenderTargetView(
		render_target->m_texture->m_p_raw_resource, &rtv_desc, d3d_render_target_view);

	if (FAILED_HR(hr))
	{
		assert(false == true);
	}
}


void DXResourceLoader::Add_DepthStencil_View(DXRenderer* renderer, RenderTarget* render_target, uint32_t mip_slice,
	ID3D11DepthStencilView** d3d_depth_stencil_view)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
	dsv_desc.Format = render_target->m_desc.m_texture_desc.m_imageFormat;

	D3D11_RESOURCE_DIMENSION resource_dim_type = {};
	render_target->m_texture->m_p_raw_resource->GetType(&resource_dim_type);

	switch (resource_dim_type)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		//TODO:
		break;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		D3D11_TEXTURE2D_DESC d3d_tex2d_desc = {};
		static_cast<ID3D11Texture2D*>(render_target->m_texture->m_p_raw_resource)->GetDesc(&d3d_tex2d_desc);
		if (d3d_tex2d_desc.SampleDesc.Count > 1)
		{
			if (d3d_tex2d_desc.ArraySize > 1)
			{
				//TODO: add array MS support for depth stencil_view
			}
			else
			{
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
		}
		else
		{
			//TODO: add support for multi sample texture2d render target
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Texture2D.MipSlice = mip_slice;
		}
		break;
	}
	default:
		break;
	}

	HRESULT hr = renderer->get_device()->CreateDepthStencilView(
		render_target->m_texture->m_p_raw_resource, &dsv_desc, d3d_depth_stencil_view);

	if (FAILED_HR(hr))
	{
		assert(false == true);
	}
}




Buffer* DXResourceLoader::Create_Buffer(DXRenderer* renderer, BufferLoadDesc& load_desc)
{
	D3D11_BUFFER_DESC d3d_buffer_desc;
	ZeroMemory(&d3d_buffer_desc, sizeof(D3D11_BUFFER_DESC));


	d3d_buffer_desc.CPUAccessFlags = CPU_Access_To_D3D11_CPU_Access(load_desc.m_desc.m_cpuAccessType);
	d3d_buffer_desc.Usage = Usage_Type_To_D3D11_Usage(load_desc.m_desc.m_usageType);

	if (d3d_buffer_desc.Usage != D3D11_USAGE_STAGING)
	{
		d3d_buffer_desc.BindFlags = Bind_Flags_To_D3D11_Bind_Flags(static_cast<Bind_Flags>(load_desc.m_desc.m_bindFlags));
	}

	uint32_t finalSize = load_desc.m_size;
	if (load_desc.m_desc.m_bindFlags & Bind_Flags::BIND_CONSTANT_BUFFER)
	{
		finalSize = round_16_bytes_alignment(finalSize);
	}

	d3d_buffer_desc.ByteWidth = (UINT)finalSize;
	d3d_buffer_desc.StructureByteStride = (load_desc.m_desc.m_miscFlags & Misc_Flags::MISC_BUFFER_ALLOW_RAW_VIEWS) != 0 ? 0 : (UINT)load_desc.m_desc.m_structureStride;
	d3d_buffer_desc.MiscFlags = misc_flags_to_d3d11_misc_flags(load_desc.m_desc.m_miscFlags);

	ID3D11Buffer* new_d3d_buffer = nullptr;

	if (load_desc.m_rawData != nullptr)
	{
		D3D11_SUBRESOURCE_DATA resource_data;
		ZeroMemory(&resource_data, sizeof(D3D11_SUBRESOURCE_DATA));
		resource_data.pSysMem = load_desc.m_rawData;

		HRESULT hr = renderer->get_device()->CreateBuffer(&d3d_buffer_desc,
			&resource_data, &new_d3d_buffer);

		if (FAILED_HR(hr))
		{
			return nullptr;
		}
	}
	else
	{
		HRESULT hr = renderer->get_device()->CreateBuffer(&d3d_buffer_desc,
			nullptr, &new_d3d_buffer);

		if (FAILED_HR(hr))
		{
			return nullptr;
		}
	}

	Buffer* buffer = new Buffer(load_desc.m_desc);
	buffer->m_p_buffer = new_d3d_buffer;
	buffer->m_buffer_size = load_desc.m_size;

	if (load_desc.m_desc.m_bindFlags == Bind_Flags::BIND_INDEX_BUFFER)
	{
		buffer->m_dxgi_index_format = load_desc.m_desc.m_indexType == IndexType::INDEX_UINT16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	}



	//TODO if Buffer<> add SRV pointer

	if (load_desc.m_desc.m_bindFlags & Bind_Flags::BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
		srv_desc.Format = DXGI_FORMAT_UNKNOWN;
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		//srv_desc.Buffer.FirstElement = (UINT)load_desc.m_desc.m_firstElement;

		srv_desc.Buffer.NumElements = static_cast<UINT>(load_desc.m_desc.m_elementCount);
		//srv_desc.Buffer.ElementWidth = static_cast<UINT>(load_desc.m_desc.m_structureStride);
		srv_desc.Buffer.ElementOffset = 0;

		if (load_desc.m_desc.m_miscFlags & Misc_Flags::MISC_BUFFER_ALLOW_RAW_VIEWS)
		{
			srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
		}

		if (srv_desc.Format != DXGI_FORMAT_UNKNOWN)
		{
			srv_desc.Buffer.ElementWidth = 0;
		}

		HRESULT hr = renderer->get_device()->CreateShaderResourceView(buffer->m_p_buffer, &srv_desc, &buffer->m_srv);

		if (FAILED_HR(hr))
		{
			assert(false == true);
			return nullptr;
		}
	}

	if (load_desc.m_desc.m_bindFlags & Bind_Flags::BIND_UNORDERED_ACCESS)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Format = DXGI_FORMAT_UNKNOWN;
		uav_desc.Buffer.FirstElement = static_cast<UINT>(load_desc.m_desc.m_firstElement);
		uav_desc.Buffer.NumElements = static_cast<UINT>(load_desc.m_desc.m_elementCount);
		uav_desc.Buffer.Flags = 0;

		if (load_desc.m_desc.m_miscFlags & Misc_Flags::MISC_BUFFER_ALLOW_RAW_VIEWS)
		{
			uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
			uav_desc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
		}
		else
		{
			uav_desc.Format = load_desc.m_desc.m_bufferFormat;
		}

		renderer->get_device()->CreateUnorderedAccessView(new_d3d_buffer, &uav_desc, &buffer->m_uav);
	}



	//TODO if RWBuffer<> add UAV pointer
	return buffer;
}


RenderTarget* DXResourceLoader::Create_RenderTarget(DXRenderer* renderer, RenderTargetDesc& render_target_desc)
{
	TextureLoadDesc load_tex_desc = {};
	load_tex_desc.m_file_name = "";
	load_tex_desc.m_rawData = nullptr;
	load_tex_desc.m_tex_desc = &render_target_desc.m_texture_desc;

	Texture* rt_texture = Create_Texture(renderer, load_tex_desc);

	RenderTarget* render_target = new RenderTarget(render_target_desc);
	render_target->m_texture = rt_texture;

	bool is_depth = is_depth_format(load_tex_desc.m_tex_desc->m_imageFormat);

	uint32_t mip_levels = std::max(render_target_desc.m_texture_desc.m_mipLevels, 1u);
	if (is_depth)
	{
		render_target->m_pp_depth_stencil_view = (ID3D11DepthStencilView**)malloc(sizeof(ID3D11DepthStencilView*) * mip_levels);
	}
	else
	{
		render_target->m_pp_rendertargetview = (ID3D11RenderTargetView**)malloc(sizeof(ID3D11RenderTargetView*) * mip_levels);
	}


	for (uint32_t i = 0; i < mip_levels; ++i)
	{

		if (is_depth)
		{
			Add_DepthStencil_View(renderer, render_target, i, &render_target->m_pp_depth_stencil_view[i]);
		}
		else
		{
			Add_RenderTarget_View(renderer, render_target, i, &render_target->m_pp_rendertargetview[i]);
		}
	}

	return render_target;
}

Texture* DXResourceLoader::Create_Texture(DXRenderer* renderer, TextureLoadDesc& load_desc)
{
	if (load_desc.m_file_name != "")
	{
		return Create_TextureFromFile(renderer, load_desc);
	}

	if (load_desc.m_isRawDataFromInternalMemory)
	{
		return Create_TextureFromMemory(renderer, load_desc);
	}

	D3D11_SUBRESOURCE_DATA raw_d3d_data = {};

	bool hasRawData = load_desc.m_rawData != nullptr;
	
	if (hasRawData)
	{
		raw_d3d_data.pSysMem = load_desc.m_rawData;
		raw_d3d_data.SysMemPitch = (UINT)load_desc.m_tex_desc->m_width  * load_desc.m_rawDataOnePixelSize;
		raw_d3d_data.SysMemSlicePitch = 0;
	}

	uint32_t arraySize = std::max(load_desc.m_tex_desc->m_arraySize, 1u);

	Texture* texture = CantMemory::PoolResource<Texture>::Allocate(*load_desc.m_tex_desc);

	bool is_depth_texture = DXResourceLoader::is_depth_format(load_desc.m_tex_desc->m_imageFormat);
	DXGI_FORMAT final_image_format = load_desc.m_tex_desc->m_imageFormat;
	if (is_depth_texture)
	{
		if (final_image_format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		{
			final_image_format = DXGI_FORMAT_R24G8_TYPELESS;
		}
		else if (final_image_format == DXGI_FORMAT_D32_FLOAT)
		{
			final_image_format = DXGI_FORMAT_R32_TYPELESS;
		}
	}

	if (load_desc.m_tex_desc->m_depth > 1)
	{
		D3D11_TEXTURE3D_DESC d3d_texture3d_desc;
		ZeroMemory(&d3d_texture3d_desc, sizeof(D3D11_TEXTURE3D_DESC));

		d3d_texture3d_desc.BindFlags = Bind_Flags_To_D3D11_Bind_Flags(load_desc.m_tex_desc->m_bindFlags);
		d3d_texture3d_desc.CPUAccessFlags = CPU_Access_To_D3D11_CPU_Access(load_desc.m_tex_desc->m_cpuAccessType);
		d3d_texture3d_desc.Format = load_desc.m_tex_desc->m_imageFormat;
		d3d_texture3d_desc.Depth = load_desc.m_tex_desc->m_depth;
		d3d_texture3d_desc.Width = load_desc.m_tex_desc->m_width;
		d3d_texture3d_desc.Height = load_desc.m_tex_desc->m_height;
		d3d_texture3d_desc.MipLevels = load_desc.m_tex_desc->m_mipLevels;
		d3d_texture3d_desc.Usage = Usage_Type_To_D3D11_Usage(load_desc.m_tex_desc->m_usageType);
		d3d_texture3d_desc.MiscFlags = misc_flags_to_d3d11_misc_flags(load_desc.m_tex_desc->m_miscFlags);

		ID3D11Texture3D* new_d3d_texture3d = nullptr;
		HRESULT hr = renderer->get_device()->CreateTexture3D(&d3d_texture3d_desc, nullptr, &new_d3d_texture3d);

		if (FAILED_HR(hr))
		{
			assert(false == true);
		}

		texture->m_p_raw_resource = static_cast<ID3D11Resource*>(new_d3d_texture3d);

	}
	else if (load_desc.m_tex_desc->m_height > 1)
	{
		D3D11_TEXTURE2D_DESC d3d_texture2d_desc;
		ZeroMemory(&d3d_texture2d_desc, sizeof(D3D11_TEXTURE2D_DESC));

		d3d_texture2d_desc.ArraySize = arraySize;
		d3d_texture2d_desc.BindFlags = Bind_Flags_To_D3D11_Bind_Flags(load_desc.m_tex_desc->m_bindFlags);
		d3d_texture2d_desc.CPUAccessFlags = CPU_Access_To_D3D11_CPU_Access(load_desc.m_tex_desc->m_cpuAccessType);
		d3d_texture2d_desc.Format = final_image_format;
		d3d_texture2d_desc.Width = load_desc.m_tex_desc->m_width;
		d3d_texture2d_desc.Height = load_desc.m_tex_desc->m_height;
		d3d_texture2d_desc.MipLevels = load_desc.m_tex_desc->m_mipLevels;
		d3d_texture2d_desc.SampleDesc.Count = (UINT)load_desc.m_tex_desc->m_sampleCount;
		d3d_texture2d_desc.SampleDesc.Quality = 0;
		d3d_texture2d_desc.Usage = Usage_Type_To_D3D11_Usage(load_desc.m_tex_desc->m_usageType);
		d3d_texture2d_desc.MiscFlags = misc_flags_to_d3d11_misc_flags(load_desc.m_tex_desc->m_miscFlags);

		ID3D11Texture2D* d3d_texture2d = nullptr;

		HRESULT hr;

		if (hasRawData)
		{
			hr = renderer->get_device()->CreateTexture2D(&d3d_texture2d_desc,
				&raw_d3d_data, &d3d_texture2d);
		}
		else
		{
			hr = renderer->get_device()->CreateTexture2D(&d3d_texture2d_desc,
				nullptr, &d3d_texture2d);
		}

		if (FAILED_HR(hr))
		{
			assert(false == true);
		}
		texture->m_p_raw_resource = static_cast<ID3D11Resource*>(d3d_texture2d);
	}
	else
	{
		D3D11_TEXTURE1D_DESC d3d_texture1d_desc;
		ZeroMemory(&d3d_texture1d_desc, sizeof(D3D11_TEXTURE1D_DESC));

		d3d_texture1d_desc.ArraySize = arraySize;
		d3d_texture1d_desc.BindFlags = Bind_Flags_To_D3D11_Bind_Flags(load_desc.m_tex_desc->m_bindFlags);
		d3d_texture1d_desc.CPUAccessFlags = CPU_Access_To_D3D11_CPU_Access(load_desc.m_tex_desc->m_cpuAccessType);
		d3d_texture1d_desc.Format = final_image_format;
		d3d_texture1d_desc.Width = load_desc.m_tex_desc->m_width;
		d3d_texture1d_desc.MipLevels = load_desc.m_tex_desc->m_mipLevels;
		d3d_texture1d_desc.Usage = Usage_Type_To_D3D11_Usage(load_desc.m_tex_desc->m_usageType);
		d3d_texture1d_desc.MiscFlags = misc_flags_to_d3d11_misc_flags(load_desc.m_tex_desc->m_miscFlags);

		ID3D11Texture1D* d3d_texture1d = nullptr;
		HRESULT hr;

		if (hasRawData)
		{
			hr = renderer->get_device()->CreateTexture1D(&d3d_texture1d_desc,
				&raw_d3d_data, &d3d_texture1d);
		}
		else
		{
			hr = renderer->get_device()->CreateTexture1D(&d3d_texture1d_desc,
				nullptr, &d3d_texture1d);
		}

		if (FAILED_HR(hr))
		{
			assert(false == true);
		}
		texture->m_p_raw_resource = static_cast<ID3D11Resource*>(d3d_texture1d);
	}

	D3D11_RESOURCE_DIMENSION resource_dim_type = {};
	texture->m_p_raw_resource->GetType(&resource_dim_type);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc = {};

	switch (resource_dim_type)
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		//TODO:
		D3D11_TEXTURE1D_DESC d3d_tex1d_desc = {};
		static_cast<ID3D11Texture1D*>(texture->m_p_raw_resource)->GetDesc(&d3d_tex1d_desc);

		if (d3d_tex1d_desc.ArraySize > 1)
		{

		}
		else
		{
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srv_desc.Texture1D.MipLevels = d3d_tex1d_desc.MipLevels;
			srv_desc.Texture1D.MostDetailedMip = 0;

			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
			uav_desc.Texture1D.MipSlice = 0;
		}


		break;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		D3D11_TEXTURE2D_DESC d3d_tex2d_desc = {};
		static_cast<ID3D11Texture2D*>(texture->m_p_raw_resource)->GetDesc(&d3d_tex2d_desc);

		if ((d3d_tex2d_desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) != 0)
		{
			srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;

			srv_desc.TextureCube.MipLevels = d3d_tex2d_desc.MipLevels;
			srv_desc.TextureCube.MostDetailedMip = 0;

			uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			uav_desc.Texture2DArray.MipSlice = 0;
			uav_desc.Texture2DArray.FirstArraySlice = 0;
			uav_desc.Texture2DArray.ArraySize = d3d_tex2d_desc.ArraySize;
		}
		else
		{
			if (d3d_tex2d_desc.ArraySize > 1)
			{
				if (d3d_tex2d_desc.SampleDesc.Count > 1)
				{
				}
				else
				{
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;

					srv_desc.Texture2DArray.MipLevels = d3d_tex2d_desc.MipLevels;
					srv_desc.Texture2DArray.ArraySize = 0;
					srv_desc.Texture2DArray.MostDetailedMip = 0;
					srv_desc.Texture2DArray.ArraySize = d3d_tex2d_desc.ArraySize;

					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
					uav_desc.Texture2DArray.MipSlice = 0;
					uav_desc.Texture2DArray.FirstArraySlice = 0;
					uav_desc.Texture2DArray.ArraySize = d3d_tex2d_desc.ArraySize;
				}
			}
			else
			{
				if (d3d_tex2d_desc.SampleDesc.Count > 1)
				{
					//TODO: add Array support
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				}
				else
				{

					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					srv_desc.Texture2D.MipLevels = d3d_tex2d_desc.MipLevels;
					srv_desc.Texture2D.MostDetailedMip = 0;

					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
					uav_desc.Texture2D.MipSlice = 0;
				}
			}
		}
		break;
	}
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		//TODO:
		break;
	}
	default:
		break;
	}

	if (load_desc.m_tex_desc->m_bindFlags & BIND_SHADER_RESOURCE)
	{

		DXGI_FORMAT srv_tex_format = final_image_format;

		if (is_depth_texture)
		{
			if (srv_tex_format == DXGI_FORMAT_R24G8_TYPELESS)
			{
				//srv_tex_format = DXGI_FORMAT_R24G8_FLOAT;
			}
			else if (srv_tex_format == DXGI_FORMAT_R32_TYPELESS)
			{
				srv_tex_format = DXGI_FORMAT_R32_FLOAT;
			}
		}

		srv_desc.Format = srv_tex_format;

		renderer->get_device()->CreateShaderResourceView(
			texture->m_p_raw_resource, &srv_desc, &texture->m_p_srv);
	
		if (load_desc.m_generateMipMap)
		{
			std::lock_guard<std::mutex> lock(renderer->m_mutex);

			renderer->get_device_context()->GenerateMips(texture->m_p_srv);
		}
	}

	//TODO
	if (load_desc.m_tex_desc->m_bindFlags & BIND_UNORDERED_ACCESS)
	{
		uav_desc.Format = load_desc.m_tex_desc->m_imageFormat;
		
		texture->m_pp_uav = (ID3D11UnorderedAccessView**)(malloc(sizeof(ID3D11UnorderedAccessView*) * load_desc.m_tex_desc->m_mipLevels));
		for (uint32_t mip_index = 0; mip_index < load_desc.m_tex_desc->m_mipLevels; ++mip_index)
		{
			switch (resource_dim_type)
			{
			case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
			{
				uav_desc.Texture1D.MipSlice = mip_index;
				break;
			}
			case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				uav_desc.Texture2D.MipSlice = mip_index;
				break;
			}
			default:
				break;
			}

			renderer->get_device()->CreateUnorderedAccessView(
				texture->m_p_raw_resource, &uav_desc, &texture->m_pp_uav[mip_index]);
		}
	}


	return texture;
}

struct STBI_Image_Data
{
	void* pixels;
	uint32_t line_width;
};

Texture* DXResourceLoader::Create_TextureFromFile(DXRenderer* renderer, TextureLoadDesc& load_desc)
{
	//std::lock_guard<std::mutex> lock(renderer->m_mutex);

	bool isHDRImage = load_desc.m_file_name.find(".hdr") != std::string::npos;

	if (load_desc.m_useDXLoader && !isHDRImage)
	{
		ID3D11Resource* d3d_tex2d = nullptr;
		ID3D11ShaderResourceView* d3d_tex2d_srv = nullptr;
		std::wstring load_file_name_w(load_desc.m_file_name.begin(), load_desc.m_file_name.end());

		if (load_desc.m_use_ex_flag)
		{
			HRESULT hr = DirectX::CreateWICTextureFromFileEx(renderer->get_device(), renderer->get_device_context(), load_file_name_w.c_str(), 0,
				Usage_Type_To_D3D11_Usage(load_desc.m_tex_desc->m_usageType), Bind_Flags_To_D3D11_Bind_Flags(load_desc.m_tex_desc->m_bindFlags),
				CPU_Access_To_D3D11_CPU_Access(load_desc.m_tex_desc->m_cpuAccessType), 0, 0, &d3d_tex2d, &d3d_tex2d_srv);

			if (FAILED_HR(hr))
			{
				assert(0);
				return nullptr;
			}
		}
		else
		{
			HRESULT hr = DirectX::CreateWICTextureFromFile(renderer->get_device(),
				renderer->get_device_context(), load_file_name_w.c_str(), &d3d_tex2d, &d3d_tex2d_srv, 0);

			if (FAILED_HR(hr))
			{
				assert(0);
				return nullptr;
			}
		}

		ID3D11Texture2D* casted_tex2d = static_cast<ID3D11Texture2D*>(d3d_tex2d);
		D3D11_TEXTURE2D_DESC tex2d_desc = {};
		casted_tex2d->GetDesc(&tex2d_desc);

		TextureDesc texture_desc = {};
		texture_desc.m_width = tex2d_desc.Width;
		texture_desc.m_height = tex2d_desc.Height;

		Texture* texture = CantMemory::PoolResource<Texture>::Allocate(texture_desc);
		texture->m_p_raw_resource = d3d_tex2d;
		texture->m_p_srv = d3d_tex2d_srv;

		return texture;
	}

	//TEMPORARY code
	//this is used to load HDR image mostly for now
	STBI_Image_Data stbi_image_data = {};

	int32_t width, height, channel;
	stbi_image_data.pixels = stbi_loadf(load_desc.m_file_name.c_str(), &width, &height, &channel, STBI_rgb_alpha);
	stbi_image_data.line_width = width * sizeof(float) * 4;

	if (stbi_image_data.pixels)
	{

		std::vector<D3D11_SUBRESOURCE_DATA> initial_data_list;
		std::vector<float*> initial_color_list;

		initial_data_list.reserve(16);
		initial_color_list.reserve(16);

		int32_t min_size = std::min(width, height);

		while (min_size >= 1)
		{
			initial_data_list.push_back(D3D11_SUBRESOURCE_DATA{});
			initial_color_list.push_back(nullptr);

			float*& cur_color_list = initial_color_list.back();
			uint64_t color_sizes_width = width * 4 * sizeof(float);
			uint64_t color_sizes = color_sizes_width * height;

			cur_color_list = (float*)malloc(color_sizes);

			std::memset(cur_color_list, 0u, color_sizes);

			D3D11_SUBRESOURCE_DATA& d3d_subresource_data = initial_data_list.back();
			d3d_subresource_data.pSysMem = &cur_color_list[0];
			d3d_subresource_data.SysMemPitch = (UINT)color_sizes_width;
			d3d_subresource_data.SysMemSlicePitch = 0;
			min_size = min_size >> 1;
		}

		ID3D11Texture2D* d3d_tex2d = nullptr;
		ID3D11ShaderResourceView* d3d_srv = nullptr;

		D3D11_TEXTURE2D_DESC tex2d_desc = {};
		tex2d_desc.ArraySize = 1;
		tex2d_desc.MipLevels = (UINT)initial_data_list.size();
		tex2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tex2d_desc.CPUAccessFlags = 0;
		tex2d_desc.Usage = D3D11_USAGE_DEFAULT;
		tex2d_desc.Width = width;
		tex2d_desc.Height = height;
		tex2d_desc.SampleDesc.Count = 1;
		tex2d_desc.SampleDesc.Quality = 0;
		tex2d_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tex2d_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_d3d_desc = {};
		srv_d3d_desc.Format = tex2d_desc.Format;
		srv_d3d_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv_d3d_desc.Texture2D.MipLevels = tex2d_desc.MipLevels;
		srv_d3d_desc.Texture2D.MostDetailedMip = 0;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_d3d_desc = {};
		uav_d3d_desc.Format = tex2d_desc.Format;
		uav_d3d_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;


		initial_data_list[0].pSysMem = stbi_image_data.pixels;
		initial_data_list[0].SysMemPitch = (UINT)stbi_image_data.line_width;

		HRESULT hr = renderer->get_device()->CreateTexture2D(&tex2d_desc,
			&initial_data_list[0], &d3d_tex2d);

		stbi_image_free(stbi_image_data.pixels);

		for (float* rawfloat : initial_color_list)
		{
			free(rawfloat);
		}

		if (FAILED_HR(hr))
		{
			assert(0);
			return nullptr;
		}
		hr = renderer->get_device()->CreateShaderResourceView(d3d_tex2d, &srv_d3d_desc, &d3d_srv);

		if (FAILED_HR(hr))
		{
			assert(0);
			return nullptr;
		}

		renderer->get_device_context()->GenerateMips(d3d_srv);

		TextureDesc texture_desc = {};
		texture_desc.m_width = tex2d_desc.Width;
		texture_desc.m_height = tex2d_desc.Height;
		texture_desc.m_mipLevels = tex2d_desc.MipLevels;
		texture_desc.m_depth = 1;
		texture_desc.m_clearVal = ClearValue{ 0.f, 0.f, 0.f, 0.f };

		Texture* texture = CantMemory::PoolResource<Texture>::Allocate(texture_desc);
		texture->m_p_raw_resource = d3d_tex2d;
		texture->m_p_srv = d3d_srv;

		return texture;
	}

	assert(0);
	return nullptr;
}


Texture* DXResourceLoader::Create_TextureFromMemory(DXRenderer* renderer, TextureLoadDesc& load_desc)
{
	ID3D11Resource* d3d_tex2d = nullptr;
	ID3D11ShaderResourceView* d3d_tex2d_srv = nullptr;
	if (load_desc.m_use_ex_flag)
	{
		HRESULT hr = DirectX::CreateWICTextureFromMemoryEx(renderer->get_device(),
			renderer->get_device_context(), (const uint8_t*)load_desc.m_rawData, load_desc.m_rawDataByteTotalSize, load_desc.m_rawDataByteTotalSize,
			Usage_Type_To_D3D11_Usage(load_desc.m_tex_desc->m_usageType), Bind_Flags_To_D3D11_Bind_Flags(load_desc.m_tex_desc->m_bindFlags),
			CPU_Access_To_D3D11_CPU_Access(load_desc.m_tex_desc->m_cpuAccessType), 0, 0, &d3d_tex2d, &d3d_tex2d_srv);

		if (FAILED_HR(hr))
		{
			assert(0);
			return nullptr;
		}
	}
	else
	{
		HRESULT hr = DirectX::CreateWICTextureFromMemory(renderer->get_device(), renderer->get_device_context(),
			(const uint8_t*)load_desc.m_rawData, load_desc.m_rawDataByteTotalSize, &d3d_tex2d, &d3d_tex2d_srv);

		if (FAILED_HR(hr))
		{
			assert(0);
			return nullptr;
		}
	}

	ID3D11Texture2D* casted_tex2d = static_cast<ID3D11Texture2D*>(d3d_tex2d);
	D3D11_TEXTURE2D_DESC tex2d_desc = {};
	casted_tex2d->GetDesc(&tex2d_desc);

	TextureDesc texture_desc = {};
	texture_desc.m_width = tex2d_desc.Width;
	texture_desc.m_height = tex2d_desc.Height;
	texture_desc.m_depth = 1;
	texture_desc.m_imageFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	Texture* texture = CantMemory::PoolResource<Texture>::Allocate(texture_desc);
	texture->m_p_raw_resource = d3d_tex2d;
	texture->m_p_srv = d3d_tex2d_srv;
	return texture;
}

void DXResourceLoader::Update_Buffer(DXRenderer* renderer, BufferUpdateDesc& buffer_update_desc)
{
	//TODO:
}

Shader* DXResourceLoader::Create_Shader(DXRenderer* renderer, const ShaderLoadDesc& shader_load_desc)
{
	Shader* shader = new Shader(shader_load_desc.m_desc);
	shader->Inner_Initialize(renderer->get_device(), shader_load_desc);
	return shader;
}

Pipeline* DXResourceLoader::Create_Pipeline(DXRenderer* renderer,
	const PipelineDesc& pipeline_desc)
{
	if (pipeline_desc.m_pipeline_type == PipelineType::GRAPHICS)
	{
		return Create_GraphicsPipeline(renderer, pipeline_desc, pipeline_desc.m_graphics_desc);
	}
	else if (pipeline_desc.m_pipeline_type == PipelineType::COMPUTE)
	{
		return Create_ComputePipeline(renderer, pipeline_desc, pipeline_desc.m_compute_desc);
	}
	return nullptr;
}

Pipeline* DXResourceLoader::Create_ComputePipeline(DXRenderer* renderer, const PipelineDesc& pipeline_desc,
	const ComputePipelineDesc& compute_pipeline_desc)
{
	assert(compute_pipeline_desc.m_shader);
	//Pipeline* new_pipeline = CantMemory::PoolResource<Pipeline>::Allocate(pipeline_desc);
	Pipeline* new_pipeline = new Pipeline(pipeline_desc);

	return new_pipeline;
}

//TODO: add for compute pipeline
Pipeline* DXResourceLoader::Create_GraphicsPipeline(DXRenderer* renderer, const PipelineDesc& pipeline_desc,
	const GraphicsPipelineDesc& graphics_pipeline_desc)
{
	ID3D11InputLayout* new_input_layout = nullptr;
	if (graphics_pipeline_desc.m_vertex_layout)
	{
		D3D11_INPUT_ELEMENT_DESC input_element_descs[MAX_VERTEX_LAYOUT_ATTRIB];
		ZeroMemory(&input_element_descs[0],
			sizeof(D3D11_INPUT_ELEMENT_DESC) * MAX_VERTEX_LAYOUT_ATTRIB);


		const VertexLayout& vertex_layout = *graphics_pipeline_desc.m_vertex_layout;
		std::string semantic_name[MAX_VERTEX_LAYOUT_ATTRIB] = { "" };
		for (uint32_t i = 0; i < vertex_layout.m_atrrib_count; ++i)
		{
			const VertexAttrib* vertex_attrib = &vertex_layout.m_attribs[i];

			switch (vertex_attrib->m_semantic)
			{
			case Attrib_Semantic::POSITION: semantic_name[i] = "POSITION"; break;
			case Attrib_Semantic::NORMAL: semantic_name[i] = "NORMAL"; break;
			case Attrib_Semantic::COLOR: semantic_name[i] = "COLOR"; break;
			case Attrib_Semantic::TANGENT: semantic_name[i] = "TANGENT"; break;
			case Attrib_Semantic::BITANGENT: semantic_name[i] = "BITANGENT"; break;
			case Attrib_Semantic::TEXCOORD_0: semantic_name[i] = "TEXCOORD"; break;
			case Attrib_Semantic::TEXCOORD_1: semantic_name[i] = "TEXCOORD"; break;
			case Attrib_Semantic::TEXCOORD_2: semantic_name[i] = "TEXCOORD"; break;
			case Attrib_Semantic::TEXCOORD_3: semantic_name[i] = "TEXCOORD"; break;
			default: break;
			}

			assert(semantic_name[i] != "");

			//semantic index is for semantic with similar names but different id
			//eg texcoord0, texcoord1, etc
			uint32_t semantic_index = 0;

			switch (vertex_attrib->m_semantic)
			{
			case Attrib_Semantic::TEXCOORD_0: semantic_index = 0; break;
			case Attrib_Semantic::TEXCOORD_1: semantic_index = 1; break;
			case Attrib_Semantic::TEXCOORD_2: semantic_index = 2; break;
			case Attrib_Semantic::TEXCOORD_3: semantic_index = 3; break;
			default: break;
			}

			input_element_descs[i].SemanticName = semantic_name[i].c_str();
			input_element_descs[i].InputSlot = vertex_attrib->m_binding;
			input_element_descs[i].SemanticIndex = semantic_index;
			input_element_descs[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			input_element_descs[i].Format = vertex_attrib->m_format;
			input_element_descs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			input_element_descs[i].InstanceDataStepRate = 0;
		}

		ID3DBlob* input_blob = graphics_pipeline_desc.
			m_shader->m_vertex_shader_blob;



		HRESULT hr = renderer->get_device()->CreateInputLayout(input_element_descs,
			vertex_layout.m_atrrib_count,
			input_blob->GetBufferPointer(),
			input_blob->GetBufferSize(),
			&new_input_layout);

		if (FAILED_HR(hr))
		{
			return nullptr;
		}
	}

	Pipeline* new_graphics_pipeline = new Pipeline(pipeline_desc);

	new_graphics_pipeline->m_input_layout = new_input_layout;

	BlendState* cur_blend_state = new_graphics_pipeline->m_desc.m_graphics_desc.m_blend_state;
	BlendState* final_blend_state = cur_blend_state != nullptr ? cur_blend_state : renderer->m_p_default_blend_state;

	new_graphics_pipeline->m_desc.m_graphics_desc.m_blend_state = final_blend_state;


	RasterizerState* cur_rast_state = new_graphics_pipeline->m_desc.m_graphics_desc.m_rasterizer_state;
	RasterizerState* final_rast_state = cur_rast_state != nullptr ? cur_rast_state : renderer->m_p_default_rasterizer_state;

	new_graphics_pipeline->m_desc.m_graphics_desc.m_rasterizer_state = final_rast_state;


	D3D_PRIMITIVE_TOPOLOGY d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	switch (pipeline_desc.m_graphics_desc.m_primitive_topo_type)
	{
	case Primitive_Topology::TOPOLOGY_LINE_LIST: d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case Primitive_Topology::TOPOLOGY_LINE_STRIP: d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
	case Primitive_Topology::TOPOLOGY_POINT_LIST: d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case Primitive_Topology::TOPOLOGY_TRIANGLE_LIST: d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	case Primitive_Topology::TOPOLOGY_TRIANGLE_STRIP: d3d_primitive_topology_type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
	default: break;
	}

	new_graphics_pipeline->m_d3d_primitive_topo_type = d3d_primitive_topology_type;
	return new_graphics_pipeline;
}

RasterizerState* DXResourceLoader::Create_RasterizerState(DXRenderer* renderer,
	const RasterizerStateDesc& rasterizer_desc)
{
	D3D11_RASTERIZER_DESC d3d_rasterizer_desc;
	ZeroMemory(&d3d_rasterizer_desc, sizeof(D3D11_RASTERIZER_DESC));


	D3D11_CULL_MODE cull_mode = D3D11_CULL_NONE;

	switch (rasterizer_desc.m_cull_mode)
	{
	case CullMode::FRONT: cull_mode = D3D11_CULL_FRONT; break;
	case CullMode::BACK: cull_mode = D3D11_CULL_BACK; break;
	default: break;
	}

	D3D11_FILL_MODE fill_mode = D3D11_FILL_SOLID;

	if (rasterizer_desc.m_fill_mode == FillMode::WIREFRAME)
	{
		fill_mode = D3D11_FILL_WIREFRAME;
	}

	d3d_rasterizer_desc.AntialiasedLineEnable = FALSE;
	d3d_rasterizer_desc.CullMode = cull_mode;
	d3d_rasterizer_desc.DepthBias = 0;
	d3d_rasterizer_desc.DepthBiasClamp = 0.f;
	d3d_rasterizer_desc.DepthClipEnable = TRUE;
	d3d_rasterizer_desc.FillMode = fill_mode;
	d3d_rasterizer_desc.FrontCounterClockwise = FALSE;
	d3d_rasterizer_desc.MultisampleEnable = (BOOL)rasterizer_desc.m_multiSample;
	d3d_rasterizer_desc.ScissorEnable = FALSE;
	d3d_rasterizer_desc.SlopeScaledDepthBias = 0.f;


	ID3D11RasterizerState* new_d3d_rasterizer_state = nullptr;

	HRESULT hr = renderer->get_device()->CreateRasterizerState(&d3d_rasterizer_desc, &new_d3d_rasterizer_state);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	//RasterizerState* rasterizerState = CantMemory::PoolResource<RasterizerState>::Allocate(rasterizer_desc);
	RasterizerState* rasterizerState = new RasterizerState(rasterizer_desc);
	rasterizerState->m_d3d_rasterizer_state = new_d3d_rasterizer_state;
	return rasterizerState;
}

DepthState* DXResourceLoader::Create_DepthState(DXRenderer* renderer,
	const DepthStateDesc& depth_state_desc)
{
	D3D11_DEPTH_STENCIL_DESC depth_stencil_state_desc;
	ZeroMemory(&depth_stencil_state_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depth_stencil_state_desc.DepthEnable = depth_state_desc.m_depth_enable;
	depth_stencil_state_desc.DepthWriteMask =
		(depth_state_desc.m_depth_write_mask == DepthWriteMask::DONT_WRITE_DEPTH) ? D3D11_DEPTH_WRITE_MASK_ZERO : D3D11_DEPTH_WRITE_MASK_ALL;

	depth_stencil_state_desc.DepthFunc = CompareFunc_To_D3D11_ComparisonFunc(depth_state_desc.m_depth_compare_func);
	depth_stencil_state_desc.StencilEnable = depth_state_desc.m_stencil_enable;

	ID3D11DepthStencilState* new_depth_stencil_state = nullptr;

	HRESULT hr = renderer->get_device()->CreateDepthStencilState(
		&depth_stencil_state_desc, &new_depth_stencil_state);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	DepthState* depth_state = new DepthState(depth_state_desc);
	depth_state->m_p_depth_stencil_state = new_depth_stencil_state;
	return depth_state;
}


BlendState* DXResourceLoader::Create_BlendState(DXRenderer* dxrenderer,
	const BlendStateDesc& blend_state_desc)
{
	D3D11_BLEND_DESC d3d_blend_desc;
	ZeroMemory(&d3d_blend_desc, sizeof(D3D11_BLEND_DESC));


	ID3D11BlendState* d3d_blend_state = nullptr;

	uint32_t blend_index = 0;

	//TODO: let user have option to set write mask
	static const UINT8 all_write_mask = 0x1 | 0x2 | 0x4 | 0x8;

	d3d_blend_desc.AlphaToCoverageEnable = blend_state_desc.m_enableAlphaCoverage;
	d3d_blend_desc.IndependentBlendEnable = TRUE;
	for (uint32_t i = 0; i < MAX_RENDER_TARGET_ATTACHMENTS; ++i)
	{
		if (blend_state_desc.m_blendStateTarget & (1 << i))
		{
			D3D11_BLEND d3d_src_blend_factor = g_d3d11_blend_factor_converter[blend_state_desc.m_srcFactors[blend_index]];
			D3D11_BLEND d3d_dst_blend_factor = g_d3d11_blend_factor_converter[blend_state_desc.m_dstFactors[blend_index]];

			D3D11_BLEND d3d_src_alpha_blend_factor = g_d3d11_blend_factor_converter[blend_state_desc.m_srcAlphaFactors[blend_index]];
			D3D11_BLEND d3d_dst_alpha_blend_factor = g_d3d11_blend_factor_converter[blend_state_desc.m_dstAlphaFactors[blend_index]];

			BOOL enable_blend = d3d_src_alpha_blend_factor != D3D11_BLEND_ONE ||
				d3d_dst_alpha_blend_factor != D3D11_BLEND_ZERO ||
				d3d_src_blend_factor != D3D11_BLEND_ONE ||
				d3d_dst_blend_factor != D3D11_BLEND_ZERO;


			d3d_blend_desc.RenderTarget[i].BlendEnable = enable_blend;
			d3d_blend_desc.RenderTarget[i].SrcBlend = d3d_src_blend_factor;
			d3d_blend_desc.RenderTarget[i].DestBlend = d3d_dst_blend_factor;
			d3d_blend_desc.RenderTarget[i].SrcBlendAlpha = d3d_src_alpha_blend_factor;
			d3d_blend_desc.RenderTarget[i].DestBlendAlpha = d3d_dst_alpha_blend_factor;

			d3d_blend_desc.RenderTarget[i].BlendOp = g_d3d11_blend_operator_converter[blend_state_desc.m_blendOperator[blend_index]];
			d3d_blend_desc.RenderTarget[i].BlendOpAlpha = g_d3d11_blend_operator_converter[blend_state_desc.m_blendAlphaOperator[blend_index]];
			d3d_blend_desc.RenderTarget[i].RenderTargetWriteMask = all_write_mask;

		}

		if (blend_state_desc.m_individualBlend)
		{
			++blend_index;
		}
	}



	HRESULT hr = dxrenderer->get_device()->CreateBlendState(&d3d_blend_desc, &d3d_blend_state);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	BlendState* p_blend_state = new BlendState(blend_state_desc);
	p_blend_state->m_d3d_blend_state = d3d_blend_state;

	return p_blend_state;
}


Sampler* DXResourceLoader::Create_Sampler(DXRenderer* renderer,
	const SamplerDesc& sampler_desc)
{
	D3D11_SAMPLER_DESC d3d_sampler_desc;
	ZeroMemory(&d3d_sampler_desc, sizeof(D3D11_SAMPLER_DESC));

	d3d_sampler_desc.Filter = SamplerDesc_To_D3D11_Filter(sampler_desc);
	d3d_sampler_desc.AddressU = AddressMode_To_D3D11_AddressMode(sampler_desc.m_address_u);
	d3d_sampler_desc.AddressV = AddressMode_To_D3D11_AddressMode(sampler_desc.m_address_v);
	d3d_sampler_desc.AddressW = AddressMode_To_D3D11_AddressMode(sampler_desc.m_address_w);
	d3d_sampler_desc.MaxAnisotropy = std::max(static_cast<unsigned int>(sampler_desc.m_max_aniso), 1u);
	d3d_sampler_desc.MipLODBias = sampler_desc.m_mip_los_bias;
	d3d_sampler_desc.ComparisonFunc = CompareFunc_To_D3D11_ComparisonFunc(sampler_desc.m_compare_func);
	d3d_sampler_desc.BorderColor[0] = 0.f;
	d3d_sampler_desc.BorderColor[1] = 0.f;
	d3d_sampler_desc.BorderColor[2] = 0.f;
	d3d_sampler_desc.BorderColor[3] = 0.f;
	d3d_sampler_desc.MinLOD = 0;
	d3d_sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* sampler_state = nullptr;
	HRESULT hr = renderer->get_device()->CreateSamplerState(&d3d_sampler_desc, &sampler_state);

	if (FAILED_HR(hr))
	{
		return nullptr;
	}

	Sampler* sampler = new Sampler(sampler_desc);
	sampler->m_sampler_state = sampler_state;
	return sampler;
}


uint32_t DXResourceLoader::round_16_bytes_alignment(uint32_t size)
{
	static const uint32_t byte_align = 16;
	uint32_t remainder = size % byte_align;
	if (remainder == 0)
	{
		return size;
	}
	uint32_t floored_divided_num = size / byte_align;
	uint32_t floored_size = byte_align * floored_divided_num;
	size = byte_align * (floored_divided_num + 1);
	return size;
}