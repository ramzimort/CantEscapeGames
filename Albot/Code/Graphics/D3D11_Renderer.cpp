
#include "Renderer_Includes.h"
#include "d3dUtils.h"
#include "Common_Renderer.h"
#include <algorithm>


static const float g_target_fps = 60.f;
static const float clear_color_black[4] = { 0.f, 0.f, 0.f, 0.f };


DXRenderer::DXRenderer(HWND window_handle, bool enable_vsync)
	:m_window_handle(window_handle),
	m_enable_vysnc(enable_vsync)
{
}


DXRenderer::~DXRenderer()
{
}

void DXRenderer::Release()
{
	//TODO:
	SafeReleaseDelete(m_p_default_blend_state);
	SafeReleaseDelete(m_p_default_rasterizer_state);
	SafeReleaseDelete(m_swap_chain.m_p_swap_chain_render_target);
	SafeReleaseDelete(m_p_transient_constant_buffer);
	SafeRelease(m_swap_chain.m_d3d_swap_chain);
	SafeRelease(m_d3d_device);
	SafeRelease(m_d3d_device_context);
}

bool DXRenderer::init(uint32_t swap_chain_sample_count)
{
	m_descriptor_data_list.reserve(1000);
	m_dxdescriptor_data_reference_list.reserve(1000);

	if (!init_d3d11(swap_chain_sample_count))
	{
		return false;
	}
	init_default_resources();
	init_transient_buffer();
	return true;
}


void DXRenderer::init_default_resources()
{
	BlendStateDesc default_blend_state_desc = {};
	default_blend_state_desc.m_dstAlphaFactors[0] = BF_ZERO;
	default_blend_state_desc.m_dstFactors[0] = BF_ZERO;
	default_blend_state_desc.m_srcAlphaFactors[0] = BF_ONE;
	default_blend_state_desc.m_srcFactors[0] = BF_ONE;
	default_blend_state_desc.m_blendStateTarget = BlendStateTargets::BLEND_STATE_RT_ALL;
	default_blend_state_desc.m_individualBlend = false;

	m_p_default_blend_state = DXResourceLoader::Create_BlendState(this, default_blend_state_desc);

	RasterizerStateDesc default_rast_state_desc = {};
	default_rast_state_desc.m_cull_mode = CullMode::NONE;
	default_rast_state_desc.m_fill_mode = FillMode::SOLID;

	m_p_default_rasterizer_state = DXResourceLoader::Create_RasterizerState(this, default_rast_state_desc);

}

void DXRenderer::init_transient_buffer()
{
	BufferLoadDesc transient_buffer_desc = {};
	transient_buffer_desc.m_desc.m_bindFlags = Bind_Flags::BIND_CONSTANT_BUFFER;
	transient_buffer_desc.m_desc.m_cpuAccessType = CPU_Access_Type::ACCESS_WRITE;
	transient_buffer_desc.m_desc.m_usageType = Usage_Type::USAGE_DYNAMIC;
	transient_buffer_desc.m_size = 65536;
	transient_buffer_desc.m_rawData = nullptr;

	m_p_transient_constant_buffer = DXResourceLoader::Create_Buffer(this, transient_buffer_desc);
}

ID3D11Device* DXRenderer::get_device() const
{
	return m_d3d_device;
}

ID3D11DeviceContext* DXRenderer::get_device_context() const
{
	return m_d3d_device_context;
}

const SwapChain* DXRenderer::GetSwapChain() const
{
	return &m_swap_chain;
}

SwapChain* DXRenderer::GetSwapChain()
{
	return &m_swap_chain;
}

SwapChain& DXRenderer::GetRefSwapChain()
{
	return m_swap_chain;
}

const SwapChain& DXRenderer::GetRefSwapChain() const
{
	return m_swap_chain;
}

HWND DXRenderer::get_window_handle() const
{
	return m_window_handle;
}

void DXRenderer::cmd_bind_render_targets(RenderTarget** color_rts,
	uint32_t color_rts_count, RenderTarget* depth_stencil_rt, const LoadActionsDesc* load_actions_desc)
{
	if (load_actions_desc)
	{
		cmd_bind_render_targets(color_rts, color_rts_count, depth_stencil_rt, *load_actions_desc);
	}
	else
	{
		LoadActionsDesc local_load_actions_desc = {};
		local_load_actions_desc.m_clear_color_values[0] = ClearValue{ 0.0, 0.0, 0.0, 1.0 };
		local_load_actions_desc.m_load_actions_color[0] = LoadActionType::DONT_CLEAR;
		local_load_actions_desc.m_load_action_depth = LoadActionType::DONT_CLEAR;
		local_load_actions_desc.m_load_action_stencil = LoadActionType::DONT_CLEAR;
		cmd_bind_render_targets(color_rts, color_rts_count, depth_stencil_rt, local_load_actions_desc);
	}
}

