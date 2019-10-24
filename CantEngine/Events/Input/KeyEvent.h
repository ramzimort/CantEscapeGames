#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

class KeyEvent : public Event<KeyEvent>
{
public:
	KeyEvent(SDL_Scancode key_scancode, bool press) :
		m_scancode(key_scancode),
		m_press(press)
	{
		DEBUG_LOG("Key: %s, State: %d\n", SDL_GetKeyName(SDL_GetKeyFromScancode(m_scancode)), press);
	}
	virtual ~KeyEvent() { }
	virtual void operator()() override
	{
		OnKeyEvent()((int)m_scancode, m_press);
	}

	static Multicast<void(int, bool)>& OnKeyEvent()
	{
		static Multicast<void(int, bool)> m;
		return m;
	}

	SDL_Scancode m_scancode;
	bool m_press;
};