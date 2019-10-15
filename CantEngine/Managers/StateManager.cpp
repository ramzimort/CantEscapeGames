/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "StateManager.h"


StateManager::StateManager()
{
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

void StateManager::SwitchState(State *newState)
{
	//Empty the stateStack (deletion could be another thread maybe?)
	for (State *state : m_stateStack) 
		delete state;
	m_stateStack.clear();

	//This is for having this call also clear the stack when called with 0
	if (newState == nullptr) return;

	//Now push the new state
	PushState(newState);
}

void StateManager::PushState(State *state)
{
	// TODO - Check what else may need to be done
	m_stateStack.push_back(state);
}

void StateManager::PopState()
{
	size_t index = m_stateStack.size() - 1;
	State *topState = m_stateStack[index];
	delete topState;
	m_stateStack.pop_back();
}

void StateManager::ProcessInstantiationAndDestruction() 
{
	for (State *state : m_stateStack)
	{
		state->ProcessInstantiationAndDestruction();
	}
}
