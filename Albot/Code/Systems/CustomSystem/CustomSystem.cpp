/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


// 1 - Include the headers and add the line for the static type
#include "CustomSystem.h"
unsigned const CustomSystem::static_type = BaseSystem::numberOfTypes++;

#include "../../GameObjects/GameObject.h"
#include "../../Components/CustomComponent/CustomComponent.h"


CustomSystem::CustomSystem() :
	BaseSystem()
{
}


void CustomSystem::Register_GameObject(GameObject *go)
{
	//TODO - Improve performance (right now, all copies. Check if that can be improved)
	std::vector<CustomComponent*> const& customComps = go->GetAllComponents<CustomComponent>();

	BaseSystemCompNode *component_node = new CustomCompNode(customComps);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void CustomSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// Get the pointer to the components, and then freely update
	CustomCompNode *node = static_cast<CustomCompNode*>(compNode);


	//THIS COMPONENT IS PURELY LUA CODE
	for (CustomComponent *comp : node->n_components) 
	{
		//Temporal, because of weird issue with maps filling with nulls
		if (comp == nullptr) return;

		try
		{
			sol::table self = comp->m_luaScriptTable;
			self["Update"](self, dt, comp->GetOwner());
		}
		catch (const sol::error& e)
		{
			const char *errorName = e.what();
			OutputDebugString(errorName);		//TODO - erase later
		}
	}
}