#pragma once

#include "Events/EventCallback.h"
#include "Events/EventBus.h"
#include "Events/State/StateEvents.h"
#include "CantDebug/DebugManager.h"

/**
 * @file EventManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Main singleton class for Event Communication and Thread-Safety
 */

class BaseEvent;
class CameraManager;
class AppRenderer;
class InputManager;
class ResourceManager;
class StateManager;
class ScriptingManager;
class EventBus;
class DebugManager;
class AudioManager;
class UserManager;

/**
 * @brief Main singleton class for Event Communication and Thread-Safety
 * 
 */
class EventManager
{
public:
	friend class ScriptingManager;

public:
	EventManager();
	~EventManager();
	/**
	 * @brief Returns the EventManager singleton
	 * 
	 * @return EventManager* 
	 */
	static EventManager* Get();
	/**
	 * @brief Initialize the engine with startup level and window parameters
	 *
	 * @param gameName
	 * @param levelName
	 * @param fullscreen
	 * @param width
	 * @param height
	 */
	void Initialize(const std::string& gameName, const std::string& levelName, bool fullscreen, int width, int height);
	/**
	 * @brief Runs the input wait event loop on its own thread
	 * 
	 */
	void RunInputThread();
	/**
	 * @brief Runs the game loop on its own thread and updates on a frame time basis
	 * 
	 */
	void RunGameThread();
	/**
	 * @brief Check if end condition requested
	 * 
	 * @return true 
	 * @return false 
	 */
	bool IsQuit();
	/**
	 * @brief Subscribe and execute callback whenever event is called. 
	 * 
	 * @tparam T 
	 * @tparam Args 
	 * @param args 
	 */
	template<typename T, typename ...Args>
	void SubscribeEvent(Args... args);
	/**
	 * @brief Unsubscribe object pointer from event callback.
	 * 
	 * @tparam T 
	 * @param objPtr 
	 */
	template<typename T>
	void UnsubscribeEvent(void* objPtr);
	/**
	 * @brief Preferable to set immediate call to false for thread safety
	 * 
	 * @tparam T 
	 * @tparam Args (bool immediateCall, event constructor args)
	 */
	template<typename T,typename ...Args>
	void EnqueueEvent(Args&&...);

private:
	void OnQuit(const QuitEvent* e);
private:
	AudioManager* m_pAudioManager;
	ScriptingManager *m_pScriptingManager;
	CameraManager* m_pCameraManager;
	AppRenderer* m_pAppRenderer;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	StateManager* m_pStateManager;
	EventBus* m_pEventBus;
	UserManager* m_pUserManager;

	static EventManager* m_EventManager;

	std::mutex m_mutex;
	bool m_quit1;
	bool m_quit2;

#ifdef DEVELOPER
	CantDebug::DebugManager* m_pDebugManager;
#endif // !DEVELOPER


};


template<typename T, typename ...Args>
void EventManager::SubscribeEvent(Args... args)
{
	m_pEventBus->AddSubscriber(EventCallback<T>(std::forward<Args>(args)...));
}

template<typename T>
void EventManager::UnsubscribeEvent(void* objPtr)
{
	m_pEventBus->DeleteSubscriber<T>(objPtr);
}


template<typename T, typename ...Args>
void EventManager::EnqueueEvent(Args&&... args)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_pEventBus->QueueEvent<T>(std::forward<Args>(args)...);
}