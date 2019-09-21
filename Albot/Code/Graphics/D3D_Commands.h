#pragma once

#include "DescriptorData.h"
#include "Buffer.h"
#include "RenderTarget.h"

class Pipeline;


struct DescriptorData;

enum DXCMD_Type
{
	Bind_Indices_Buffer,
	Bind_Vertex_Buffer,
	Bind_RenderTargets,
	Bind_Pipeline,
	Update_Buffer,
	Bind_Descriptors,
	Set_Viewport,
	Draw,
	Draw_Index,
	Draw_Index_Instanced,
	COUNT
};

struct DXCMD_Bind_Indices_Buffer
{
	Buffer* m_indices_buffer;
};

struct DXCMD_Bind_Vertex_Buffer
{
	Buffer* m_vertex_buffer;
};


struct DXCMD_Bind_Pipeline
{
	Pipeline* m_pipeline;
};

struct DXCMD_Update_Buffer
{
	BufferUpdateDesc m_update_desc;
};

struct DXCMD_Bind_Descriptors
{
	DescriptorData* m_p_descriptor_data;
	uint32_t m_descriptor_count;
	Pipeline* m_pipeline;
};

struct DXCMD_Bind_RenderTargets
{
	RenderTarget** m_color_rts;
	uint32_t m_color_rts_count;
	RenderTarget* m_depth_stencil_rt;
	LoadActionsDesc m_load_actions_desc;
};


struct DXCMD_Set_Viewport
{
	int32_t m_x;
	int32_t m_y;
	int32_t m_width;
	int32_t m_height;
};

struct DXCMD_Draw
{
	uint32_t m_first_vertex;
	uint32_t m_vertex_count;
};

struct DXCMD_Draw_Index
{
	uint32_t m_index_count;
	uint32_t m_first_index;
	uint32_t m_first_vertex;
};

struct DXCMD_Draw_Index_Instanced
{
	uint32_t m_instance_count; 
	uint32_t m_first_instance; 
	uint32_t m_indices_count;		
	uint32_t m_first_index; 
	uint32_t m_first_vertex;
};

struct DXCMD
{
	DXCMD_Type m_type;
	union
	{
		DXCMD_Bind_Descriptors m_cmd_bind_descriptors;
		DXCMD_Bind_Indices_Buffer m_cmd_bind_indices_buffer;
		DXCMD_Bind_Pipeline m_cmd_bind_pipeline;
		DXCMD_Bind_Vertex_Buffer m_cmd_bind_vertex_buffer;
		DXCMD_Draw m_cmd_draw;
		DXCMD_Draw_Index m_cmd_draw_index;
		DXCMD_Draw_Index_Instanced m_cmd_draw_index_instanced;
		DXCMD_Set_Viewport m_cmd_set_viewport;
		DXCMD_Update_Buffer m_cmd_update_buffer;
		DXCMD_Bind_RenderTargets m_cmd_bind_rendertargets;
	};
};