void DXRenderer::cmd_bind_render_targets(RenderTarget** color_rts,
	uint32_t color_rts_count, RenderTarget* depth_stencil_rt, const LoadActionsDesc& load_actions_desc)
{
	DXCMD pre_cmd = {};
	pre_cmd.m_type = DXCMD_Type::Bind_RenderTargets;

	m_cmd_list.push_back(pre_cmd);


	DXCMD& cmd = m_cmd_list.back();

	cmd.m_cmd_bind_rendertargets.m_color_rts = color_rts;
	cmd.m_cmd_bind_rendertargets.m_color_rts_count = color_rts_count;
	for (uint32_t i = 0; i < MAX_RENDER_TARGET_ATTACHMENTS; ++i)
	{
		cmd.m_cmd_bind_rendertargets.m_color_mips_levels[i] = 0;
	}

	cmd.m_cmd_bind_rendertargets.m_depth_stencil_rt = depth_stencil_rt;
	cmd.m_cmd_bind_rendertargets.m_depth_mips_level = 0;

	cmd.m_cmd_bind_rendertargets.m_load_actions_desc.
		m_load_action_depth = load_actions_desc.m_load_action_depth;
	cmd.m_cmd_bind_rendertargets.m_load_actions_desc.
		m_load_action_stencil = load_actions_desc.m_load_action_stencil;
	cmd.m_cmd_bind_rendertargets.m_load_actions_desc.
		m_clear_depth_stencil = load_actions_desc.m_clear_depth_stencil;

	for (uint32_t i = 0; i < color_rts_count; ++i)
	{
		cmd.m_cmd_bind_rendertargets.m_load_actions_desc.
			m_load_actions_color[i] = load_actions_desc.m_load_actions_color[i];
		cmd.m_cmd_bind_rendertargets.m_load_actions_desc.
			m_clear_color_values[i] = load_actions_desc.m_clear_color_values[i];
	}

}



void DXRenderer::cmd_bind_pipeline(Pipeline* pipeline)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Bind_Pipeline;
	cmd.m_cmd_bind_pipeline.m_pipeline = pipeline;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_bind_descriptor(Pipeline* pipeline, uint32_t descriptor_count, DescriptorData* descriptor_data)
{
	size_t ori_size = m_descriptor_data_list.size();

	//TODO
	for (uint32_t i = 0; i < descriptor_count; ++i)
	{
		m_descriptor_data_list.push_back(DescriptorData());

		DescriptorData& back_desc = m_descriptor_data_list.back();

		DescriptorData* cur_descriptor = descriptor_data + i;

		uint32_t final_var_count = std::max(cur_descriptor->m_var_count, 1u);

		size_t dx_descriptor_ref_original_size = m_dxdescriptor_data_reference_list.size();

		back_desc.m_binding_location = cur_descriptor->m_binding_location;
		back_desc.m_shader_stages = cur_descriptor->m_shader_stages;
		back_desc.m_var_count = final_var_count;
		back_desc.m_descriptor_type = cur_descriptor->m_descriptor_type;


		for (uint32_t j = 0; j < final_var_count; ++j)
		{
			back_desc.p_sizes[j] = cur_descriptor->p_sizes[j];
			back_desc.p_offsets[j] = cur_descriptor->p_offsets[j];
		}
		//printf("size : %i ", back_desc.p_sizes.size();

		if (cur_descriptor->m_descriptor_type == DescriptorType::DESCRIPTOR_BUFFER ||
			cur_descriptor->m_descriptor_type == DescriptorType::DESCRIPTOR_RW_BUFFER)
		{
			for (uint32_t j = 0; j < final_var_count; ++j)
			{
				DXDescriptorDataReference dx_descriptor_data = {};
				dx_descriptor_data.p_buffer = cur_descriptor->m_buffers[j];
				m_dxdescriptor_data_reference_list.push_back(dx_descriptor_data);
			}

			back_desc.m_buffers = &m_dxdescriptor_data_reference_list[dx_descriptor_ref_original_size].p_buffer;
		}
		else if (cur_descriptor->m_descriptor_type == DescriptorType::DESCRIPTOR_SAMPLER)
		{
			for (uint32_t j = 0; j < final_var_count; ++j)
			{
				DXDescriptorDataReference dx_descriptor_data = {};
				dx_descriptor_data.p_sampler = cur_descriptor->m_samplers[j];
				m_dxdescriptor_data_reference_list.push_back(dx_descriptor_data);
			}

			back_desc.m_samplers = &m_dxdescriptor_data_reference_list[dx_descriptor_ref_original_size].p_sampler;


		}
		else if (cur_descriptor->m_descriptor_type == DescriptorType::DESCRIPTOR_TEXTURE ||
			cur_descriptor->m_descriptor_type == DescriptorType::DESCRIPTOR_RW_TEXTURE)
		{

			for (uint32_t j = 0; j < final_var_count; ++j)
			{
				DXDescriptorDataReference dx_descriptor_data = {};
				dx_descriptor_data.p_texture = cur_descriptor->m_textures[j];
				m_dxdescriptor_data_reference_list.push_back(dx_descriptor_data);



			}


			back_desc.m_textures = &m_dxdescriptor_data_reference_list[dx_descriptor_ref_original_size].p_texture;

		}

		//m_descriptor_data_list.push_back(*(descriptor_data + i));
	}

	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Bind_Descriptors;
	cmd.m_cmd_bind_descriptors.m_descriptor_count = descriptor_count;
	cmd.m_cmd_bind_descriptors.m_pipeline = pipeline;

	DescriptorData* start_p = &m_descriptor_data_list[0];
	DescriptorData* new_begin_p = start_p + ori_size;

	cmd.m_cmd_bind_descriptors.m_p_descriptor_data = new_begin_p;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_bind_vertex_buffer(Buffer* buffer)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Bind_Vertex_Buffer;
	cmd.m_cmd_bind_vertex_buffer.m_vertex_buffer = buffer;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_bind_index_buffer(Buffer* buffer)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Bind_Indices_Buffer;
	cmd.m_cmd_bind_indices_buffer.m_indices_buffer = buffer;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Set_Viewport;
	cmd.m_cmd_set_viewport.m_x = x;
	cmd.m_cmd_set_viewport.m_y = y;
	cmd.m_cmd_set_viewport.m_width = width;
	cmd.m_cmd_set_viewport.m_height = height;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_draw(uint32_t vertex_count, uint32_t first_vertex)
{
	DXCMD  cmd = {};
	cmd.m_type = DXCMD_Type::Draw;
	cmd.m_cmd_draw.m_vertex_count = vertex_count;
	cmd.m_cmd_draw.m_first_vertex = first_vertex;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_draw_index(uint32_t indices_count, uint32_t first_index, uint32_t first_vertex)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Draw_Index;
	cmd.m_cmd_draw_index.m_first_index = first_index;
	cmd.m_cmd_draw_index.m_index_count = indices_count;
	cmd.m_cmd_draw_index.m_first_vertex = first_vertex;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_draw_index_instanced(
	uint32_t instance_count,
	uint32_t first_instance,
	uint32_t indices_count,
	uint32_t first_index,
	uint32_t first_vertex)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Draw_Index_Instanced;
	cmd.m_cmd_draw_index_instanced.m_first_instance = first_instance;
	cmd.m_cmd_draw_index_instanced.m_instance_count = instance_count;
	cmd.m_cmd_draw_index_instanced.m_indices_count = indices_count;
	cmd.m_cmd_draw_index_instanced.m_first_index = first_index;
	cmd.m_cmd_draw_index_instanced.m_first_vertex = first_vertex;
	m_cmd_list.push_back(cmd);
}


void DXRenderer::cmd_dispatch(uint32_t thread_group_x, uint32_t thread_group_y, uint32_t thread_group_z)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::DXCMD_Type_Dispatch;
	cmd.m_cmd_dispatch.m_thread_group_x = thread_group_x;
	cmd.m_cmd_dispatch.m_thread_group_y = thread_group_y;
	cmd.m_cmd_dispatch.m_thread_group_z = thread_group_z;
	m_cmd_list.push_back(cmd);
}


