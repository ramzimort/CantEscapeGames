#include "LightSystem.h"
#include "GameObjects/GameObject.h"
#include "Components/LightComponent.h"
#include "Components/TransformComponent.h"
#include "Graphics/AppRenderer.h"

unsigned const LightSystem::static_type = BaseSystem::numberOfTypes++;

LightSystem::LightSystem()
{
	Push_required_comp<TransformComponent>();
	Push_required_comp<LightComponent>();
}


LightSystem::~LightSystem()
{
}


void LightSystem::Register_GameObject(GameObject *go)
{
	TransformComponent *transform = go->GetComponent<TransformComponent>();
	LightComponent* light_comp = go->GetComponent<LightComponent>();

	BaseSystemCompNode *component_node = new LightCompNode(transform, light_comp);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}

void LightSystem::Update(float dt, BaseSystemCompNode *compNode)
{

}

void LightSystem::Draw(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	LightCompNode *node = static_cast<LightCompNode*>(compNode);
	TransformComponent *transformComp = node->m_transform;
	LightComponent* lightComp = node->m_light_comp;

	const Light* light = lightComp->GetLight();

	if (light->GetLightType() == ELightType::POINT_LIGHT)
	{
		PointLightInstanceData light_inst_data = {};
		light_inst_data.light = light;
		light_inst_data.light_position = transformComp->GetPosition() + light->GetPosition();
		m_pAppRenderer->RegisterPointLightInstance(light_inst_data);
	}
	else if (light->GetLightType() == ELightType::DIRECTIONAL_LIGHT)
	{
		DirectionalLightInstanceData direction_light_inst_data = {};
		direction_light_inst_data.light = light;

		Matrix inverse_rot_mat = transformComp->GetRotationMatrix().Transpose();
		Vector3 light_dir = Vector3::Transform(Vector3(0.f, 0.f, -1.f), inverse_rot_mat);
		light_dir.Normalize();
		direction_light_inst_data.light_direction = light_dir;
		m_pAppRenderer->RegisterDirectionalLightInstance(direction_light_inst_data);
	}
}

void LightSystem::RegisterAppRenderer(AppRenderer* appRenderer)
{
	m_pAppRenderer = appRenderer;
}