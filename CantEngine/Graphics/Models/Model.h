#pragma once

#include "Mesh.h"
#include "Physics/Geometry/Aabb.h"
#include "Helper/Hash.h"



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

class Model 
{
public:
	friend class ModelLoader;
	friend class ResourceManager;
	//TODO: temp way
	friend class Factory;
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

	Buffer* get_vertex_buffer();
	Buffer* get_index_buffer();
	
	uint32_t get_index_total_count() const;

	void Release();

	const MeshesList& GetMeshesList() const;
	const VerticesList& GetVerticesList() const;
	const std::vector<Triangle>& GetTriangleList() const;
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