void DXRenderer::cmd_bind_streamout_render_targets(Buffer* streamoutVertexBuffer, uint32_t offsets)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Bind_StreamoutRenderTargets;
	cmd.m_cmd_bind_streamout_render_targets.m_streamOutVB = streamoutVertexBuffer;
	cmd.m_cmd_bind_streamout_render_targets.m_offset = offsets;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::cmd_draw_auto()
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Draw_Auto;
	m_cmd_list.push_back(cmd);
}


void DXRenderer::cmd_update_buffer(const BufferUpdateDesc& buffer_update_desc)
{
	DXCMD cmd = {};
	cmd.m_type = DXCMD_Type::Update_Buffer;
	cmd.m_cmd_update_buffer.m_update_desc = buffer_update_desc;
	m_cmd_list.push_back(cmd);
}

void DXRenderer::instant_update_buffer(const BufferUpdateDesc& buffer_update_desc)
{
	Buffer* buffer = buffer_update_desc.m_buffer;

	if (buffer->get_desc().m_usageType == Usage_Type::USAGE_DEFAULT)
	{
		m_d3d_device_context->UpdateSubresource(buffer->m_p_buffer, 0, nullptr, buffer_update_desc.m_pSource, 0, 0);
	}
	else
	{
		uint32_t update_size = buffer_update_desc.m_size == 0 ?
			buffer->m_buffer_size : buffer_update_desc.m_size;

		map_buffer(buffer_update_desc.m_buffer);
		memcpy
		(
			(uint8_t*)buffer->m_mapped_address + buffer_update_desc.m_destOffset,
			(uint8_t*)buffer_update_desc.m_pSource
			+ buffer_update_desc.m_sourceOffset, update_size
		);

		unmap_buffer(buffer_update_desc.m_buffer);
	}

}

