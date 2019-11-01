#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "Shader.h"
#include "Pipeline.h"
#include "RasterizerState.h"
#include "DepthState.h"
#include "BlendState.h"
#include "Sampler.h"

#include <vector>

class DXRenderer;


class DXResourceLoader{
public:
	DXResourceLoader();
	~DXResourceLoader();

	static Buffer* Create_Buffer(DXRenderer* renderer, BufferLoadDesc& load_desc);
	static void Update_Buffer(DXRenderer* renderer, BufferUpdateDesc& buffer_update_desc);
	static RenderTarget* Create_RenderTarget(DXRenderer* renderer, RenderTargetDesc& render_target_desc);

	static Texture* Create_Texture(DXRenderer* renderer, TextureLoadDesc& load_desc);
	static Shader* Create_Shader(DXRenderer* renderer, const ShaderLoadDesc& shader_load_desc);

	static Pipeline* Create_Pipeline(DXRenderer* renderer, 
		const PipelineDesc& pipeline_desc);
	static RasterizerState* Create_RasterizerState(DXRenderer* renderer,
		const RasterizerStateDesc& rasterizer_desc);
	static DepthState* Create_DepthState(DXRenderer* renderer,
		const DepthStateDesc& depth_state_desc);

	static BlendState* Create_BlendState(DXRenderer* dxrenderer,
		const BlendStateDesc& blend_state_desc);

	static Sampler* Create_Sampler(DXRenderer* renderer,
		const SamplerDesc& sampler_desc);

	static bool is_depth_format(DXGI_FORMAT image_format);
	static uint32_t ImageFormatToElementCount(DXGI_FORMAT imageFormat);
private:
	static D3D11_USAGE Usage_Type_To_D3D11_Usage(Usage_Type usage_type);
	static UINT CPU_Access_To_D3D11_CPU_Access(CPU_Access_Type cpu_access_type);
	static D3D11_BIND_FLAG Bind_Flags_To_D3D11_Bind_Flags(uint32_t bind_flags);

	static D3D11_FILTER SamplerDesc_To_D3D11_Filter(const SamplerDesc& sampler_desc);
	static D3D11_COMPARISON_FUNC CompareFunc_To_D3D11_ComparisonFunc(CompareFunc compare_func);
	static D3D11_TEXTURE_ADDRESS_MODE AddressMode_To_D3D11_AddressMode(SamplerAddressMode sampler_address_mode);

	static D3D11_RESOURCE_MISC_FLAG misc_flags_to_d3d11_misc_flags(uint32_t misc_flags);

	static Pipeline* Create_GraphicsPipeline(DXRenderer* renderer, const PipelineDesc& pipeline_desc,
		const GraphicsPipelineDesc& graphics_pipeline_desc);

	static Pipeline* Create_ComputePipeline(DXRenderer* renderer, const PipelineDesc& pipeline_desc,
		const ComputePipelineDesc& graphics_pipeline_desc);

	static Texture* Create_TextureFromFile(DXRenderer* renderer, TextureLoadDesc& load_desc);
	static Texture* Create_TextureFromMemory(DXRenderer* renderer, TextureLoadDesc& load_desc);

	static void Add_RenderTarget_View(
		DXRenderer* renderer, 
		RenderTarget* render_target, uint32_t mip_slice, 
		ID3D11RenderTargetView** d3d_render_target_view);
	static void Add_DepthStencil_View(DXRenderer* renderer, 
		RenderTarget* render_target, uint32_t mip_slice,
		ID3D11DepthStencilView** d3d_depth_stencil_view);

	static uint32_t round_16_bytes_alignment(uint32_t size);
private:
};

