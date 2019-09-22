

#include "Managers/WindowManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"


// TODO - REMOVE LATER (jose)
#include "Managers/StateManager.h"
StateManager *stateMgr;


//TODO: remove later (ALBERTO ;_T)
#include "Managers/ResourceManager.h"
#include "Graphics/AppRenderer.h"
#include "Managers/CameraManager.h"
#include "Graphics/Camera.h"
#include "Managers/InputManager.h"
AppRenderer* appRenderer;
ResourceManager* resourceManager;
CameraManager* gCameraManager;
InputManager* gInputManager;
WindowManager* gWindowManager;


int main()
{
	// Create Window
	gWindowManager = new WindowManager();

	SDL_Window* main_window = gWindowManager->SDLCreateWindow("Albot", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	FrameManager frame_manager;
	SDL_Event quit_event;
	bool done = false;
	float dt;

	// TODO - REMOVE LATER (jose)
	stateMgr = new StateManager();
	stateMgr->SwitchState(new State("level1.json"));

	//TODO - ALBERTO STUFF ;)
	gCameraManager = new CameraManager();
	resourceManager = new ResourceManager();
	appRenderer = new AppRenderer(*main_window, resourceManager, gCameraManager);
	gInputManager = new InputManager();
	resourceManager->SetDXRenderer(appRenderer->GetDXRenderer());
	int32_t windowWidth, windowHeight;
	SDL_GetWindowSize(main_window, &windowWidth, &windowHeight);
	Camera* camera = new Camera(windowWidth, windowHeight, 45.f,
		0.1f, 1000.f, Vector3(0.0, 0.0, 20.f));
	gCameraManager->RegisterCamera("Main", camera);
	//
	DEBUG_INIT();
	while (!gInputManager->IsQuit())
	{
		frame_manager.StartFrame();
		
		dt = static_cast<float>(frame_manager.GetFloatFrameTime());

		DEBUG_TRACE("Frame Time: %f ms", dt);
		DEBUG_TRACE("Framerate: %f Hz", 1000.f/dt);

		gInputManager->Update();

		// TODO - REMOVE LATER (jose)
		stateMgr->ProcessInstantiationAndDestruction();
		stateMgr->UpdateStack(dt);
		stateMgr->DrawStack(dt);
		// TODO - ALBERTOOOO
		appRenderer->UpdateAppRenderer(dt);
		appRenderer->RenderApp();
		appRenderer->PresentApp();
		
		DEBUG_UPDATE
		frame_manager.EndFrame();
	}

	// TODO - REMOVE LATER (jose)
	delete stateMgr;

	return 0;
}