void DXRenderer::execute_queued_cmd()
{
	for (uint32_t i = 0; i < m_cmd_list.size(); ++i)
	{
		const DXCMD& cmd = m_cmd_list[i];

		switch (cmd.m_type)
		{
		case DXCMD_Type::Bind_Descriptors:
		{
			const DXCMD_Bind_Descriptors& cmd_bind_descriptors = cmd.m_cmd_bind_descriptors;

			for (uint32_t i = 0; i < cmd_bind_descriptors.m_descriptor_count; ++i)
			{
				DescriptorData* p_cur_descriptor_data = cmd_bind_descriptors.m_p_descriptor_data + i;

				DescriptorType cur_descriptor_type = p_cur_descriptor_data->m_descriptor_type;

				uint32_t var_count = std::max(1u, p_cur_descriptor_data->m_var_count);

				Shader_Stages descriptor_shader_stages = (Shader_Stages)p_cur_descriptor_data->m_shader_stages;
				if (cur_descriptor_type == DescriptorType::DESCRIPTOR_BUFFER)
				{
					const BufferDesc& buffer_desc = p_cur_descriptor_data->m_buffers[0]->m_desc;

					if (buffer_desc.m_bindFlags & Bind_Flags::BIND_SHADER_RESOURCE)
					{
						//TODO: figure out the dynamic allocation
						ID3D11ShaderResourceView* srvs[32] = { nullptr };

						for (uint32_t srv_i = 0; srv_i < var_count; ++srv_i)
						{
							Buffer* p_cur_buffer = p_cur_descriptor_data->m_buffers[srv_i];
							srvs[srv_i] = p_cur_buffer->m_srv;
						}
						set_shader_resources(p_cur_descriptor_data->m_binding_location,
							descriptor_shader_stages, var_count, srvs);
					}
					else if (buffer_desc.m_bindFlags & Bind_Flags::BIND_CONSTANT_BUFFER)
					{
						//TODO: probably need multiple constant buffer support

						for (uint32_t index = 0; index < var_count; ++index)
						{
							Buffer* constant_buffer = p_cur_descriptor_data->m_buffers[index];


							//ID3D11Buffer* d3d_constant_buffer = constant_buffer->m_p_buffer;
							//set_constant_buffer(p_cur_descriptor_data->m_binding_location, descriptor_shader_stages, 1, &d3d_constant_buffer);

							if ((p_cur_descriptor_data->p_offsets[index] == 0 && p_cur_descriptor_data->p_sizes[index] == 0) ||
								(constant_buffer->get_desc().m_usageType == Usage_Type::USAGE_DEFAULT))
							{
								ID3D11Buffer* d3d_constant_buffer = constant_buffer->m_p_buffer;
								set_constant_buffer(p_cur_descriptor_data->m_binding_location, descriptor_shader_stages, 1, &d3d_constant_buffer);
							}
							else
							{
								D3D11_MAPPED_SUBRESOURCE read_mapped_data = {};
								m_d3d_device_context->Map(constant_buffer->m_p_buffer, 0, D3D11_MAP_READ, 0, &read_mapped_data);

								D3D11_MAPPED_SUBRESOURCE write_mapped_data = {};
								m_d3d_device_context->Map(m_p_transient_constant_buffer->m_p_buffer, 0,
									D3D11_MAP_WRITE_DISCARD, 0, &write_mapped_data);

								memcpy(write_mapped_data.pData,
									reinterpret_cast<uint8_t*>(read_mapped_data.pData) + p_cur_descriptor_data->p_offsets[index],
									p_cur_descriptor_data->p_sizes[index]);

								m_d3d_device_context->Unmap(m_p_transient_constant_buffer->m_p_buffer, 0);
								m_d3d_device_context->Unmap(constant_buffer->m_p_buffer, 0);

								set_constant_buffer(p_cur_descriptor_data->m_binding_location,
									descriptor_shader_stages, 1, &m_p_transient_constant_buffer->m_p_buffer);
							}
						}
					}
					
				}
				else if (cur_descriptor_type == DescriptorType::DESCRIPTOR_RW_BUFFER)
				{
					const BufferDesc& buffer_desc = p_cur_descriptor_data->m_buffers[0]->m_desc;
					assert((buffer_desc.m_bindFlags & Bind_Flags::BIND_UNORDERED_ACCESS) != 0);

					ID3D11UnorderedAccessView* uavs[32] = { NULL };
					for (uint32_t index = 0; index < var_count; ++index)
					{
						Buffer* uav_buffer = p_cur_descriptor_data->m_buffers[index];
						uavs[index] = uav_buffer->m_uav;
					}

					set_uav_resources(p_cur_descriptor_data->m_binding_location, var_count, uavs);
				}
				else if (cur_descriptor_type == DescriptorType::DESCRIPTOR_SAMPLER)
				{
					//TODO:

					ID3D11SamplerState* sampler_states[32] = { nullptr };
					for (uint32_t sampler_i = 0; sampler_i < var_count; ++sampler_i)
					{
						Sampler* p_cur_sampler = p_cur_descriptor_data->m_samplers[sampler_i];
						sampler_states[sampler_i] = p_cur_sampler->m_sampler_state;
					}

					set_samplers(p_cur_descriptor_data->m_binding_location,
						descriptor_shader_stages, var_count, sampler_states);
				}
				else if (cur_descriptor_type == DescriptorType::DESCRIPTOR_TEXTURE)
				{
					//TODO: do for UAV
					ID3D11ShaderResourceView* srvs[32] = { nullptr };
					for (uint32_t srv_i = 0; srv_i < var_count; ++srv_i)
					{
						Texture* p_cur_texture = (*p_cur_descriptor_data->m_textures) + srv_i;

						srvs[srv_i] = p_cur_texture->m_p_srv;
					}

					set_shader_resources(p_cur_descriptor_data->m_binding_location,
						descriptor_shader_stages, var_count, srvs);
				}
				else if (cur_descriptor_type == DescriptorType::DESCRIPTOR_RW_TEXTURE)
				{
					const TextureDesc& texture_desc = p_cur_descriptor_data->m_textures[0]->m_texture_desc;
					assert((texture_desc.m_bindFlags & Bind_Flags::BIND_UNORDERED_ACCESS) != 0);

					ID3D11UnorderedAccessView* uavs[32] = { NULL };
					for (uint32_t index = 0; index < var_count; ++index)
					{
						Texture* uav_texture = p_cur_descriptor_data->m_textures[index];
						uavs[index] = uav_texture->m_pp_uav[p_cur_descriptor_data->m_uav_mip_slice];
					}
					set_uav_resources(p_cur_descriptor_data->m_binding_location, var_count, uavs);
				}
			}
			break;
		}
		case DXCMD_Type::Bind_Indices_Buffer:
		{
			const DXCMD_Bind_Indices_Buffer& cmd_bind_indices_buffer = cmd.m_cmd_bind_indices_buffer;
			m_d3d_device_context->IASetIndexBuffer(cmd_bind_indices_buffer.m_indices_buffer->m_p_buffer,
				cmd_bind_indices_buffer.m_indices_buffer->m_dxgi_index_format, 0);
			break;
		}
		case DXCMD_Type::Bind_Pipeline:
		{
			const DXCMD_Bind_Pipeline& cmd_bind_pipeline = cmd.m_cmd_bind_pipeline;

			Pipeline* pipeline = cmd_bind_pipeline.m_pipeline;
			if (pipeline)
			{
				reset_shader_resources();
				reset_shader_uavs();
				PipelineType pipeline_type = cmd_bind_pipeline.m_pipeline->m_desc.m_pipeline_type;
				if (pipeline_type == PipelineType::GRAPHICS)
				{
					m_d3d_device_context->RSSetState(pipeline->m_desc.m_graphics_desc.
						m_rasterizer_state->m_d3d_rasterizer_state);
					m_d3d_device_context->OMSetDepthStencilState(
						pipeline->m_desc.m_graphics_desc.m_depth_state->m_p_depth_stencil_state, 0);
					m_d3d_device_context->IASetPrimitiveTopology(pipeline->m_d3d_primitive_topo_type);
					m_d3d_device_context->IASetInputLayout(pipeline->m_input_layout);

					m_d3d_device_context->OMSetBlendState(pipeline->m_desc.m_graphics_desc.
						m_blend_state->m_d3d_blend_state, clear_color_black, (~0));

					ID3D11VertexShader* vertex_shader = pipeline->m_desc.m_graphics_desc.m_shader->m_vertex_shader;
					ID3D11PixelShader* pixel_shader = pipeline->m_desc.m_graphics_desc.m_shader->m_pixel_shader;
					ID3D11GeometryShader* geometryShader = pipeline->m_desc.m_graphics_desc.m_shader->m_geometry_shader;

					m_d3d_device_context->VSSetShader(vertex_shader, nullptr, 0);
					m_d3d_device_context->GSSetShader(geometryShader, nullptr, 0);
					m_d3d_device_context->PSSetShader(pixel_shader, nullptr, 0);
				}
				else if (pipeline_type == PipelineType::COMPUTE)
				{
					m_d3d_device_context->OMSetBlendState(nullptr, clear_color_black, (~0));
					m_d3d_device_context->RSSetState(nullptr);
					m_d3d_device_context->OMSetDepthStencilState(nullptr, 0);

					ID3D11ComputeShader* cs_shader = pipeline->m_desc.m_compute_desc.m_shader->m_compute_shader;
					m_d3d_device_context->CSSetShader(cs_shader, nullptr, 0);
				}
			}
			else
			{
				m_d3d_device_context->VSSetShader(nullptr, nullptr, 0);
				m_d3d_device_context->PSSetShader(nullptr, nullptr, 0);
				m_d3d_device_context->RSSetState(nullptr);
				m_d3d_device_context->OMSetDepthStencilState(nullptr, 0);

				m_d3d_device_context->IASetInputLayout(nullptr);
			}


			break;
		}
		case DXCMD_Type::Bind_RenderTargets:
		{
			const DXCMD_Bind_RenderTargets& cmd_bind_rendertargets = cmd.m_cmd_bind_rendertargets;

			reset_shader_resources();
			static ID3D11RenderTargetView* null_rtvs[MAX_RENDER_TARGET_ATTACHMENTS] = { NULL };
			m_d3d_device_context->OMSetRenderTargets(MAX_RENDER_TARGET_ATTACHMENTS, null_rtvs, nullptr);

			ID3D11RenderTargetView* rtvs[MAX_RENDER_TARGET_ATTACHMENTS] = { NULL };

			for (uint32_t i = 0; i < cmd_bind_rendertargets.m_color_rts_count; ++i)
			{
				rtvs[i] = cmd_bind_rendertargets.m_color_rts[i]->m_pp_rendertargetview[cmd_bind_rendertargets.m_color_mips_levels[i]];
			}

			ID3D11DepthStencilView* dsv = nullptr;

			if (cmd_bind_rendertargets.m_depth_stencil_rt)
			{
				dsv = cmd_bind_rendertargets.m_depth_stencil_rt->m_pp_depth_stencil_view[cmd_bind_rendertargets.m_depth_mips_level];
			}

			m_d3d_device_context->OMSetRenderTargets(cmd_bind_rendertargets.m_color_rts_count, rtvs, dsv);

			const LoadActionsDesc& load_actions_desc = cmd_bind_rendertargets.m_load_actions_desc;

			for (uint32_t i = 0; i < cmd_bind_rendertargets.m_color_rts_count; ++i)
			{
				if (load_actions_desc.m_load_actions_color[i] == LoadActionType::CLEAR)
				{
					FLOAT d3d_clear_values[4] =
					{
						load_actions_desc.m_clear_color_values[i].r,
						load_actions_desc.m_clear_color_values[i].g,
						load_actions_desc.m_clear_color_values[i].b,
						load_actions_desc.m_clear_color_values[i].a
					};
					m_d3d_device_context->ClearRenderTargetView(rtvs[i], d3d_clear_values);
				}
			}

			if (cmd_bind_rendertargets.m_depth_stencil_rt && (
				load_actions_desc.m_load_action_depth == LoadActionType::CLEAR ||
				load_actions_desc.m_load_action_stencil == LoadActionType::CLEAR))
			{
				uint32_t final_depth_stencil_clear_flags = 0;
				if (load_actions_desc.m_load_action_depth == LoadActionType::CLEAR)
				{
					final_depth_stencil_clear_flags |= D3D11_CLEAR_DEPTH;
				}
				if (load_actions_desc.m_load_action_stencil == LoadActionType::CLEAR)
				{
					final_depth_stencil_clear_flags |= D3D11_CLEAR_STENCIL;
				}
				m_d3d_device_context->ClearDepthStencilView(dsv,
					final_depth_stencil_clear_flags, load_actions_desc.m_clear_depth_stencil.m_depth, 0);
			}
			break;
		}

		case DXCMD_Type::Update_Buffer:
		{
			//TODO: only temp way
			const DXCMD_Update_Buffer& cmd_update_buffer = cmd.m_cmd_update_buffer;
			const BufferUpdateDesc& buffer_update_desc = cmd_update_buffer.m_update_desc;

			instant_update_buffer(buffer_update_desc);
			break;
		}

		case DXCMD_Type::Bind_Vertex_Buffer:
		{
			const DXCMD_Bind_Vertex_Buffer& cmd_bind_vertex_buffer = cmd.m_cmd_bind_vertex_buffer;
			Buffer* p_vertex_buffer = cmd_bind_vertex_buffer.m_vertex_buffer;
			uint32_t offset = 0;
			m_d3d_device_context->IASetVertexBuffers(0, 1, &p_vertex_buffer->m_p_buffer,
				&p_vertex_buffer->m_desc.m_vertexStride, &offset);
			break;
		}
		case DXCMD_Type::Bind_StreamoutRenderTargets:
		{
			const DXCMD_Bind_StreamoutRenderTargets& cmd_bind_streamout_rts = cmd.m_cmd_bind_streamout_render_targets;
			uint32_t offset = cmd_bind_streamout_rts.m_offset;

			ID3D11Buffer* d3dBuffer = nullptr;
			if (cmd_bind_streamout_rts.m_streamOutVB)
			{
				d3dBuffer = cmd_bind_streamout_rts.m_streamOutVB->m_p_buffer;
			}
			m_d3d_device_context->SOSetTargets(1, &d3dBuffer, &offset);
			break;
		}

		case DXCMD_Type::Draw_Auto:
		{
			m_d3d_device_context->DrawAuto();
			break;
		}

		case DXCMD_Type::Set_Viewport:
		{
			const DXCMD_Set_Viewport& cmd_set_viewport = cmd.m_cmd_set_viewport;
			D3D11_VIEWPORT viewport;
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
			viewport.MinDepth = 0.f;
			viewport.MaxDepth = 1.f;
			viewport.TopLeftX = static_cast<FLOAT>(cmd_set_viewport.m_x);
			viewport.TopLeftY = static_cast<FLOAT>(cmd_set_viewport.m_y);
			viewport.Width = static_cast<FLOAT>(cmd_set_viewport.m_width);
			viewport.Height = static_cast<FLOAT>(cmd_set_viewport.m_height);
			m_d3d_device_context->RSSetViewports(1, &viewport);
			break;
		}
		case DXCMD_Type::Draw:
		{
			const DXCMD_Draw& cmd_draw = cmd.m_cmd_draw;
			m_d3d_device_context->Draw(cmd_draw.m_vertex_count, cmd_draw.m_first_vertex);
			break;
		}
		case DXCMD_Type::Draw_Index:
		{
			const DXCMD_Draw_Index& cmd_draw_index = cmd.m_cmd_draw_index;
			m_d3d_device_context->DrawIndexed(cmd_draw_index.m_index_count,
				cmd_draw_index.m_first_index, cmd_draw_index.m_first_vertex);
			break;
		}
		case DXCMD_Type::Draw_Index_Instanced:
		{
			const DXCMD_Draw_Index_Instanced& cmd_draw_index_instanced = cmd.m_cmd_draw_index_instanced;
			m_d3d_device_context->DrawIndexedInstanced(
				cmd_draw_index_instanced.m_indices_count,
				cmd_draw_index_instanced.m_instance_count,
				cmd_draw_index_instanced.m_first_index,
				cmd_draw_index_instanced.m_first_vertex,
				cmd_draw_index_instanced.m_first_instance);
			break;
		}
		case DXCMD_Type::DXCMD_Type_Dispatch:
		{
			const DXCMD_Dispatch& dispatch = cmd.m_cmd_dispatch;
			m_d3d_device_context->Dispatch(dispatch.m_thread_group_x, dispatch.m_thread_group_y, dispatch.m_thread_group_z);
			break;
		}
		}
	}
	m_descriptor_data_list.clear();
	m_dxdescriptor_data_reference_list.clear();
	m_cmd_list.clear();
}



