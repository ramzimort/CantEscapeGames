/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
- End Header --------------------------------------------------------*/

#include "WindowManager.h"
#include <string>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include "FrameManager.h"

WindowManager::WindowManager()
{
	int error = 0;
	error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	assert(error >= 0);

	mp_window = SDL_CreateWindow("AL-Bot",					
		SDL_WINDOWPOS_CENTERED,									
		SDL_WINDOWPOS_CENTERED,									
		800,
		600,												
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	assert(mp_window != NULL);

	mp_frame_manager = new FrameManager();
}

WindowManager::~WindowManager()
{
	delete mp_frame_manager;
}

void WindowManager::Run()
{
	SDL_Event quit_event;

	bool isQuit = false;
	while (!isQuit)
	{
		while (SDL_PollEvent(&quit_event))
			if (quit_event.type == SDL_QUIT)
				isQuit = true;

		mp_frame_manager->StartFrame();



		mp_frame_manager->EndFrame();
	}
}
