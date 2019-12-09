#include "Mesh.h"
#include "Graphics/DXResourceLoader.h"


Mesh::Mesh()
	:m_material(nullptr)
{

}


Mesh::~Mesh()
{
}

Material* Mesh::get_material() const
{
	return m_material;
}


uint32_t Mesh::get_index_count() const
{
	return m_index_count;
}

uint32_t Mesh::get_start_index() const
{
	return m_start_index;
}

uint32_t Mesh::get_start_vertex() const
{
	return m_start_vertex;
}