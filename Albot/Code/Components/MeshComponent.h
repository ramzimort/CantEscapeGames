#pragma once
#include "BaseComponent.h"
#include "Graphics/Model.h"

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

class MeshComponent :	public BaseComponent
{
public:
	friend class Factory;
public:
	MeshComponent(GameObject *owner);
	virtual ~MeshComponent();

	void Init() override;
	void Begin() override;
	Model* GetModel();

	//TODO: temp function, this public function is only for testing
	void SetModel(Model* model);
public:
	static ComponentId const static_type;
private:
	Model* m_model;
	MeshType m_mesh_type;
	std::string m_model_name;
	bool m_calculate_missing_tangents;

	//RTTR_ENABLE(BaseComponent);
	//RTTR_REGISTRATION_FRIEND;
};

