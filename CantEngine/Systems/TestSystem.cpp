/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

// 1 - Include the headers and add the line for the static type
#include "TestSystem.h"
#include "../GameObjects/GameObject.h"
unsigned const TestSystem::static_type = BaseSystem::numberOfTypes++;

// 2 - Include the components you want to add
#include "../Components/TestComponent.h"
#include "Components/TransformComponent.h"


TestSystem::TestSystem() :
	BaseSystem()
{
	// 3 - Push the comp to set the comp mask
	Push_required_comp<TestComp>();
}


void TestSystem::Register_GameObject(GameObject *go)
{
	// 4 - Register by adding the components to the system

	TestComp *test = go->GetComponent<TestComp>();


	BaseSystemCompNode *component_node = new TestCompCompNode(test);
	component_node->m_goID = go->GetId();

	this->m_ObjComponentsMap[go->GetId()] = component_node;
}


void TestSystem::Update(float dt, BaseSystemCompNode *compNode)
{
	// 5 - Get the pointer to the components, and then freely update
	TestCompCompNode *node = static_cast<TestCompCompNode*>(compNode);
	TestComp *T = node->n_testComp;

	TransformComponent* transform_comp = T->GetOwner()->GetComponent<TransformComponent>();

	//TODO: added by albert just to test AABB world space
	if (transform_comp)
	{
		const float rotateSpeedX = 160.f;
		const float rotateSpeedY = 90.f;
		transform_comp->Rotate(rotateSpeedX * dt, rotateSpeedY * dt, 0.f);
	}

	//UPDATE CODE GOES HERE
	totalTime += dt;
	if (dt >= 5.0f) 
	{
		T->onWhateverFires("Delegate fired from test system!", totalTime);
		totalTime = 0.0f;
	}
}

void TestSystem::printFloat(float a) 
{
	float b = a + 5;
}