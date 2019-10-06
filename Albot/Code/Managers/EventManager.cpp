#include "EventManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Graphics/AppRenderer.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "Graphics/Camera.h"
#include "Memory/CantMemory.h"
#include "Events/EventBus.h"
#include "ScriptingManager.h"

EventManager* EventManager::m_EventManager = new EventManager();

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
	delete m_pStateManager;
	delete m_pResourceManager;
	delete m_pAppRenderer;
	delete m_pInputManager;
	delete m_pCameraManager;
	delete m_pEventBus;
}

void EventManager::Initialize()
{
	m_pEventBus = new EventBus();
	m_pCameraManager = new CameraManager();
	m_pInputManager = new InputManager();
	m_pScriptingManager = new ScriptingManager();
	m_pResourceManager = new ResourceManager();
	m_pAppRenderer = new AppRenderer(*m_pInputManager->GetWindow(), m_pResourceManager, m_pCameraManager);
	m_pResourceManager->SetDXRenderer(m_pAppRenderer->GetDXRenderer());
	m_pStateManager = new StateManager();

	m_pStateManager->SwitchState(new State("level1.json", m_pAppRenderer, 
		m_pResourceManager, m_pScriptingManager));
	m_pAppRenderer->LoadContent();
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