void DXRenderer::reset_shader_resources()
{
	static ID3D11ShaderResourceView* empty_srvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
	m_d3d_device_context->VSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, empty_srvs);
	m_d3d_device_context->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, empty_srvs);
	m_d3d_device_context->CSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, empty_srvs);
	m_d3d_device_context->GSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, empty_srvs);
}

void DXRenderer::reset_shader_uavs()
{
	static ID3D11UnorderedAccessView* empty_uavs[D3D11_PS_CS_UAV_REGISTER_COUNT] = { nullptr };
	m_d3d_device_context->CSSetUnorderedAccessViews(0, D3D11_PS_CS_UAV_REGISTER_COUNT, empty_uavs, nullptr);
}

void DXRenderer::set_shader_resources(uint32_t binding_loc, Shader_Stages shader_stages,
	uint32_t num_resources, ID3D11ShaderResourceView** pp_resource_views)
{
	if (shader_stages & Shader_Stages::VERTEX_STAGE)
	{
		m_d3d_device_context->VSSetShaderResources(binding_loc,
			num_resources, pp_resource_views);
	}

	if (shader_stages & Shader_Stages::GEOMETRY_STAGE)
	{
		m_d3d_device_context->GSSetShaderResources(binding_loc, 
			num_resources, pp_resource_views);
	}

	if (shader_stages & Shader_Stages::PIXEL_STAGE)
	{
		m_d3d_device_context->PSSetShaderResources(binding_loc,
			num_resources, pp_resource_views);
	}

	if (shader_stages & Shader_Stages::COMPUTE_STAGE)
	{
		m_d3d_device_context->CSSetShaderResources(binding_loc,
			num_resources, pp_resource_views);
	}
}

