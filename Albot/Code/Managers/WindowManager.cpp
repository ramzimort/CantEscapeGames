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
}

WindowManager::~WindowManager()
{ }

SDL_Window* WindowManager::SDLCreateWindow(const char* title, int32_t resx, int32_t resy, int32_t flags)
{
	SDL_Window* window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		resx,
		resy,
		flags);
	assert(window != NULL);
	m_pwindows.push_back(window);
	m_numwindows++;

	return window;
}

SDL_Window* WindowManager::GetWindow(const int32_t index) const
{
	if (index >= m_numwindows)
		return nullptr;

	return m_pwindows.at(index);
}
