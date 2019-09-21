#pragma once
#define MAX_VERTEX_LAYOUT_ATTRIB 10

enum Attrib_Semantic
{
	POSITION,
	NORMAL,
	COLOR,
	TANGENT,
	BITANGENT,
	TEXCOORD_0,
	TEXCOORD_1,
	MAX_COUNTS
};


struct VertexAttrib
{
	Attrib_Semantic m_semantic;
	uint32_t m_binding;
	DXGI_FORMAT m_format;
	uint32_t m_elementCount;
};

struct VertexLayout
{	
	uint32_t m_atrrib_count;
	VertexAttrib m_attribs[MAX_VERTEX_LAYOUT_ATTRIB];
};

