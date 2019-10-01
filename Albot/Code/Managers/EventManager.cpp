#include "EventManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Graphics/AppRenderer.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "Graphics/Camera.h"
#include "Memory/CantMemory.h"
#include "Events/EventBus.h"

EventManager* EventManager::m_EventManager = new EventManager();

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::Initialize()
{
	m_pInputManager = new InputManager();
	m_pInputManager->Initialize();

	m_pCameraManager = new CameraManager();
	m_pResourceManager = new ResourceManager();
	m_pAppRenderer = new AppRenderer(*m_pInputManager->GetWindow(), m_pResourceManager, m_pCameraManager);
	m_pResourceManager->SetDXRenderer(m_pAppRenderer->GetDXRenderer());
	m_pStateManager = new StateManager();
	m_pEventBus = new EventBus();


	// REMOVE
	SDL_Window* main_window = m_pInputManager->GetWindow();
	int32_t windowWidth, windowHeight;
	SDL_GetWindowSize(main_window, &windowWidth, &windowHeight);
	Camera* camera = new Camera(windowWidth, windowHeight, 45.f,
		0.1f, 1000.f, Vector3(0.0, 0.0, 20.f));
	m_pCameraManager->RegisterCamera("Main", camera);
	m_pStateManager->SwitchState(new State("level1.json", m_pAppRenderer, m_pResourceManager));
}

EventManager* EventManager::Get()
{
	return m_EventManager;
}

void EventManager::Update(float dt)
{
	m_pInputManager->Update();
	m_pStateManager->ProcessInstantiationAndDestruction();
	m_pStateManager->UpdateStack(dt);
	m_pStateManager->DrawStack(dt);
	m_pAppRenderer->UpdateAppRenderer(dt);
	m_pAppRenderer->RenderApp();
	m_pAppRenderer->PresentApp();
	m_pEventBus->Update(dt);
}



bool EventManager::IsQuit()
{
	return m_pInputManager->IsQuit();
}