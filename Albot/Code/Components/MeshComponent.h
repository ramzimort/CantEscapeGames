#pragma once
#include "BaseComponent.h"
#include "Graphics/Model.h"
#include "Helper/Hash.h"

class Model;
class GameObjectManager;

struct VertexWorldSpaceData
{
	Vector3 m_position;
	Vector3 m_normal;
};

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
	friend class TransformSystem;
public:
	MeshComponent(GameObject *owner);
	virtual ~MeshComponent();

	void Init(ResourceManager* resMgr) override;
	void Begin(GameObjectManager *goMgr) override;
	Model* GetModel();

	//TODO: temp function, this public function is only for testing
	void SetModel(Model* model);
public:
	static ComponentId const static_type;
private:
	StringId m_modelId;
	Model* m_model;
	MeshType m_mesh_type;
	std::vector<VertexWorldSpaceData> m_vertices_world_space_list;
	bool m_calculate_missing_tangents;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};

