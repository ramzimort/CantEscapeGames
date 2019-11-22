#pragma once
#include <vector>
#include "Graphics/D3D11_Renderer.h"
#include "Graphics/Renderer_Includes.h"
#include "Shaders/Shading.h"


/**
 * @file DebugRendering.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief The public high level interface debug rendering available for users to use
 */

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

/**
 * @brief The high level debug rendering interfaces
 * 
 */
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
	/**
	 * @brief Release all graphics resources related to debug rendering
	 * 
	 */
	void Release();

	/**
	 * @brief Send render info request for debug line drawing
	 * 
	 * @param start_pos 
	 * @param end_pos 
	 * @param color 
	 */
	void RegisterDebugLineInstance(const Vector3& start_pos, const Vector3& end_pos, const Vector3& color);
	/**
	 * @brief Send render info request for debug line drawing
	 * 
	 * @param line_instance 
	 */
	void RegisterDebugLineInstance(const DebugLineInstance& line_instance);

	/**
	 * @brief Send render info request for wireframe debug AABB drawing
	 * 
	 * @param debug_aabb_instance 
	 */
	void RegisterDebugAABB(const DebugAABBInstance& debug_aabb_instance);

	/**
	 * @brief Send render info request for wireframe debug sphere drawing
	 * 
	 * @param debug_sphere_instance 
	 */
	void RegisterDebugSphere(const DebugSphereInstance& debug_sphere_instance);
	
	/**
	 * @brief Update debug rendering logic
	 * 
	 * @param dt 
	 */
	void Update(float dt);

	/**
	 * @brief Load all high level resources related to debug rendering
	 * 
	 * @param dxrenderer 
	 */
	void LoadContent(DXRenderer* dxrenderer);

	/**
	 * @brief Clear all the debug render info request
	 * 
	 */
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

