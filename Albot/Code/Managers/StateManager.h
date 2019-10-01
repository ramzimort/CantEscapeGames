/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#pragma once

//#include <vector>
#include "States/State.h"

class StateManager
{
public:
	StateManager();
	~StateManager();

	void UpdateStack(float dt);
	void DrawStack(float dt);
	void ProcessInstantiationAndDestruction();

	//This would be called when changing states, it empties 
	//the stack, and pushes a state into the empty one. 
	void SwitchState(State *newState);

	//These are called when we dont want to replace 
	//a state, but put other states on top
	void PushState(State *state);
	void PopState();

private:
	std::vector<State*> m_stateStack;
};

