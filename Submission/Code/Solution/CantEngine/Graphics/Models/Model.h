#pragma once

#include "Mesh.h"
#include "Physics/Geometry/Aabb.h"
#include "Helper/Hash.h"

/**
 * @file Model.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief Defines 3D Model data
 */

/**
 * @brief VertexData defines the data required to render primitive 3D meshes
 * 
 */
struct VertexData
{
	VertexData(const Vector3& pos, const Vector3& normal, const Vector3& tangent, const Vector3& bitangent, const Vector2& uv)
		:m_position(pos), m_normal(normal), m_tangent(tangent), m_bitangent(bitangent), m_tex_coords(uv) {}

	VertexData(const Vector3& pos, const Vector3& normal, const Vector2& uv)
		:m_position(pos), m_normal(normal), m_tangent(0.f, 0.f, 0.f), m_bitangent(0.f, 0.f, 0.f), m_tex_coords(uv) {}

	Vector3 m_position;
	Vector3 m_normal;
	Vector3 m_tangent;
	Vector3 m_bitangent;
	Vector2 m_tex_coords;
};



class LoadedMesh;
class Texture;
class DXRenderer;


/**
 * @brief Contains all the interface for getting Model's graphic data
 * 
 */
class Model 
{
public:
	friend class ModelLoader;
	friend class ResourceManager;
	typedef std::vector<Mesh> MeshesList;
	typedef std::vector<VertexData> VerticesList;
public:
	struct Triangle
	{
		uint32_t index[3];
		uint32_t operator[](int i) const { return index[i]; }
		uint32_t & operator[](int i) { return index[i]; }
	};
public:
	Model();
	virtual ~Model();
	/**
	 * @brief Get the Model's Vertex Buffer object
	 * 
	 * @return Buffer* 
	 */
	Buffer* GetVertexBuffer();

	/**
	 * @brief Get the Model's Index Buffer object
	 * 
	 * @return Buffer* 
	 */
	Buffer* GetIndexBuffer();
	/**
	 * @brief Get the Model's Index Total Count
	 * 
	 * @return uint32_t 
	 */
	uint32_t GetIndexTotalCount() const;

	/**
	 * @brief Release all graphics resources related to Model
	 * 
	 */
	void Release();

	/**
	 * @brief Get the Meshes List of the Model
	 * 
	 * @return const MeshesList& 
	 */
	const MeshesList& GetMeshesList() const;
	/**
	 * @brief Get the Vertices List of the Model
	 * 
	 * @return const VerticesList& 
	 */
	const VerticesList& GetVerticesList() const;
	/**
	 * @brief Get the Triangle List of the Model
	 * 
	 * @return const std::vector<Triangle>& 
	 */
	const std::vector<Triangle>& GetTriangleList() const;
	/**
	 * @brief Get the bounding boxes of the Model in object space
	 * 
	 * @return const Aabb& 
	 */
	const Aabb& GetAABB() const;
protected:
	std::vector<Triangle> m_triangle_indices;
	VerticesList m_vertices;

	Buffer* m_vertex_buffer;
	Buffer* m_index_buffer;

	bool m_hasBones;

private:
	virtual void InitBuffer(DXRenderer* dxrenderer);
private:
	Aabb m_aabb;
	StringId m_modelId;
	bool m_has_tangent;
	std::vector<Mesh> m_meshes;
	std::vector<const Texture*> m_textures;
};