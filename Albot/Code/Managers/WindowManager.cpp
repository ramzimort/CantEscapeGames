/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
- End Header --------------------------------------------------------*/

#include "WindowManager.h"
#include <string>
#include <assert.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_events.h"
#include "FrameManager.h"

WindowManager::WindowManager()
{
	int error = 0;
	error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	assert(error >= 0);

	SDL_GL_LoadLibrary(NULL);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	mp_window = SDL_CreateWindow("AL-Bot",					
		SDL_WINDOWPOS_CENTERED,									
		SDL_WINDOWPOS_CENTERED,									
		800,
		600,												
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	assert(mp_window != NULL);

	SDL_GL_SetSwapInterval(1);
	SDL_ShowCursor(SDL_DISABLE);

	mp_frame_manager = new FrameManager();
}

WindowManager::~WindowManager()
{
	delete mp_frame_manager;
}

void WindowManager::Run()
{
	SDL_Event quit_event;

	// While EventManager has not quit 
	bool isQuit = false;
	while (!isQuit)
	{
		while (SDL_PollEvent(&quit_event))
			if (quit_event.type == SDL_QUIT)
				isQuit = true;

		// Start frame
		mp_frame_manager->StartFrame();
		//float delta = m_frame_rate_controller.Get_Frame_Time();

		//World::Get()->Get_Input_Manager()->Update();

		//World::Get()->Get_Event_Manager()->Process_Queued_Events(delta);

		//End frame
		mp_frame_manager->EndFrame();

		SDL_GL_SwapWindow(mp_window);
	}
}
