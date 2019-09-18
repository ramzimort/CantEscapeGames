/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
- End Header --------------------------------------------------------*/

#pragma once
#include <vector>

struct SDL_Window;
class FrameManager;

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	SDL_Window* SDLCreateWindow(const char* title, int32_t resx, int32_t resy, int32_t flags);
	SDL_Window* GetWindow(const int32_t index) const;

private:
	std::vector<SDL_Window*> m_pwindows;
	int32_t m_numwindows;
};