void DXRenderer::set_uav_resources(uint32_t binding_loc, uint32_t num_resources,
	ID3D11UnorderedAccessView** pp_uav_views)
{
	m_d3d_device_context->CSSetUnorderedAccessViews(binding_loc,
		num_resources, pp_uav_views, nullptr);
}


void  DXRenderer::set_constant_buffer(uint32_t binding_loc, Shader_Stages shader_stages,
	uint32_t num_resources, ID3D11Buffer** pp_buffers)
{
	if (shader_stages & Shader_Stages::VERTEX_STAGE)
	{
		m_d3d_device_context->VSSetConstantBuffers(binding_loc,
			num_resources, pp_buffers);
	}

	if (shader_stages & Shader_Stages::GEOMETRY_STAGE)
	{
		m_d3d_device_context->GSSetConstantBuffers(binding_loc,
			num_resources, pp_buffers);
	}

	if (shader_stages & Shader_Stages::PIXEL_STAGE)
	{
		m_d3d_device_context->PSSetConstantBuffers(binding_loc,
			num_resources, pp_buffers);
	}

	if (shader_stages & Shader_Stages::COMPUTE_STAGE)
	{
		m_d3d_device_context->CSSetConstantBuffers(binding_loc,
			num_resources, pp_buffers);
	}
}


