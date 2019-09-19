

#include "Managers/WindowManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"


#ifdef DEVELOPER
#include "Factory/Factory.h"
#include "Managers/SystemManager.h"
#include "Managers/GameObjectManager.h"

GameObjectManager *goMgr;
SystemManager *sysMgr;
Factory *factory;
#endif


int main()
{
	// Create Window
	WindowManager window_manager;
	
	DEBUG_INIT(window_manager.SDLCreateWindow("CantDebug", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI));

	SDL_Window* main_window = window_manager.SDLCreateWindow("Albot", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	FrameManager frame_manager;
	SDL_Event quit_event;
	bool done = false;
	float dt;


	#ifdef DEVELOPER
	goMgr = new GameObjectManager();
	sysMgr = new SystemManager();
	factory = new Factory();
	#endif


	while (!done)
	{
		while (SDL_PollEvent(&quit_event))
		{
			if (quit_event.type == SDL_QUIT)
				done = true;
			if (quit_event.type == SDL_WINDOWEVENT && quit_event.window.event == SDL_WINDOWEVENT_CLOSE && quit_event.window.windowID == SDL_GetWindowID(main_window))
				done = true;
		}
		frame_manager.StartFrame();
		
		dt = static_cast<float>(frame_manager.GetFrameTime());

		DEBUG_TRACE("Frame Time: %f ms", dt);
		DEBUG_TRACE("Framerate: %f Hz", 1000.f/dt);

		
		#ifdef DEVELOPER
		goMgr->ProcessQueues();
		sysMgr->Update(dt);
		#endif

		
		DEBUG_UPDATE
		frame_manager.EndFrame();
	}

	DEBUG_CLOSE;

	#ifdef DEVELOPER
	delete goMgr;
	delete sysMgr;
	delete factory;
	#endif

	return 0;
}
