/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#pragma once

/**
 * @file InputManager.h
 * @author Ramzi Mourtada
 * @date 1/20/2019
 * @brief	Contains functions for checking mouse/keyboard press/trigger/release \n
 *			#include <SDL2/SDL_keycode.h> for key/mouse constants
 */

class InputManager
{
public:
	InputManager();
	~InputManager();
	void Update();
	bool IsKeyPressed(unsigned int KeyScanCode);
	
	/** Get mouse position in screen space as int[2] array (depends on window width/height) */
	int* GetPointerLocation();
	/** Get mouse position in screen space as vec2 (depends on window width/height) */
	Vector2 GetPointerLocVec2();
	/** Get mouse position change b/w frames as vec2 (depends on window width/height) */
	Vector2 GetPointerDeltaVec2();

	/** Check mouse button press */
	bool IsMousePressed(unsigned int MouseScanCode);
	/** Check mouse button trigger (rising edge) */
	bool IsMouseTriggered(unsigned int MouseScanCode);
	/** Check mouse button release (falling edge) */
	bool IsMouseReleased(unsigned int MouseScanCode);
	/** Returns scrolling amount in y-direction*/
	Sint32 GetMouseScroll();

	SDL_Window* GetWindow();
	/** Check if exit button is pressed (close window) */
	bool IsQuit();

private:
	void UpdateMouseState();


private:
	SDL_Window* m_pWindow;
	std::array<bool, 512> m_keyboardState;

	Uint32 m_mouseStateCurrent;
	Uint32 m_mouseStatePrevious;
	int m_mousePositionCurrent[2];
	int m_mousePositionPrevious[2];
	Sint32 m_mouseWheelY;
	SDL_Event m_event;
	
	bool m_update;
	bool m_quit;
};
