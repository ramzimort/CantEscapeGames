/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
- End Header --------------------------------------------------------*/

#pragma once

struct SDL_Window;
class FrameManager;

class WindowManager
{
public:
	WindowManager();
	~WindowManager();
	
	void Run();

private:
	SDL_Window* mp_window;
	FrameManager* mp_frame_manager;
};