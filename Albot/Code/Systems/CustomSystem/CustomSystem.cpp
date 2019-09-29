/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "CustomSystem.h"
#include "../../GameObjects/GameObject.h"
unsigned const CustomSystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../../Components/CustomComponent/CustomComponent.h"


CustomSystem::CustomSystem() :
	BaseSystem()
{
	//Push the comp to set the comp mask
	//Push_required_comp<TestComp>();

	//Here we usually define the parameters for later suscribing GO to this system
	//Since this is a special case, we will leave it empty for now
	// TODO
}


void CustomSystem::Register_GameObject(GameObject *go)
{
	//TODO - Improve performance (right now, all copies. Check if that can be improved)
	std::vector<CustomComponent*> customComps = go->GetAllComponents<CustomComponent>();

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
		//We can retrieve info from the comp here, like the name assigned 
		//(which should be the same as the lua script name)

		try
		{
			sol::table self = (*(comp->luaState))[comp->name];
			if(self.valid())
				self["Update"](self, dt );// , comp->GetOwner());
		}
		catch (const sol::error& e)
		{
			const char *errorName = e.what();
			OutputDebugString(errorName);
		}
	}
}