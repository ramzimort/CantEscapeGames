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


/**
 * @file DXResourceLoader.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public middle level interface for loading DirectX 11 graphic resources
 */

class DXRenderer;

/**
 * @brief The public middle level interface for loading DirectX 11 graphic resources
 * 
 */
class DXResourceLoader{
public:
	DXResourceLoader();
	~DXResourceLoader();

	/**
	 * @brief Create Buffer
	 * 
	 * @param renderer 
	 * @param load_desc 
	 * @return Buffer* 
	 */
	static Buffer* Create_Buffer(DXRenderer* renderer, BufferLoadDesc& load_desc);
	static void Update_Buffer(DXRenderer* renderer, BufferUpdateDesc& buffer_update_desc);
	/**
	 * @brief Create Render Target
	 * 
	 * @param renderer 
	 * @param render_target_desc 
	 * @return RenderTarget* 
	 */
	static RenderTarget* Create_RenderTarget(DXRenderer* renderer, RenderTargetDesc& render_target_desc);

	/**
	 * @brief Create Texture either from hard disk, internal raw memory(raw pointer), or create empty texture for render target or straight up normal empty texture
	 * 
	 * @param renderer 
	 * @param load_desc 
	 * @return Texture* 
	 */
	static Texture* Create_Texture(DXRenderer* renderer, TextureLoadDesc& load_desc);
	/**
	 * @brief Create Shader
	 * 
	 * @param renderer 
	 * @param shader_load_desc 
	 * @return Shader* 
	 */
	static Shader* Create_Shader(DXRenderer* renderer, const ShaderLoadDesc& shader_load_desc);
	/**
	 * @brief Create Pipeline. DX11 input layout is initialized here as well
	 * 
	 * @param renderer 
	 * @param pipeline_desc 
	 * @return Pipeline* 
	 */
	static Pipeline* Create_Pipeline(DXRenderer* renderer, 
		const PipelineDesc& pipeline_desc);
	/**
	 * @brief Create RasterizerState
	 * 
	 * @param renderer 
	 * @param rasterizer_desc 
	 * @return RasterizerState* 
	 */
	static RasterizerState* Create_RasterizerState(DXRenderer* renderer,
		const RasterizerStateDesc& rasterizer_desc);
	/**
	 * @brief Create DepthState
	 * 
	 * @param renderer 
	 * @param depth_state_desc 
	 * @return DepthState* 
	 */
	static DepthState* Create_DepthState(DXRenderer* renderer,
		const DepthStateDesc& depth_state_desc);

	/**
	 * @brief Create BlendState
	 * 
	 * @param dxrenderer 
	 * @param blend_state_desc 
	 * @return BlendState* 
	 */
	static BlendState* Create_BlendState(DXRenderer* dxrenderer,
		const BlendStateDesc& blend_state_desc);
	/**
	 * @brief Create Sampler
	 * 
	 * @param renderer 
	 * @param sampler_desc 
	 * @return Sampler* 
	 */
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

