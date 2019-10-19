#pragma once
#include <vector>
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"

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

	friend class DebugRenderingInstance;

	typedef std::vector<DebugAABBInstance> DebugAABBInstanceList;
	typedef std::vector<DebugSphereInstance> DebugSphereInstanceList;

	typedef std::vector<DebugLineInstance> DebugLineInstanceList;

public:
	DebugRendering(AppRenderer* app_renderer, ResourceManager* resourceManager);
	~DebugRendering();

	void UpdateDebugUniformBuffer();
	void RenderDebugScene();
	void Release();

	void RegisterDebugLineInstance(const Vector3& start_pos, const Vector3& end_pos, const Vector3& color);
	void RegisterDebugLineInstance(const DebugLineInstance& line_instance);
	void RegisterDebugAABB(const DebugAABBInstance& debug_aabb_instance);
	void RegisterDebugSphere(const DebugSphereInstance& debug_sphere_instance);
	
	void Update(float dt);
	void LoadContent(DXRenderer* dxrenderer);
	void ClearInstances();
private:
	void RenderDebugAABBScene();
	void RenderDebugLineScene();
	
	
private:
	AppRenderer* m_appRenderer;
	DXRenderer* m_dxrenderer;
	ResourceManager* m_resourceManager;

	DebugAABBInstanceList m_debug_aabb_instance_list;
	Pipeline* m_debug_draw_pipeline;
	Shader* m_debug_draw_shader;

	DebugLineInstanceList m_debug_line_instance_list;
	Buffer* m_debug_line_vertex_buffer;
	LineVertexUniformData m_line_vertex_uniform_data[MAX_LINE_DEBUG_DRAW];
	Buffer* m_line_uniform_structured_buffer;
	Pipeline* m_line_draw_pipeline;
	Shader* m_line_draw_shader;

	std::vector<Vector3> m_raw_line_vertices;

	DebugSphereInstanceList m_debug_sphere_list;
};

