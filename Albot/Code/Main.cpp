

#include "Managers/WindowManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"


// TODO - REMOVE LATER (jose)
#include "Managers/StateManager.h"
StateManager *stateMgr;
#include "Managers/ScriptingManager.h"
ScriptingManager *luaMgr;


//TODO: remove later (ALBERTO ;_T)
#include "Managers/ResourceManager.h"
#include "Graphics/AppRenderer.h"
#include "Managers/CameraManager.h"
#include "Graphics/Camera.h"
#include "Managers/InputManager.h"
AppRenderer* gAppRenderer;
ResourceManager* gResourceManager;
CameraManager* gCameraManager;
InputManager* gInputManager;
WindowManager* gWindowManager;



int CALLBACK WinMain(
	__in  HINSTANCE hInstance,
	__in  HINSTANCE hPrevInstance,
	__in  LPSTR lpCmdLine,
	__in  int nCmdShow
)
{
	// Create Window
	gWindowManager = new WindowManager();

	SDL_Window* main_window = gWindowManager->SDLCreateWindow("Albot", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	DEBUG_INIT;
	FrameManager frame_manager;
	bool done = false;
	float dt;

	//TODO - ALBERTO STUFF ;)
	gCameraManager = new CameraManager();
	gResourceManager = new ResourceManager();
	gAppRenderer = new AppRenderer(*main_window, gResourceManager, gCameraManager);
	gInputManager = new InputManager();
	gResourceManager->SetDXRenderer(gAppRenderer->GetDXRenderer());
	int32_t windowWidth, windowHeight;
	SDL_GetWindowSize(main_window, &windowWidth, &windowHeight);
	Camera* camera = new Camera(windowWidth, windowHeight, 45.f,
		0.1f, 1000.f, Vector3(0.0, 0.0, 20.f));
	gCameraManager->RegisterCamera("Main", camera);

	// TODO - REMOVE LATER (jose)
	stateMgr = new StateManager();
	stateMgr->SwitchState(new State("level1.json"));
	luaMgr = new ScriptingManager();
	//

	DEBUG_INIT;
	DEBUG_LOG("Frame Time: %f ms\n", 20.f);
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
		gAppRenderer->UpdateAppRenderer(dt);
		gAppRenderer->RenderApp();
		gAppRenderer->PresentApp();

		frame_manager.EndFrame();
		DEBUG_UPDATE
	}
	gAppRenderer->Release();
	// TODO - REMOVE LATER (jose)
	delete stateMgr;
	delete luaMgr;

	return 0;
}
