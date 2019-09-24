#pragma once
#include "BaseComponent.h"
#include "Graphics/Mesh.h"

class Model;

enum MeshType
{
	MESH_QUAD,
	MESH_CUBE,
	MESH_MODEL,
	MESH_TEXTURED_MODEL,
	MESH_PLANE,
	MeshTypeCount
};

class MeshesComponent :	public BaseComponent
{
public:
	friend class Factory;
public:
	MeshesComponent(GameObject *owner);
	virtual ~MeshesComponent();

	void Init() override;
	void Begin() override;
	Model* GetModel();

	//TODO: temp function, this public function is only for testing
	void SetModel(Model* model);
public:
	static ComponentId const static_type;
private:
	Model* m_model;
	std::string m_model_name;
	MeshType m_mesh_type;
	bool m_calculate_missing_tangents;
};

