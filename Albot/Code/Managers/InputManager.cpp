/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "InputManager.h"
#include "CantDebug/CantDebug.h"
#include "Events/Input/KeyDownEvent.h"
#include "EventManager.h"

InputManager* gpInputManager;

InputManager::InputManager()
{
	gpInputManager = this;
}

InputManager::~InputManager()
{
}

void InputManager::Initialize()
{
	int error = 0;
	error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	assert(error >= 0);

	m_pWindow = SDL_CreateWindow("CantEscapeGames",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	assert(m_pWindow != NULL);
	DEBUG_INIT;


	SDL_memset(m_keyboardStateCurrent, 0, 512 * sizeof(Uint8));
	SDL_memset(m_keyboardStatePrevious, 0, 512 * sizeof(Uint8));
	SDL_JoystickEventState(SDL_ENABLE);
	m_mouseStateCurrent = 0;
	m_mouseStatePrevious = 0;
	SDL_memset(m_mousePositionCurrent, 0, 2 * sizeof(Uint8));
	SDL_memset(m_mousePositionCurrent, 0, 2 * sizeof(Uint8));
	m_mouseWheelY = 0;
	m_quit = false;
}

void InputManager::Update()
{
	//SDL_Event sdl_event;
	//SDL_JoystickUpdate();
	m_mouseWheelY = 0;
	while (SDL_PollEvent(&m_event))
	{
		//Update_ImGui_Event(m_event);
		//SDL_JoystickUpdate();
		switch (m_event.type)
		{
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			break;
		case SDL_MOUSEWHEEL:
			m_mouseWheelY = m_event.wheel.y;
			break;
		case SDL_WINDOWEVENT:
			switch (m_event.window.event)
			{
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				break;
			case SDL_WINDOWEVENT_CLOSE:
				DEBUG_QUIT(m_event);
				if (m_event.window.windowID == SDL_GetWindowID(m_pWindow))
					m_quit = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		case SDL_KEYDOWN:
			//EventManager::Get()->EnqueueEvent<KeyDownEvent>(false, m_event.key.keysym.scancode);
			break;
		}
	}


	int numberOffFetchedKeys = 0;
	const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOffFetchedKeys);
	if (numberOffFetchedKeys > 512)
		numberOffFetchedKeys = 512;
	SDL_memcpy(m_keyboardStatePrevious, m_keyboardStateCurrent, 512 * sizeof(Uint8));
	SDL_memcpy(m_keyboardStateCurrent, pCurrentKeyStates, numberOffFetchedKeys * sizeof(Uint8));

	m_mouseStatePrevious = m_mouseStateCurrent;
	m_mousePositionPrevious[0] = m_mousePositionCurrent[0]; m_mousePositionPrevious[1] = m_mousePositionCurrent[1];
	m_mouseStateCurrent = SDL_GetMouseState(m_mousePositionCurrent, m_mousePositionCurrent + 1);

	// DEBUG
	DEBUG_TRACE("Pointer: x = %f, y = %f", GetPointerLocVec2().x, GetPointerLocVec2().y);

}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (m_keyboardStateCurrent[KeyScanCode])
		return true;

	return false;
}

bool InputManager::IsKeyTriggered(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (m_keyboardStateCurrent[KeyScanCode] && !m_keyboardStatePrevious[KeyScanCode])
		return true;

	return false;
}

bool InputManager::IsKeyReleased(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;

	if (!m_keyboardStateCurrent[KeyScanCode] && m_keyboardStatePrevious[KeyScanCode])
		return true;

	return false;
}

int* InputManager::GetPointerLocation()
{
	return m_mousePositionCurrent;
}

Vector2 InputManager::GetPointerLocVec2()
{
	return Vector2( static_cast<float>(m_mousePositionCurrent[0]), 
					static_cast<float>(m_mousePositionCurrent[1]));
}

Vector2 InputManager::GetPointerDeltaVec2()
{
	int deltaX = m_mousePositionCurrent[0] - m_mousePositionPrevious[0];
	int deltaY = m_mousePositionCurrent[1] - m_mousePositionPrevious[1];

	return Vector2(static_cast<float>(deltaX), static_cast<float>(deltaY));
}

bool InputManager::IsMousePressed(unsigned int MouseScanCode)
{
	return (m_mouseStateCurrent & SDL_BUTTON(MouseScanCode));
}

bool InputManager::IsMouseTriggered(unsigned int MouseScanCode)
{
	return (m_mouseStateCurrent & ~m_mouseStatePrevious & SDL_BUTTON(MouseScanCode));
}

bool InputManager::IsMouseReleased(unsigned int MouseScanCode)
{
	return (m_mouseStatePrevious & ~m_mouseStateCurrent & SDL_BUTTON(MouseScanCode));
}

Sint32 InputManager::GetMouseScroll()
{
	return m_mouseWheelY;
}

SDL_Window* InputManager::GetWindow()
{
	return m_pWindow;
}


bool InputManager::IsQuit()
{
	return m_quit;
}


bool InputManager::AnyKeyPressed() const
{
	for (int i = 0; i < 512; ++i)
	{
		if (m_keyboardStateCurrent[i] && !m_keyboardStatePrevious[i])
		{
			return true;
		}
	}

	return false;
}