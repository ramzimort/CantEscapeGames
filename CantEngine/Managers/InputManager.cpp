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

InputManager::InputManager(bool fullscreen, int w, int h) : 
	m_keyboardState({ false })
{
	int error = 0;
	error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	assert(error >= 0);

	Uint32 flags = fullscreen ?
		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP :
		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_RESIZABLE;

	m_pWindow = SDL_CreateWindow("CantEscapeGames",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			w,
			h,
			flags);

	assert(m_pWindow != NULL);

	SDL_JoystickEventState(SDL_ENABLE);
	m_mouseStateCurrent = 0;
	m_mouseStatePrevious = 0;
	m_mouseWheelY = 0;
	m_quit = false;

	EventManager::Get()->SubscribeEvent<GameWindowSizeEvent>(this, std::bind(&InputManager::OnWindowResizeRequest, this, std::placeholders::_1));
}

void InputManager::OnWindowResizeRequest(const GameWindowSizeEvent* e)
{
	SetWindowSize(e->m_width, e->m_height);
}

InputManager::~InputManager()
{
}

void InputManager::UpdateMouseClickState()
{
	m_mouseStatePrevious = m_mouseStateCurrent;
	m_mouseStateCurrent = SDL_GetMouseState(NULL, NULL);
}


void InputManager::Update()
{
	using namespace std::chrono_literals;
	m_mouseWheelY = 0;
	while (SDL_WaitEvent(&m_event) && !m_quit)
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
				EventManager::Get()->EnqueueEvent<WindowFocusEvent>(false, true);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_FOCUS_LOST:
			case SDL_WINDOWEVENT_MINIMIZED:
				EventManager::Get()->EnqueueEvent<WindowFocusEvent>(false, false);
				break;
			case SDL_WINDOWEVENT_CLOSE:
				EventManager::Get()->EnqueueEvent<QuitEvent>(false);
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
			EventManager::Get()->EnqueueEvent<MouseScrollEvent>(false, m_event.wheel.x, m_event.wheel.y);
			break;
		case SDL_MOUSEMOTION:
		{
			m_mousePositionPrevious[0] = m_mousePositionCurrent[0]; m_mousePositionPrevious[1] = m_mousePositionCurrent[1];
			m_mousePositionCurrent[0] = m_event.button.x; m_mousePositionCurrent[1] = m_event.button.y;
			Vector2 Loc = GetPointerLocVec2();
			Vector2 Delta = GetPointerDeltaVec2();
			EventManager::Get()->EnqueueEvent<MouseMotionEvent>(false, Loc, Delta);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
			if ((m_mouseStateCurrent & SDL_BUTTON(m_event.button.button)) == 0)
			{
				EventManager::Get()->EnqueueEvent<MouseClickEvent>(false, m_event.button.button, true);
				UpdateMouseClickState();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if ((m_mouseStateCurrent & SDL_BUTTON(m_event.button.button)) > 0)
			{
				EventManager::Get()->EnqueueEvent<MouseClickEvent>(false, m_event.button.button, false);
				UpdateMouseClickState();
			}
			break;
		case SDL_KEYDOWN:
			if (m_event.key.windowID == SDL_GetWindowID(m_pWindow) &&
				m_event.key.keysym.scancode < 512 &&
				m_keyboardState[m_event.key.keysym.scancode] == false)
			{
				m_keyboardState[m_event.key.keysym.scancode] = true;
				EventManager::Get()->EnqueueEvent<KeyEvent>(false, m_event.key.keysym.scancode, m_event.key.state);
				if (m_event.key.keysym.scancode == SDL_SCANCODE_F11)
					ToggleFullscreenMode();
			}
			break;
		case SDL_KEYUP:
			if (m_event.key.windowID == SDL_GetWindowID(m_pWindow) &&
				m_event.key.keysym.scancode < 512 &&
				m_keyboardState[m_event.key.keysym.scancode] == true)
			{
				m_keyboardState[m_event.key.keysym.scancode] = false;
				EventManager::Get()->EnqueueEvent<KeyEvent>(false, m_event.key.keysym.scancode, m_event.key.state);
			}
			break;
		default:
			break;
		}
	}
}

bool InputManager::IsKeyPressed(unsigned int KeyScanCode) const
{
	if (KeyScanCode >= 512)
		return false;

	return m_keyboardState[KeyScanCode];
}

const int* InputManager::GetPointerLocation() const
{
	return m_mousePositionCurrent;
}

Vector2 InputManager::GetPointerLocVec2() const
{
	return Vector2( static_cast<float>(m_mousePositionCurrent[0]), 
					static_cast<float>(m_mousePositionCurrent[1]));
}

Vector2 InputManager::GetPointerDeltaVec2() const
{
	int deltaX = m_mousePositionCurrent[0] - m_mousePositionPrevious[0];
	int deltaY = m_mousePositionCurrent[1] - m_mousePositionPrevious[1];

	return Vector2(static_cast<float>(deltaX), static_cast<float>(deltaY));
}

bool InputManager::IsMousePressed(unsigned int MouseScanCode)  const
{
	return (m_mouseStateCurrent & SDL_BUTTON(MouseScanCode));
}

bool InputManager::IsMouseTriggered(unsigned int MouseScanCode)  const
{
	return (m_mouseStateCurrent & ~m_mouseStatePrevious & SDL_BUTTON(MouseScanCode));
}

bool InputManager::IsMouseReleased(unsigned int MouseScanCode)  const
{
	return (m_mouseStatePrevious & ~m_mouseStateCurrent & SDL_BUTTON(MouseScanCode));
}

Sint32 InputManager::GetMouseScroll()  const
{
	return m_mouseWheelY;
}


void InputManager::ToggleFullscreenMode()
{
	uint32_t flag = SDL_GetWindowFlags(m_pWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;
	flag ? SDL_SetWindowFullscreen(m_pWindow, 0) : SDL_SetWindowFullscreen(m_pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	
	int w, h;
	SDL_GetWindowSize(m_pWindow, &w, &h);
	SetWindowSize(w, h);
}

void InputManager::SetWindowSize(int w, int h)
{
	int prevWidth, prevHeight;
	SDL_GetWindowSize(m_pWindow, &prevWidth, &prevHeight);
	float xScale = (float)w / (float)prevWidth;
	float yScale = (float)h / (float)prevHeight;
	SDL_SetWindowSize(m_pWindow, w, h);
	EventManager::Get()->EnqueueEvent<WindowSizeEvent>(false, w, h, xScale, yScale);
}


SDL_Window* InputManager::GetWindow()
{
	return m_pWindow;
}


bool InputManager::IsQuit()  const
{
	return m_quit;
}

void InputManager::Quit()
{
	m_quit = true;
}