void DXRenderer::set_samplers(uint32_t binding_loc, Shader_Stages shader_stages,
	uint32_t num_resources, ID3D11SamplerState** sampler_states)
{
	if (shader_stages & Shader_Stages::VERTEX_STAGE)
	{
		m_d3d_device_context->VSSetSamplers(binding_loc,
			num_resources, sampler_states);
	}

	if (shader_stages & Shader_Stages::GEOMETRY_STAGE)
	{
		m_d3d_device_context->GSSetSamplers(binding_loc,
			num_resources, sampler_states);
	}

	if (shader_stages & Shader_Stages::PIXEL_STAGE)
	{
		m_d3d_device_context->PSSetSamplers(binding_loc,
			num_resources, sampler_states);
	}

	if (shader_stages & Shader_Stages::COMPUTE_STAGE)
	{
		m_d3d_device_context->CSSetSamplers(binding_loc,
			num_resources, sampler_states);
	}
}

void DXRenderer::map_buffer(Buffer* buffer)
{
	CPU_Access_Type cpu_access_type = buffer->m_desc.m_cpuAccessType;
	D3D11_MAP map_type = static_cast<D3D11_MAP>(0);
	switch (cpu_access_type)
	{
	case CPU_Access_Type::ACCESS_READ: map_type = D3D11_MAP::D3D11_MAP_READ; break;
	case CPU_Access_Type::ACCESS_WRITE: map_type = D3D11_MAP::D3D11_MAP_WRITE_NO_OVERWRITE; break;
	case CPU_Access_Type::ACCESS_WRITE_READ: map_type = D3D11_MAP::D3D11_MAP_READ_WRITE; break;
	default: break;
	}

	D3D11_MAPPED_SUBRESOURCE mapped_resource_data;
	ZeroMemory(&mapped_resource_data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_d3d_device_context->Map(buffer->m_p_buffer, 0, map_type, 0, &mapped_resource_data);
	buffer->m_mapped_address = mapped_resource_data.pData;
}

void DXRenderer::unmap_buffer(Buffer* buffer)
{
	if (!buffer->m_mapped_address)
	{
		return;
	}

	m_d3d_device_context->Unmap(buffer->m_p_buffer, 0);
	buffer->m_mapped_address = nullptr;
}


bool DXRenderer::init_d3d11(uint32_t swap_chain_sample_count)
{
	HRESULT dxgi_hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&m_dxgi_factory));

	if (FAILED_HR(dxgi_hr))
	{
		assert(false == true);
		return false;
	}



	//TODO: currently very dirty way to select GPU
	IDXGIAdapter1* adapter = NULL;
	DXGI_ADAPTER_DESC1 desc1 = {};
	DXGI_ADAPTER_DESC1 desc2 = {};



	for (uint32_t i = 0; DXGI_ERROR_NOT_FOUND != m_dxgi_factory->EnumAdapters1(i, (IDXGIAdapter1**)&adapter); ++i)
	{
		DXGI_ADAPTER_DESC1 desc = {};
		adapter->GetDesc1(&desc);

		char raw_char[64];
		wcstombs(raw_char, desc.Description, MAX_PATH);

		std::string gpu_name(raw_char);

		if (i == 0)
		{
			adapter->GetDesc1(&desc1);
			m_cur_adapter = adapter;
			m_cur_gpu_name = gpu_name;
		}
		else if (i == 1)
		{
			adapter->GetDesc1(&desc2);
		}


		if (gpu_name.find("NVIDIA") != std::string::npos)
		{
			m_cur_gpu_name = gpu_name;
			m_cur_adapter = adapter;
			break;
		}
	}


	RECT client_rect;
	GetClientRect(m_window_handle, &client_rect);

	unsigned int client_width = client_rect.right - client_rect.left;
	unsigned int client_height = client_rect.bottom - client_rect.top;




	IDXGIOutput* p_adapter_output;
	unsigned num_modes;

	HRESULT result;
	// Enumerate the primary adapter output (monitor).
	result = m_cur_adapter->EnumOutputs(0, &p_adapter_output);
	if (FAILED(result))
	{
		return false;
	}
	/// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor).
	result = p_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* display_mode_list;
	display_mode_list = (DXGI_MODE_DESC*)alloca(num_modes * sizeof(DXGI_MODE_DESC));
	if (!display_mode_list)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = p_adapter_output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &num_modes, display_mode_list);
	if (FAILED(result))
	{
		return false;
	}


	p_adapter_output->Release();

	unsigned numerator = 0;
	unsigned denominator = 0;
	for (unsigned i = 0; i < num_modes; i++)
	{
		if (display_mode_list[i].Width == (unsigned int)client_width)
		{
			if (display_mode_list[i].Height == (unsigned int)client_height)
			{
				numerator = display_mode_list[i].RefreshRate.Numerator;
				denominator = display_mode_list[i].RefreshRate.Denominator;
			}
		}
	}



	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swap_chain_desc.BufferCount = 3;
	swap_chain_desc.BufferDesc.Width = client_width;
	swap_chain_desc.BufferDesc.Height = client_height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_enable_vysnc)
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = numerator;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.OutputWindow = m_window_handle;
	swap_chain_desc.SampleDesc.Count = swap_chain_sample_count;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_desc.Windowed = TRUE;

	UINT create_device_flags = 0;
