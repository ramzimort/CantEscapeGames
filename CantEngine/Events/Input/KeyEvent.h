#pragma once

#include "Events/Event.h"
#include "Events/Multicast.h"

/**
 * @brief Keyboard event
 * 
 */
class KeyEvent : public Event<KeyEvent>
{
public:
	/**
	 * @brief Keyboard event
	 * 
	 * @param key_scancode 
	 * @param press 
	 */
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
	/**
	 * @brief Key Scancode
	 * 
	 */
	SDL_Scancode m_scancode;
	/**
	 * @brief Key Current State
	 * 
	 */
	bool m_press;
};