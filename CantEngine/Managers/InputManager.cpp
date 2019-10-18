/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Ramzi Mourtada
Other Authors : <None>
-End Header--------------------------------------------------------*/

#include "InputManager.h"
#include "CantDebug/CantDebug.h"
#include "Events/Input/Input.h"
#include "EventManager.h"

const int numKeys = 512;

InputManager::InputManager() : 
	m_keyboardState({ false })
{
	int error = 0;
	error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	assert(error >= 0);

	m_pWindow = SDL_CreateWindow("CantEscapeGames",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280,
		720,
		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS);
	EventManager::Get()->EnqueueEvent<WindowSizeEvent>(true, 1280, 720);

	assert(m_pWindow != NULL);

	SDL_JoystickEventState(SDL_ENABLE);
	m_mouseStateCurrent = 0;
	m_mouseStatePrevious = 0;
	m_mouseWheelY = 0;
	m_quit = false;
}

InputManager::~InputManager()
{
}

void InputManager::UpdateMouseState()
{
	m_mouseStatePrevious = m_mouseStateCurrent;
	m_mousePositionPrevious[0] = m_mousePositionCurrent[0]; m_mousePositionPrevious[1] = m_mousePositionCurrent[1];
	m_mouseStateCurrent = SDL_GetMouseState(m_mousePositionCurrent, m_mousePositionCurrent + 1);
}


void InputManager::Update()
{
	m_mouseWheelY = 0;
	while (SDL_PollEvent(&m_event))
	{
		DEBUG_PROCESSIO(m_event, m_quit);
		switch (m_event.type)
		{

		// ALL WINDOW EVENTS
		case SDL_WINDOWEVENT:
		{
			if (m_event.window.windowID != SDL_GetWindowID(m_pWindow))
				break;
			switch (m_event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				// TODO: CALL RESIZE EVENT HERE WITH WIDTH AND HEIGHT AND ANY OTHER PARAMS 
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_RESTORED:
				EventManager::Get()->EnqueueEvent<WindowFocusEvent>(true, true);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				EventManager::Get()->EnqueueEvent<WindowFocusEvent>(true, false);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_quit = true;
				break;
			default:
				break;
			}
			break;
		}
		
		case SDL_JOYDEVICEADDED:
			EventManager::Get()->EnqueueEvent<JoystickEvent>(false, m_event.jdevice.which, true);
			break;
		case SDL_JOYDEVICEREMOVED:
			EventManager::Get()->EnqueueEvent<JoystickEvent>(false, m_event.jdevice.which, false);
			break;
		case SDL_MOUSEWHEEL:
			break;
		case SDL_MOUSEMOTION:
			EventManager::Get()->EnqueueEvent<MouseMotionEvent>(true, GetPointerLocVec2(), GetPointerDeltaVec2());
			break;
		case SDL_MOUSEBUTTONDOWN:
			if ((m_mouseStateCurrent & SDL_BUTTON(m_event.button.button)) == 0)
				EventManager::Get()->EnqueueEvent<MouseClickEvent>(true, m_event.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			if((m_mouseStateCurrent & SDL_BUTTON(m_event.button.button)) > 0)
				EventManager::Get()->EnqueueEvent<MouseClickEvent>(true, m_event.button.button, false);
			break;
		case SDL_KEYDOWN:
			if (m_event.key.windowID == SDL_GetWindowID(m_pWindow) &&
				m_event.key.keysym.scancode < 512 && 
				m_keyboardState[m_event.key.keysym.scancode] == false)
			{
				m_keyboardState[m_event.key.keysym.scancode] = true;
				EventManager::Get()->EnqueueEvent<KeyEvent>(true, m_event.key.keysym.scancode, m_event.key.state);
			}
			break;
		case SDL_KEYUP:
			if (m_event.key.windowID == SDL_GetWindowID(m_pWindow) &&
				m_event.key.keysym.scancode < 512 &&
				m_keyboardState[m_event.key.keysym.scancode] == true)
			{
				m_keyboardState[m_event.key.keysym.scancode] = false;
				EventManager::Get()->EnqueueEvent<KeyEvent>(true, m_event.key.keysym.scancode, m_event.key.state);
			}
			break;
		default:
			break;
		}
	}

	UpdateMouseState();
	// DEBUG
	DEBUG_TRACE("Pointer: x = %f, y = %f", GetPointerLocVec2().x, GetPointerLocVec2().y);

}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode) 
{
	if (KeyScanCode >= 512)
		return false;

	return m_keyboardState[KeyScanCode];
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

void InputManager::SetWindowSize(size_t w, size_t h)
{
	SDL_SetWindowSize(m_pWindow, static_cast<int>(w), static_cast<int>(h));
}


SDL_Window* InputManager::GetWindow()
{
	return m_pWindow;
}


bool InputManager::IsQuit()
{
	return m_quit;
}