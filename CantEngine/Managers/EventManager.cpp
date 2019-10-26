#include "EventManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "Graphics/AppRenderer.h"
#include "FrameManager.h"
#include "ResourceManager.h"
#include "StateManager.h"
#include "AudioManager.h"
#include "Graphics/Camera.h"
#include "Memory/CantMemory.h"
#include "Events/EventBus.h"
#include "ScriptingManager.h"
#include "Directory/User.h"
#include "Factory/Factory.h"

EventManager* EventManager::m_EventManager = new EventManager();

EventManager::EventManager()
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);
	//std::cout << "This works" << std::endl;
}

EventManager::~EventManager()
{
	delete m_pStateManager;
	delete m_pResourceManager;
	delete m_pAppRenderer;
	delete m_pAudioManager;
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
	m_pAudioManager = new AudioManager();
	m_pResourceManager = new ResourceManager();
	m_pAppRenderer = new AppRenderer(*m_pInputManager->GetWindow(), m_pResourceManager, m_pCameraManager);

	DEBUG_INIT(
		CantDirectory::Path(),
		m_pInputManager->GetWindow(), 
		m_pAppRenderer->GetDXRenderer()->get_device(), 
		m_pAppRenderer->GetDXRenderer()->get_device_context());


	m_pAppRenderer->InitializeResources();
	m_pScriptingManager = new ScriptingManager(m_pResourceManager);
	m_pResourceManager->Initialize(m_pAppRenderer->GetDXRenderer(), &m_pScriptingManager->luaState, m_pAudioManager);
	Factory::Initialize(m_pResourceManager, m_pAppRenderer->GetDXRenderer(), m_pScriptingManager);
		
	m_pStateManager = new StateManager(m_pAppRenderer, m_pResourceManager, m_pScriptingManager);
#ifdef DEVELOPER
	m_pDebugManager = new CantDebug::DebugManager(m_pAppRenderer, m_pResourceManager, m_pStateManager);
#endif

	m_pStateManager->SwitchState(levelPath);

	m_pAppRenderer->LoadContent();

	m_pInputManager->SetWindowSize(width, height);

	SubscribeEvent<QuitEvent>(this, std::bind(&EventManager::OnQuit, this, std::placeholders::_1));
}

void EventManager::OnQuit(const QuitEvent* e)
{
	m_quit1 = true;
	m_quit2 = true;

}

EventManager* EventManager::Get()
{
	return m_EventManager;
}

void EventManager::RunInputThread()
{
	m_pInputManager->Update();
	m_quit2 = true;

}

void EventManager::RunGameThread()
{
	EventManager* World = EventManager::Get();
	FrameManager frame_manager;
	bool done = false;
	float dt;
	while (!m_quit2)
	{
		frame_manager.StartFrame();
		dt = static_cast<float>(frame_manager.GetFloatFrameTime());
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
		m_pAudioManager->Update(dt);
		frame_manager.EndFrame();
	}
}



bool EventManager::IsQuit()
{
	return m_pInputManager->IsQuit();
}