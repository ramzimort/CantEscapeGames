#pragma once

#include "Common_Renderer.h"
#include "DescriptorData.h"
#include "Pipeline.h"
#include "D3D_Commands.h"
#include "RenderTarget.h"
#include <list>
#include <string>

/**
 * @file D3D11_Renderer.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public middle level interface for executing DirectX 11 API
 */

struct SwapChain
{
	IDXGISwapChain1* m_d3d_swap_chain;
	RenderTarget* m_p_swap_chain_render_target;
};

struct DXDescriptorDataReference
{
	Buffer* p_buffer;
	Texture* p_texture;
	Sampler* p_sampler;
};

/**
 * @brief he public middle level interface for executing DirectX 11 API
 * 
 */
class DXRenderer
{
public:
	friend class DXResourceLoader;
	friend class EventManager;
	friend class ResourceManager;
public:
	/**
	* @brief Construct a new DXRenderer object
	* 
	* @param window_handle 
	* @param enable_vsync 
	*/
	DXRenderer(HWND window_handle, bool enable_vsync);
	~DXRenderer();
	/**
	 * @brief Get the d3d11 device 
	 * 
	 * @return ID3D11Device* 
	 */
	ID3D11Device* get_device() const;

	/**
	 * @brief Get the d3d11 device context
	 * 
	 * @return ID3D11DeviceContext* 
	 */
	ID3D11DeviceContext* get_device_context() const;

	/**
	 * @brief Bind Render Targets
	 * 
	 * @param color_rts 
	 * @param color_rts_count 
	 * @param depth_stencil_rt 
	 * @param load_actions_desc 
	 */
	void cmd_bind_render_targets(RenderTarget** color_rts,
		uint32_t color_rts_count, RenderTarget* depth_stencil_rt, const LoadActionsDesc& load_actions_desc);

	/**
	 * @brief Bind Render Targets
	 * 
	 * @param color_rts 
	 * @param color_rts_count 
	 * @param depth_stencil_rt 
	 * @param load_actions_desc 
	 */
	void cmd_bind_render_targets(RenderTarget** color_rts,
		uint32_t color_rts_count, RenderTarget* depth_stencil_rt, const LoadActionsDesc* load_actions_desc);

	/**
	 * @brief Bind graphics/compute pipeline.
	 * 
	 * @param pipeline 
	 */
	void cmd_bind_pipeline(Pipeline* pipeline);
	/**
	 * @brief Bind hader resources (textures, samplers, buffers)
	 * 
	 * @param pipeline 
	 * @param descriptor_count 
	 * @param descriptor_data 
	 */
	void cmd_bind_descriptor(Pipeline* pipeline,
		uint32_t descriptor_count, DescriptorData* descriptor_data);
	
	/**
	 * @brief Bind Vertex Buffer
	 * 
	 * @param buffer 
	 */
	void cmd_bind_vertex_buffer(Buffer* buffer);
	/**
	 * @brief Bind Index Buffer
	 * 
	 * @param buffer 
	 */
	void cmd_bind_index_buffer(Buffer* buffer);
	/**
	 * @brief Set viewport
	 * 
	 * @param x 
	 * @param y 
	 * @param width 
	 * @param height 
	 */
	void cmd_set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	/**
	 * @brief Execute Draw call
	 * 
	 * @param vertex_count 
	 * @param first_vertex 
	 */
	void cmd_draw(uint32_t vertex_count, uint32_t first_vertex);
	/**
	 * @brief Execture Draw Index
	 * 
	 * @param indices_count 
	 * @param first_index 
	 * @param first_vertex 
	 */
	void cmd_draw_index(uint32_t indices_count, uint32_t first_index, uint32_t first_vertex);
	/**
	 * @brief Execute Draw Index Instanced
	 * 
	 * @param instance_count 
	 * @param first_instance 
	 * @param indices_count 
	 * @param first_index 
	 * @param first_vertex 
	 */
	void cmd_draw_index_instanced(uint32_t instance_count, uint32_t first_instance, uint32_t indices_count,
		uint32_t first_index, uint32_t first_vertex);
	
