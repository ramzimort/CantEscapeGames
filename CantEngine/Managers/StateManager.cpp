/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "StateManager.h"
#include "EventManager.h"
#include "Events/State/StateEvents.h"


StateManager::StateManager(AppRenderer* AppRenderer, ResourceManager* ResourceManager, ScriptingManager* ScriptingManager)
	: m_pAppRenderer(AppRenderer), m_pResourceManager(ResourceManager), m_pScriptingManager(ScriptingManager)
{
	EventManager::Get()->SubscribeEvent<PushStateEvent>(this, std::bind(&StateManager::OnPushStateEvent, this, std::placeholders::_1));
	EventManager::Get()->SubscribeEvent<PopStateEvent>(this, std::bind(&StateManager::OnPopStateEvent, this, std::placeholders::_1));
}

StateManager::~StateManager()
{
	//Clears up the stateStack
	SwitchState(0);
}

void StateManager::UpdateStack(float dt)
{
	size_t index = m_stateStack.size() - 1;
	State *topState = m_stateStack[index];

	//Update only STATE on top of stack
	topState->Update(dt);
}

void StateManager::DrawStack(float dt)
{
	//Drawing happens from bottom to top
	// TODO - Do we want the GraphicsManager to actually draw between these calls?

	for (int i = 0; i < m_stateStack.size(); ++i)
	{
		//Draw call is the one that will pass the relevant info to the graphics manager
		State *state = m_stateStack[i];
		state->Draw(dt);
	}
}

void StateManager::SwitchState(const std::string& levelPath)
{
	State* state = new State(levelPath, m_pAppRenderer, m_pResourceManager, m_pScriptingManager);
	ClearStack();
	m_stateStack.push_back(state);
}

void StateManager::PushState(const std::string& levelPath)
{
	State* state = new State(levelPath, m_pAppRenderer, m_pResourceManager, m_pScriptingManager);
	m_stateStack.push_back(state);
}

void StateManager::PopState()
{
	size_t index = m_stateStack.size() - 1;

	if (index >= 0) 
	{
		State *topState = m_stateStack[index];
		delete topState;
		m_stateStack.pop_back();
	}
}


void StateManager::ClearStack() 
{
	for (State *state : m_stateStack)
		delete state;
	m_stateStack.clear();
}


void StateManager::OnPushStateEvent(const PushStateEvent* e)
{
	PushState(e->m_path);
}

void StateManager::OnPopStateEvent(const PopStateEvent* e)
{
	PopState();
}

void StateManager::ProcessInstantiationAndDestruction() 
{
	for (State *state : m_stateStack)
	{
		state->ProcessInstantiationAndDestruction();
	}
}
