#pragma once

#include "Common_Renderer.h"
#include "DescriptorData.h"
#include "Pipeline.h"
#include "D3D_Commands.h"
#include "RenderTarget.h"

struct SwapChain
{
	IDXGISwapChain* m_d3d_swap_chain;
	RenderTarget* m_p_swap_chain_render_target;
};

struct DXDescriptorDataReference
{
	Buffer* p_buffer;
	Texture* p_texture;
	Sampler* p_sampler;
};


class DXRenderer
{
public:
	friend class DXResourceLoader;
public:
	DXRenderer(HWND window_handle, bool enable_vsync);
	~DXRenderer();

	ID3D11Device* get_device() const;
	ID3D11DeviceContext* get_device_context() const;

	void cmd_bind_render_targets(RenderTarget** color_rts,
		uint32_t color_rts_count, RenderTarget* depth_stencil_rt, const LoadActionsDesc& load_actions_desc);

	void cmd_bind_pipeline(Pipeline* pipeline);
	void cmd_bind_descriptor(Pipeline* pipeline,
		uint32_t descriptor_count, DescriptorData* descriptor_data);
	void cmd_bind_vertex_buffer(Buffer* buffer);
	void cmd_bind_index_buffer(Buffer* buffer);
	void cmd_set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	void cmd_draw(uint32_t vertex_count, uint32_t first_vertex);
	void cmd_draw_index(uint32_t indices_count, uint32_t first_index, uint32_t first_vertex);
	void cmd_draw_index_instanced(uint32_t instance_count, uint32_t first_instance, uint32_t indices_count,
		uint32_t first_index, uint32_t first_vertex);

	//TODO: temmp function / cmd, should use staging buffer way to update gpu buffer
	void cmd_update_buffer(const BufferUpdateDesc& buffer_update_desc);

	void instant_update_buffer(const BufferUpdateDesc& buffer_update_desc);

	void present_swap_chain();
	void execute_queued_cmd();

	void Release();


	bool init(uint32_t swap_chain_sample_count);
	void init_default_resources();
	void init_transient_buffer();

	const SwapChain* GetSwapChain() const;
	SwapChain* GetSwapChain();
	SwapChain& GetRefSwapChain();
	const SwapChain& GetRefSwapChain() const;

	HWND get_window_handle() const;
	std::string get_selected_gpu_name() const { return m_cur_gpu_name; }
private:
	bool init_d3d11(uint32_t swap_chain_sample_count);

	void reset_shader_resources();
	void reset_shader_uavs();
	void set_shader_resources(uint32_t binding_loc, Shader_Stages shader_stages,
		uint32_t num_resources, ID3D11ShaderResourceView** pp_resource_views);

	void set_samplers(uint32_t binding_loc, Shader_Stages shader_stages,
		uint32_t num_resources, ID3D11SamplerState** sampler_states);

	void set_constant_buffer(uint32_t binding_loc, Shader_Stages shader_stages,
		uint32_t num_resources, ID3D11Buffer** pp_buffers);

	void map_buffer(Buffer* buffer);
	void unmap_buffer(Buffer* buffer);
private:
	HWND m_window_handle;
	bool m_enable_vysnc;

	BlendState* m_p_default_blend_state;
	RasterizerState* m_p_default_rasterizer_state;

	//member data that is constantly changing every frame

	Buffer* m_p_transient_constant_buffer;

	std::vector<DescriptorData> m_descriptor_data_list;

	std::vector<DXDescriptorDataReference> m_dxdescriptor_data_reference_list;


	std::vector<DXCMD> m_cmd_list;

	SwapChain m_swap_chain;

	ID3D11Device* m_d3d_device;
	ID3D11DeviceContext* m_d3d_device_context;
	IDXGIFactory1* m_dxgi_factory;

	IDXGIAdapter1* m_cur_adapter;
	std::string m_cur_gpu_name;
};

