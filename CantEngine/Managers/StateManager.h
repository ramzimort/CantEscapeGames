/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#pragma once

//#include <vector>
#include "States/State.h"
class AppRenderer;
class ScriptingManager;
class ResourceManager;
class PushStateEvent;
class PopStateEvent;

class StateManager
{
public:
	StateManager(AppRenderer* m_pAppRenderer, ResourceManager* m_pResourceManager, ScriptingManager* m_pScriptingManager);
	~StateManager();

	void UpdateStack(float dt);
	void DrawStack(float dt);
	void ProcessInstantiationAndDestruction();

	//This would be called when changing states, it empties 
	//the stack, and pushes a state into the empty one. 
	void SwitchState(const std::string& levelPath = "");

	//These are called when we dont want to replace 
	//a state, but put other states on top
	void PushState(const std::string& levelPath = "");
	void PopState();

	void OnPushStateEvent(const PushStateEvent* e);
	void OnPopStateEvent(const PopStateEvent* e);

private:
	std::vector<State*> m_stateStack;

	AppRenderer* m_pAppRenderer;
	ResourceManager* m_pResourceManager; 
	ScriptingManager* m_pScriptingManager;
};

