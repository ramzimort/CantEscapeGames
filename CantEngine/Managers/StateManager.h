/**
 * @file StateManager.h
 * @author Jose Rosenbluth
 * @date 12/4/2019
 * @brief Class that manages the running and switching of game states
 */

#pragma once

//#include <vector>
#include "States/State.h"

class AppRenderer;
class ScriptingManager;
class ResourceManager;
class PushStateEvent;
class PopStateEvent;
class LoadStateEvent;
class PushLoadedStateEvent;


/**
 * @brief Class that manages the running and switching of game states
 * 
 */
class StateManager
{
public:
	/**
	 * @brief Construct a new State Manager object
	 * 
	 * @param m_pAppRenderer 
	 * @param m_pResourceManager 
	 * @param m_pScriptingManager 
	 */
	StateManager(AppRenderer* m_pAppRenderer, ResourceManager* m_pResourceManager, ScriptingManager* m_pScriptingManager);
	/**
	 * @brief Destroy the State Manager object
	 * 
	 */
	~StateManager();

	/**
	 * @brief Updates the current top State in the main stack
	 * 
	 * @param dt 
	 */
	void UpdateStack(float dt);
	/**
	 * @brief Draws, from bottom to top, every state in the stack (top state draws last, on top of other states)
	 * 
	 * @param dt 
	 */
	void DrawStack(float dt);
	/**
	 * @brief Calls ProcessQueues on the top of the stack state
	 * 
	 */
	void ProcessInstantiationAndDestruction();

	/**
	* @brief Switch state will wipe the current stack clean, and then push a new state defined by the levelPath parameter
	* 
	* @param levelPath 
	*/
	void SwitchState(const std::string& levelPath = "");
	/**
	 * @brief Clears the state stack
	 * 
	 */
	void ClearStack();

	/**
	 * @brief Pushes a new state on top of the stack
	 * 
	 * @param levelPath 
	 */
	void PushState(const std::string& levelPath = "");
	/**
	 * @brief Pops the state on top of the stack
	 * 
	 */
	void PopState();

	/**
	 * @brief Loads a state defined by the parameter levelPath
	 * 
	 * @param levelPath 
	 */
	void LoadState(const std::string& levelPath = "");
	
	/**
	 * @brief Pushes the loaded state onto the stack 
	 * 
	 * @param levelPath 
	 */
	void PushLoadedState();

	/**
	 * @brief Event fired when a state is pushed onto the stack
	 * 
	 * @param e 
	 */
	void OnPushStateEvent(const PushStateEvent* e);
	/**
	 * @brief Event fired when a state is popped from the stack
	 * 
	 * @param e 
	 */
	void OnPopStateEvent(const PopStateEvent* e);
	/**
	 * @brief Event fired when loading a state
	 * 
	 * @param e 
	 */
	void OnLoadStateEvent(const LoadStateEvent* e);
	/**
	 * @brief Event fired when pushing a loaded state onto the stack
	 * 
	 * @param e 
	 */
	void OnPushLoadedStateEvent(const PushLoadedStateEvent* e);

private:
	/**
	 * @brief The state manager state stack
	 * 
	 */
	std::vector<State*> m_stateStack;
	/**
	 * @brief state queue
	 * 
	 */
	std::queue<State*> m_stateQueue;

	/**
	 * @brief Pointer to the Graphic's manager
	 * 
	 */
	AppRenderer* m_pAppRenderer;
	/**
	 * @brief Pointer to the Resource manager
	 * 
	 */
	ResourceManager* m_pResourceManager; 
	/**
	 * @brief Pointer to the Scripting manager
	 * 
	 */
	ScriptingManager* m_pScriptingManager;
#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
};

