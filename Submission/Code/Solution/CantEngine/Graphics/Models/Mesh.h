#pragma once
#include <vector>
#include "Graphics/Buffer.h"

/**
 * @file Mesh.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief Defines 3D mesh data
 */

class Material;

/**
 * @brief Contains all the interface for getting Mesh's graphic data
 * 
 */
class Mesh
{
public:
	friend class Model;
	friend class ModelLoader;
public:
	Mesh();
	~Mesh();

	/**
	 * @brief Get the index count number
	 * 
	 * @return uint32_t 
	 */
	uint32_t get_index_count() const;
	/**
	 * @brief Get the start index number
	 * 
	 * @return uint32_t 
	 */
	uint32_t get_start_index() const;
	/**
	 * @brief Get the start vertex number
	 * 
	 * @return uint32_t 
	 */
	uint32_t get_start_vertex() const;
	/**
	 * @brief Get the material object
	 * 
	 * @return Material* 
	 */
	Material* get_material() const;
private:
	uint32_t m_index_count;
	uint32_t m_start_index;
	uint32_t m_start_vertex;

	Material* m_material;
};