	/**
	 * @brief Dispatch compute shader thread
	 * 
	 * @param thread_group_x 
	 * @param thread_group_y 
	 * @param thread_group_z 
	 */
	void cmd_dispatch(uint32_t thread_group_x, uint32_t thread_group_y, uint32_t thread_group_z);
	/**
	 * @brief Bind streamout vertex buffer
	 * 
	 * @param streamoutVertexBuffer 
	 * @param offsets 
	 */
	void cmd_bind_streamout_render_targets(Buffer* streamoutVertexBuffer, uint32_t offsets);

	/**
	 * @brief Draw Auto
	 * 
	 */
	void cmd_draw_auto();
	/**
	 * @brief Execute begin sprite batch
	 * 
	 * @param pSpriteBatch 
	 */
	void cmd_spritebatch_begin(DirectX::SpriteBatch* pSpriteBatch);
	/**
	 * @brief Exectue end sprite batch
	 * 
	 * @param pSpriteBatch 
	 */
	void cmd_spritebatch_end(DirectX::SpriteBatch* pSpriteBatch);
	/**
	 * @brief Execute internal text rendering command
	 * 
	 * @param pSpriteBatch 
	 * @param text 
	 * @param pFont 
	 * @param position 
	 * @param color 
	 * @param scale 
	 * @param rotation 
	 */
	void cmd_draw_font_text_string(DirectX::SpriteBatch* pSpriteBatch, const std::wstring& text, DirectX::SpriteFont* pFont,
		const Vector2& position, const Vector3& color, const Vector3& scale, float rotation);

	
	/**
	 * @brief Execute the buffer update command
	 * 
	 * @param buffer_update_desc 
	 */
	void cmd_update_buffer(const BufferUpdateDesc& buffer_update_desc);

	void instant_update_buffer(const BufferUpdateDesc& buffer_update_desc);

	/**
	 * @brief Present the internal swap chain
	 * 
	 */
	void present_swap_chain();
	/**
	 * @brief Execute all queued render command list
	 * 
	 */
	void execute_queued_cmd();

	/**
	 * @brief Release all internal renderer resources
	 * 
	 */
	void Release();

	/**
	 * @brief Init the renderer's swap chain
	 * 
	 * @param swap_chain_sample_count 
	 * @return true 
	 * @return false 
	 */
	bool init(uint32_t swap_chain_sample_count);
	void init_default_resources();
	void init_transient_buffer();

	/**
	 * @brief Get the Swap Chain
	 * 
	 * @return const SwapChain* 
	 */
	const SwapChain* GetSwapChain() const;
	/**
	 * @brief Get the Swap Chain
	 * 
	 * @return SwapChain* 
	 */
	SwapChain* GetSwapChain();
	/**
	 * @brief Get the Ref Swap Chain
	 * 
	 * @return SwapChain& 
	 */
	SwapChain& GetRefSwapChain();
	/**
	 * @brief Get the Ref Swap Chain object
	 * 
	 * @return const SwapChain& 
	 */
	const SwapChain& GetRefSwapChain() const;

	/**
	 * @brief Get the window handle object
	 * 
	 * @return HWND 
	 */
	HWND get_window_handle() const;
	std::string get_selected_gpu_name() const { return m_cur_gpu_name; }
private:
	bool init_d3d11(uint32_t swap_chain_sample_count);

	void reset_shader_resources();
	void reset_shader_uavs();

	void set_uav_resources(uint32_t binding_loc, uint32_t num_resources,
		ID3D11UnorderedAccessView** pp_uav_views);


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
	Buffer* m_p_transient_constant_buffer;

	std::vector <DescriptorData> m_descriptor_data_list;
	std::vector<DXDescriptorDataReference> m_dxdescriptor_data_reference_list;
	std::vector<DXCMD> m_cmd_list;
	std::deque< DXCMD_DrawFontTextStringReference> m_drawFontTextStringReferenceList;

	SwapChain m_swap_chain;

	ID3D11Device* m_d3d_device;
	ID3D11DeviceContext* m_d3d_device_context;
	IDXGIFactory1* m_dxgi_factory;

	IDXGIAdapter1* m_cur_adapter;
	std::string m_cur_gpu_name;
	std::mutex m_mutex;
};

