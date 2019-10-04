#pragma once

#include "Events/Event.h"

class KeyEvent : public Event<KeyEvent>
{
public:
	KeyEvent(SDL_Scancode key_scancode, bool press) :
		m_scancode(key_scancode),
		m_press(press)
	{
		DEBUG_LOG("Key: %s\n", SDL_GetKeyName(SDL_GetKeyFromScancode(m_scancode)));
	}
	virtual ~KeyEvent() { }

	SDL_Scancode m_scancode;
	bool m_press;
};