#if _DEBUG
	create_device_flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	D3D_FEATURE_LEVEL feature_level;

	


	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		m_cur_adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		create_device_flags,
		feature_levels,
		_countof(feature_levels),
		D3D11_SDK_VERSION, &swap_chain_desc,
		&m_swap_chain.m_d3d_swap_chain,
		&m_d3d_device, &feature_level,
		&m_d3d_device_context);

	if (hr == E_INVALIDARG)
	{
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			create_device_flags,
			&feature_levels[1],
			_countof(feature_levels) - 1,
			D3D11_SDK_VERSION, &swap_chain_desc,
			&m_swap_chain.m_d3d_swap_chain,
			&m_d3d_device, &feature_level,
			&m_d3d_device_context);
	}

	if (FAILED_HR(hr))
	{
		return false;
	}

	ID3D11Texture2D* back_buffer;
	hr = m_swap_chain.m_d3d_swap_chain->GetBuffer(0,
		__uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer);

	if (FAILED_HR(hr))
	{
		return false;
	}
	//TODO: formalize the swap chain creation
	m_swap_chain.m_p_swap_chain_render_target = new RenderTarget(RenderTargetDesc());
	m_swap_chain.m_p_swap_chain_render_target->m_desc.m_texture_desc.m_width = client_width;
	m_swap_chain.m_p_swap_chain_render_target->m_desc.m_texture_desc.m_height = client_height;
	m_swap_chain.m_p_swap_chain_render_target->m_desc.m_texture_desc.m_clearVal = ClearValue{ 0.2f, 0.2f, 0.2f, 1.f };
	m_swap_chain.m_p_swap_chain_render_target->m_desc.m_texture_desc.m_mipLevels = 1;
	m_swap_chain.m_p_swap_chain_render_target->m_desc.m_texture_desc.m_depth = 1;

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};

	if (swap_chain_sample_count > 1)
	{
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	}
	else
	{
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;
	}

	m_swap_chain.m_p_swap_chain_render_target->m_pp_rendertargetview =
		(ID3D11RenderTargetView**)malloc(sizeof(ID3D11RenderTargetView*) * 1);

	hr = m_d3d_device->CreateRenderTargetView(back_buffer,
		&rtv_desc, &m_swap_chain.m_p_swap_chain_render_target->m_pp_rendertargetview[0]);

	if (FAILED_HR(hr))
	{
		return false;
	}

	SafeRelease(back_buffer);
	return true;
}


void DXRenderer::present_swap_chain()
{
	if (m_enable_vysnc)
	{
		m_swap_chain.m_d3d_swap_chain->Present(1, 0);
		return;
	}
	m_swap_chain.m_d3d_swap_chain->Present(0, 0);
}