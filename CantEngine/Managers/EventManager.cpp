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
#include "Directory/User.h"
#include "Factory/Factory.h"

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

#ifdef DEVELOPER
	delete m_pDebugManager;
#endif // DEVELOPER
}

void EventManager::Initialize(const std::string& levelPath, size_t width, size_t height)
{
	m_pEventBus = new EventBus();
	m_pCameraManager = new CameraManager();
	m_pInputManager = new InputManager();
	m_pResourceManager = new ResourceManager();
	m_pAppRenderer = new AppRenderer(*m_pInputManager->GetWindow(), m_pResourceManager, m_pCameraManager);

	DEBUG_INIT(
		CantDirectory::Path(),
		m_pInputManager->GetWindow(), 
		m_pAppRenderer->GetDXRenderer()->get_device(), 
		m_pAppRenderer->GetDXRenderer()->get_device_context());



	m_pScriptingManager = new ScriptingManager(m_pResourceManager);
	m_pResourceManager->Initialize(m_pAppRenderer->GetDXRenderer(), &m_pScriptingManager->luaState);
	Factory::Initialize(m_pResourceManager, m_pAppRenderer->GetDXRenderer(), m_pScriptingManager);
		
	m_pStateManager = new StateManager(m_pAppRenderer, m_pResourceManager, m_pScriptingManager);
#ifdef DEVELOPER
	m_pDebugManager = new CantDebug::DebugManager(m_pAppRenderer, m_pResourceManager, m_pStateManager);
#endif

	m_pStateManager->SwitchState(levelPath);

	m_pAppRenderer->LoadContent();

	m_pInputManager->SetWindowSize(width, height);
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

#ifdef DEVELOPER
	m_pDebugManager->Update();
#endif // DEVELOPER

	m_pAppRenderer->UpdateAppRenderer(dt);

	m_pAppRenderer->RenderApp();


	DEBUG_UPDATE;
	m_pAppRenderer->PresentApp();
	m_pEventBus->Update(dt);

}



bool EventManager::IsQuit()
{
	return m_pInputManager->IsQuit();
}