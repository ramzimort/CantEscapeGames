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


//GameObject experiment (TODO - ERASE)
#include "../GameObjects/GameObject.h"
#include "../Components/TransformComponent.h"
#include "../Components/TestComponent.h"
//GameObject experiment (TODO - erase)


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


		//GameObject experiment (TODO - erase)
		GameObject *go01 = new GameObject("Carlos");
		GameObject *go02 = new GameObject("Juan");
		GameObject *go03 = new GameObject("Diego");
		go01->AddComponent<Transform>();
		go02->AddComponent<TestComp>();
		Transform *T1 = go03->AddComponent<Transform>();
		TestComp *T2 = go03->AddComponent<TestComp>();

		Transform *T = go01->GetComponent<Transform>();

		delete go01;
		delete go02;
		delete go03;
		//GameObject experiment (TODO - erase)



		mp_frame_manager->EndFrame();
	}
}
