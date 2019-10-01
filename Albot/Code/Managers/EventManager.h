#pragma once

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

private:
	WindowManager* m_pWindowManager;
	CameraManager* m_pCameraManager;
	AppRenderer* m_pAppRenderer;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	StateManager* m_pStateManager;
	EventBus* m_pEventBus;
	static EventManager* m_EventManager;

private:

};

