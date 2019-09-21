/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "InputManager.h"



/**
 * @file InputManager.cpp
 * @author Ramzi Mourtada
 * @date 1/20/2019
 * @brief	Contains functions for checking mouse/keyboard press/trigger/release \n
 *			#include <SDL2/SDL_keycode.h> for key/mouse constants
 */

InputManager::InputManager()
{
	SDL_memset(m_Keyboard_Current_State, 0, 512 * sizeof(Uint8));
	SDL_memset(m_Keyboard_Previous_State, 0, 512 * sizeof(Uint8));
	SDL_JoystickEventState(SDL_ENABLE);
	m_Mouse_Current_State = 0;
	m_Mouse_Previous_State = 0;
}

InputManager::~InputManager()
{
}

/** \brief Updates members containing current frame and previous frame values \n
	#Update \n
	*/
void InputManager::Update()
{
	//SDL_Event sdl_event;
	//SDL_JoystickUpdate();
	m_wheel_y = 0;
	while (SDL_PollEvent(&m_Event))
	{
		//Update_ImGui_Event(m_Event);
		//SDL_JoystickUpdate();
		switch (m_Event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			break;
		case SDL_MOUSEWHEEL:
			m_wheel_y = m_Event.wheel.y;
			break;
		case SDL_WINDOWEVENT:
			switch (m_Event.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	int numberOffFetchedKeys = 0;

	const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOffFetchedKeys);
	if (numberOffFetchedKeys > 512)
		numberOffFetchedKeys = 512;
	SDL_memcpy(m_Keyboard_Previous_State, m_Keyboard_Current_State, 512 * sizeof(Uint8));
	SDL_memcpy(m_Keyboard_Current_State, pCurrentKeyStates, numberOffFetchedKeys * sizeof(Uint8));

	m_Mouse_Previous_State = m_Mouse_Current_State;
	m_Mouse_Previous_Position[0] = m_Mouse_Current_Position[0]; m_Mouse_Previous_Position[1] = m_Mouse_Current_Position[1];
	m_Mouse_Current_State = SDL_GetMouseState(m_Mouse_Current_Position, m_Mouse_Current_Position + 1);
}


/** \brief Check keybaord key press. \n
	#is_Key_Pressed \n
	\param unsigned int \a KeyScanCode | ex: SDLK_a for checking if 'a' pressed
	\return bool: returns true if pressed
	*/
bool InputManager::is_Key_Pressed(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (m_Keyboard_Current_State[KeyScanCode])
		return true;

	return false;
}

/** \brief Check keybaord key trigger (rising edge). \n
	#is_Key_Triggered \n
	\param unsigned int \a KeyScanCode | ex: SDLK_a for checking if 'a' triggered
	\return bool: returns true if triggered this frame
	*/
bool InputManager::is_Key_Triggered(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (m_Keyboard_Current_State[KeyScanCode] && !m_Keyboard_Previous_State[KeyScanCode])
		return true;

	return false;
}

/** \brief Check keybaord key release (falling edge). \n
	#is_Key_Released \n
	\param unsigned int \a KeyScanCode | ex: SDLK_a for checking if 'a' released
	\return bool: returns true if released this frame
	*/
bool InputManager::is_Key_Released(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (!m_Keyboard_Current_State[KeyScanCode] && m_Keyboard_Previous_State[KeyScanCode])
		return true;

	return false;
}

/** \brief Get mousition in screen space (depends on window width/height). \n
	#Get_Pointer_Location \n
	\return int*: returns pointer to array of two elements containing x,y values
	*/
int* InputManager::Get_Pointer_Location()
{
	return m_Mouse_Current_Position;
}

Vector2 InputManager::Get_Vec2_Pointer_Location()
{
	return Vector2(m_Mouse_Current_Position[0], m_Mouse_Current_Position[1]);
}

Vector2 InputManager::Get_Vec2_Pointer_Delta()
{
	int deltaX = m_Mouse_Current_Position[0] - m_Mouse_Previous_Position[0];
	int deltaY = m_Mouse_Current_Position[1] - m_Mouse_Previous_Position[1];

	return Vector2(deltaX, deltaY);
}

/** \brief Check mouse button press. \n
	#is_Mouse_Pressed \n
	\param unsigned int \a MouseScanCode | ex: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
	\return bool: returns true if pressed
	*/
bool InputManager::is_Mouse_Pressed(unsigned int MouseScanCode)
{
	return (m_Mouse_Current_State & SDL_BUTTON(MouseScanCode));
}

/** \brief Check mouse button trigger (rising edge). \n
	#is_Mouse_Triggered \n
	\param unsigned int \a MouseScanCode | ex: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
	\return bool: returns true if triggered this frame
	*/
bool InputManager::is_Mouse_Triggered(unsigned int MouseScanCode)
{
	return (m_Mouse_Current_State & ~m_Mouse_Previous_State & SDL_BUTTON(MouseScanCode));
}

/** \brief Check mouse button release (falling edge) \n
	#is_Mouse_Released \n
	\param unsigned int \a MouseScanCode | ex: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, SDL_BUTTON_RIGHT
	\return bool: returns true if released this frame
	*/
bool InputManager::is_Mouse_Released(unsigned int MouseScanCode)
{
	return (m_Mouse_Previous_State & ~m_Mouse_Current_State & SDL_BUTTON(MouseScanCode));
}

Sint32 InputManager::Get_Mouse_Scroll()
{
	return m_wheel_y;
}

/** \brief Check if exit button is pressed (close window) \n
	#is_Quit \n
	\return bool: returns true if closed
	*/
bool InputManager::is_Quit()
{
	return quit;
}


bool InputManager::Any_Keyboard_Key_Pressed() const
{
	for (int i = 0; i < 512; ++i)
	{
		if (m_Keyboard_Current_State[i] && !m_Keyboard_Previous_State[i])
		{
			return true;
		}
	}

	return false;
}