#pragma once
#include <vector>
#include "D3D11_Renderer.h"
#include "Renderer_Includes.h"
#include "../Shaders/Shading.h"

class AppRenderer;
class ResourceManager;

struct DebugAABBInstance
{
	Vector3 m_min_bound;
	Vector3 m_max_bound;
	Vector3 m_color;
};

struct DebugSphereInstance
{
	Vector3 m_center;
	float m_radius;
	Vector3 m_color;
};

struct DebugLineInstance
{
	Vector3 m_startpos;
	Vector3 m_endpos;
	Vector3 m_color;
};


class DebugRendering
{
public:
	typedef std::vector<DebugAABBInstance> DebugAABBInstanceList;
	typedef std::vector<DebugSphereInstance> DebugSphereInstanceList;

	typedef std::vector<DebugLineInstance> DebugLineInstanceList;

public:
	DebugRendering(AppRenderer* app_renderer, ResourceManager* resourceManager);
	~DebugRendering();

	void render_debug_scene();
	void Release();

	void register_debug_line_instance(const Vector3& start_pos, const Vector3& end_pos, const Vector3& color);
	void register_debug_line_instance(const DebugLineInstance& line_instance);
	void register_debug_aabb(const DebugAABBInstance& debug_aabb_instance);
	void register_debug_sphere(const DebugSphereInstance& debug_sphere_instance);

	void LoadContent(DXRenderer* dxrenderer);
private:
	void render_debug_aabb_scene();
	void render_debug_line_scene();
	
private:
	AppRenderer* m_app_renderer;
	DXRenderer* m_dxrenderer;
	ResourceManager* m_resourceManager;

	DebugAABBInstanceList m_debug_aabb_instance_list;
	Pipeline* m_debug_draw_pipeline;
	Shader* m_debug_draw_shader;
	Buffer* m_debug_aabb_instance_uniform_buffer;
	DebugInstanceUniformData m_debug_aabb_instance_uniform_data[MAX_WIREFRAME_DEBUG_DRAW];


	DebugLineInstanceList m_debug_line_instance_list;
	Buffer* m_debug_line_vertex_buffer;
	LineVertexUniformData m_line_vertex_uniform_data[MAX_LINE_DEBUG_DRAW];
	Buffer* m_line_uniform_structured_buffer;
	Pipeline* m_line_draw_pipeline;
	Shader* m_line_draw_shader;

	std::vector<Vector3> m_raw_line_vertices;

	//TODO:
	DebugSphereInstanceList m_debug_sphere_list;
};

