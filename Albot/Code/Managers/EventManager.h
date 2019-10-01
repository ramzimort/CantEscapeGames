#pragma once

#include "Events/EventCallback.h"
#include "Events/EventBus.h"

class BaseEvent;
class WindowManager;
class CameraManager;
class AppRenderer;
class InputManager;
class ResourceManager;
class StateManager;
class EventBus;

class EventManager
{
public:
	EventManager();
	~EventManager();
	static EventManager* Get();
	
	void Initialize();
	void Update(float dt);
	bool IsQuit();

	template<typename T, typename ...Args>
	void SubscribeEvent(void* subscriber, Args... args);
	template<class T>
	void UnsubscribeEvent(void* objPtr);
	template<typename T,typename ...Args>
	void EnqueueEvent(bool, Args&&...);

private:
	WindowManager* m_pWindowManager;
	CameraManager* m_pCameraManager;
	AppRenderer* m_pAppRenderer;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	StateManager* m_pStateManager;
	EventBus* m_pEventBus;
	static EventManager* m_EventManager;
};


template<typename T, typename ...Args>
void EventManager::SubscribeEvent(void* subscriber, Args... args)
{
	m_pEventBus->AddSubscriber(subscriber, (EventCallBack<T>(std::forward<Args>(args)...)));
}

template<class T>
void EventManager::UnsubscribeEvent(void* objPtr)
{
	m_pEventBus->DeleteSubscriber(objPtr);
}


template<typename T, typename ...Args>
void EventManager::EnqueueEvent(bool directCall, Args&&... args)
{
	m_pEventBus->QueueEvent<T>(directCall, std::forward<Args>(args) ...);
}




