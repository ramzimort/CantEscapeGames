/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
-End Header--------------------------------------------------------*/

#include "InputManager.h"
#include <SDL2\SDL_keyboard.h>
#include <SDL2/SDL_gamecontroller.h>


/**
 * @file InputManager.cpp
 * @author Ramzi Mourtada
 * @date 1/20/2019
 * @brief	Contains functions for checking mouse/keyboard press/trigger/release \n
 *			#include <SDL2/SDL_keycode.h> for key/mouse constants
 */

InputManager::InputManager()
{
	//SDL_memset(m_Keyboard_Current_State, 0, 512 * sizeof(Uint8));
	//SDL_memset(m_Keyboard_Previous_State, 0, 512 * sizeof(Uint8));
	//SDL_memset(m_joystick_current_state, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof(Uint8));
	//SDL_memset(m_joystick_previous_state, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof(Uint8));
	//SDL_JoystickEventState(SDL_ENABLE);
	//m_Mouse_Current_State = 0;
	//m_Mouse_Previous_State = 0;
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
	//m_wheel_y = 0;
	//while (SDL_PollEvent(&m_Event))
	//{
	//	//Update_ImGui_Event(m_Event);
	//	//SDL_JoystickUpdate();
	//	switch (m_Event.type)
	//	{
	//	case SDL_QUIT:
	//		quit = true;
	//		break;
	//	case SDL_JOYDEVICEADDED:
	//	case SDL_JOYDEVICEREMOVED:
	//		Update_Joystick();
	//		break;
	//	case SDL_MOUSEWHEEL:
	//		m_wheel_y = m_Event.wheel.y;
	//		break;
	//	case SDL_WINDOWEVENT:
	//		switch (m_Event.window.event)
	//		{
	//		case SDL_WINDOWEVENT_HIDDEN:
	//		case SDL_WINDOWEVENT_FOCUS_LOST:
	//		case SDL_WINDOWEVENT_MINIMIZED:
	//		{
	//			auto event_manager = World::Get()->Get_Event_Manager();
	//			event_manager->Queue_Event<EventWindowFocusLost>(false, true);
	//			break;
	//		}
	//		default:
	//			break;
	//		}
	//		break;
	//	default:
	//		break;
	//	}
	//}
	///*VERY PLACE HOLDER CODE*/
	//if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
	//{
	//	SDL_memset(m_Keyboard_Current_State, 0, 512 * sizeof(Uint8));
	//	SDL_memset(m_Keyboard_Previous_State, 0, 512 * sizeof(Uint8));
	//	m_Mouse_Current_State = 0;
	//	m_Mouse_Previous_State = 0;
	//	m_wheel_y = 0.f;
	//	return;
	//}
	//int numberOffFetchedKeys = 0;

	//const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOffFetchedKeys);
	//if (numberOffFetchedKeys > 512)
	//	numberOffFetchedKeys = 512;
	//SDL_memcpy(m_Keyboard_Previous_State, m_Keyboard_Current_State, 512 * sizeof(Uint8));
	//SDL_memcpy(m_Keyboard_Current_State, pCurrentKeyStates, numberOffFetchedKeys * sizeof(Uint8));

	////m_Mouse_Previous_State = m_Mouse_Current_State;
	////m_Mouse_Previous_Position[0] = m_Mouse_Current_Position[0]; m_Mouse_Previous_Position[1] = m_Mouse_Current_Position[1];
	////m_Mouse_Current_State = SDL_GetMouseState(m_Mouse_Current_Position, m_Mouse_Current_Position + 1);

	//Update_Joystick_Button_State();
}