#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

class KeyEvent : public Event<KeyEvent>
{
	typedef Multicast<void(SDL_Scancode, bool)> KeyMulticast;
public:
	KeyEvent(SDL_Scancode key_scancode, bool press) :
		m_scancode(key_scancode),
		m_press(press)
	{
		DEBUG_LOG("Key: %s, State: %d\n", SDL_GetKeyName(SDL_GetKeyFromScancode(m_scancode)), press);
		OnKeyEvent()(key_scancode, press);
	}
	virtual ~KeyEvent() { }

	static KeyMulticast& OnKeyEvent() {
		static KeyMulticast val;
		return val;
	}

	SDL_Scancode m_scancode;
	bool m_press;
};