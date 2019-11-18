#pragma once

#include "Events/EventCallback.h"
#include "Events/EventBus.h"
#include "Events/State/StateEvents.h"
#include "CantDebug/DebugManager.h"

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

class EventManager
{
public:
	friend class ScriptingManager;

public:
	EventManager();
	~EventManager();
	static EventManager* Get();
	void OnQuit(const QuitEvent* e);
	void Initialize(const std::string& gameName, const std::string& levelName, bool fullscreen, int width, int height);
	void RunInputThread();
	void RunGameThread();
	bool IsQuit();
	template<typename T, typename ...Args>
	void SubscribeEvent(Args... args);
	template<typename T>
	void UnsubscribeEvent(void* objPtr);
	template<typename T,typename ...Args>
	void EnqueueEvent(Args&&...);

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