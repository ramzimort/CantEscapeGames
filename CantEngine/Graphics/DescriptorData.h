#pragma once
#include <array>

#define MAX_DESCRIPTOR_VARIABLE_COUNT 128

class Buffer;
class Texture;
class Sampler;
class Pipeline;

enum DescriptorType
{
	DESCRIPTOR_SAMPLER = 0,
	DESCRIPTOR_BUFFER,
	DESCRIPTOR_TEXTURE,

	DESCRIPTOR_RW_BUFFER, 
	DESCRIPTOR_RW_TEXTURE,
	MAX_DESCRIPTORTYPE_COUNT
};

struct DescriptorData
{
	uint32_t m_var_count;
	uint32_t m_binding_location;
	uint32_t m_shader_stages;
	DescriptorType m_descriptor_type;

	//this two variables are only for buffers
	std::array<uint32_t, MAX_DESCRIPTOR_VARIABLE_COUNT> p_offsets;
	std::array<uint32_t, MAX_DESCRIPTOR_VARIABLE_COUNT> p_sizes;

	uint32_t m_uav_mip_slice;
	//uint32_t p_offsets[MAX_DESCRIPTOR_VARIABLE_COUNT];
	//uint32_t p_sizes[MAX_DESCRIPTOR_VARIABLE_COUNT];
	//only for buffer end

	union
	{
		Buffer** m_buffers;
		Texture** m_textures;
		Sampler** m_samplers;
	};
};