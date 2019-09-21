

#include "Managers/WindowManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"


// TODO - REMOVE LATER (jose)
#include "Managers/StateManager.h"
StateManager *stateMgr;


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

	// TODO - REMOVE LATER (jose)
	stateMgr = new StateManager();
	stateMgr->SwitchState(new State("level1.json"));


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


		// TODO - REMOVE LATER (jose)
		stateMgr->ProcessInstantiationAndDestruction();
		stateMgr->UpdateStack(dt);
		stateMgr->DrawStack(dt);
		
		
		DEBUG_UPDATE
		frame_manager.EndFrame();
	}

	DEBUG_CLOSE;

	// TODO - REMOVE LATER (jose)
	delete stateMgr;

	return 0;
}
