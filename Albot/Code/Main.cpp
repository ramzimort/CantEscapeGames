
#include <iostream>
#include <SDL2/SDL_events.h>

#include "Managers/WindowManager.h"
#include "Managers/FrameManager.h"

#ifdef DEVELOPER
#include "..\..\CantDebug\CantDebug.h"
#endif

int main()
{
	// Create Window
	WindowManager window_manager;
	SDL_Window* main_window = window_manager.CreateWindow("Albot", 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);

#ifdef DEVELOPER
	SDL_Window* debug_window = window_manager.CreateWindow("CantDebug", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	CantDebug::InitDebugWindow(debug_window);
#endif // DEVELOPER

	FrameManager frame_manager;
	SDL_Event quit_event;
	bool done = false;
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

#ifdef DEVELOPER
		int f = frame_manager.GetFrameTime();

		CantDebug::Print("Test");

		CantDebug::UpdateDebugWindow();
#endif
		frame_manager.EndFrame();
	}

	return 0;
}