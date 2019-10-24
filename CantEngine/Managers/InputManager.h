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
	bool IsKeyPressed(unsigned int KeyScanCode) const; 
	
	/** Get mouse position in screen space as int[2] array (depends on window width/height) */
	const int* GetPointerLocation() const;
	/** Get mouse position in screen space as vec2 (depends on window width/height) */
	Vector2 GetPointerLocVec2() const;
	/** Get mouse position change b/w frames as vec2 (depends on window width/height) */
	Vector2 GetPointerDeltaVec2() const;

	/** Check mouse button press */
	bool IsMousePressed(unsigned int MouseScanCode) const;
	/** Check mouse button trigger (rising edge) */
	bool IsMouseTriggered(unsigned int MouseScanCode) const;
	/** Check mouse button release (falling edge) */
	bool IsMouseReleased(unsigned int MouseScanCode) const;
	/** Returns scrolling amount in y-direction*/
	Sint32 GetMouseScroll() const;

	void SetWindowSize(size_t width, size_t height);

	SDL_Window* GetWindow();
	/** Check if exit button is pressed (close window) */
	bool IsQuit() const;

private:
	void UpdateMouseClickState();
private:
	SDL_Window* m_pWindow;
	std::array<bool, 512> m_keyboardState;
	Uint8 m_mouseStateCurrent;
	Uint8 m_mouseStatePrevious;
	int m_mousePositionCurrent[2];
	int m_mousePositionPrevious[2];
	Sint32 m_mouseWheelY;
	SDL_Event m_event;
	bool m_quit;
};

