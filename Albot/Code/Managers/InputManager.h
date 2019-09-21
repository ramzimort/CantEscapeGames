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



class ControllerComponent;

class InputManager
{
public:
	InputManager();
	~InputManager();

	/** Updates members containing current frame and previous frame values */
	void Update();
	/** \brief Check keybaord key press */
	bool is_Key_Pressed(unsigned int KeyScanCode);
	/** Check keybaord key trigger (rising edge). */
	bool is_Key_Triggered(unsigned int KeyScanCode);
	/** Check keybaord key release(falling edge) */
	bool is_Key_Released(unsigned int KeyScanCode);
	/** Get mouse position in screen space as int[2] array (depends on window width/height) */
	int* Get_Pointer_Location();
	/** Get mouse position in screen space as vec2 (depends on window width/height) */
	Vector2 Get_Vec2_Pointer_Location();
	/** Get mouse position change b/w frames as vec2 (depends on window width/height) */
	Vector2 Get_Vec2_Pointer_Delta();

	/** Check mouse button press */
	bool is_Mouse_Pressed(unsigned int MouseScanCode);
	/** Check mouse button trigger (rising edge) */
	bool is_Mouse_Triggered(unsigned int MouseScanCode);
	/** Check mouse button release (falling edge) */
	bool is_Mouse_Released(unsigned int MouseScanCode);
	/** Returns scrolling amount in y-direction*/
	Sint32 Get_Mouse_Scroll();

	/** Check if exit button is pressed (close window) */
	bool is_Quit();

	bool Any_Keyboard_Key_Pressed() const;

private:
	SDL_Event m_Event;
	Uint8 m_Keyboard_Current_State[512];
	Uint8 m_Keyboard_Previous_State[512];

	Uint32 m_Mouse_Current_State;
	Uint32 m_Mouse_Previous_State;

	int m_Mouse_Current_Position[2];
	int m_Mouse_Previous_Position[2];
	Sint32 m_wheel_y;

	bool quit = { false };
};

