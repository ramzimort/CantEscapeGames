#pragma once
#include <vector>
#include "Graphics/Buffer.h"


class Texture;
class Material;


class Mesh
{
public:
	friend class Model;
	friend class ModelLoader;
		
public:
	
public:
	Mesh();
	~Mesh();

	uint32_t get_index_count() const;
	uint32_t get_start_index() const;
	uint32_t get_start_vertex() const;
	Material* get_material() const;
private:
	
private:
	//TODO: add mesh aabb

	uint32_t m_index_count;
	uint32_t m_start_index;
	uint32_t m_start_vertex;

	Material* m_material;
};

