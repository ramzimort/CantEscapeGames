#pragma once
#include "Events/Event.h"

/**
 * @brief Broadcasted by Input Manager when mouse moves
 * 
 */
class MouseMotionEvent : public Event<MouseMotionEvent>
{
public:
	/**
	 * @brief Broadcasted by Input Manager when mouse moves
	 * 
	 * @param pos 
	 * @param delta 
	 */
	MouseMotionEvent(const Vector2 pos, const Vector2 delta) :
		m_position(pos),
		m_deltaPosition(delta)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f\n", pos.x, pos.y, delta.x, delta.y);
	}
	virtual ~MouseMotionEvent() {}
	virtual void operator()() override
	{
		OnMouseMotion()(m_position, m_deltaPosition);
	}

	static Multicast<void(Vector2, Vector2)>& OnMouseMotion()
	{
		static Multicast<void(Vector2, Vector2)> m;
		return m;
	}

	Vector2 m_position;
	Vector2 m_deltaPosition;
};

/**
 * @brief Braodcasted by Input Manager when mouse scrolls
 * 
 */
class MouseScrollEvent : public Event<MouseScrollEvent>
{
public:
	/**
	 * @brief Braodcasted by Input Manager when mouse scrolls
	 * 
	 * @param x 
	 * @param y 
	 */
	MouseScrollEvent(int32_t x, int32_t y) :
		m_xScroll(x), m_yScroll(y)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f, State: %d\n", pos.x, pos.y, delta.x, delta.y, state);
	}
	virtual ~MouseScrollEvent() {}
	virtual void operator()() override
	{
		OnMouseScroll()(m_xScroll, m_yScroll);
	}
	static Multicast<void(int32_t, int32_t)>& OnMouseScroll()
	{
		static Multicast<void(int32_t, int32_t)> m;
		return m;
	}
	/**
	 * @brief Scroll in X (uncommon)
	 * 
	 */
	const int32_t m_xScroll;
	/**
	 * @brief Scroll in Y
	 * 
	 */
	const int32_t m_yScroll;
};

/**
 * @brief Broadcast by Input Manager when Mouse is clicked
 * 
 */
class MouseClickEvent : public Event<MouseClickEvent>
{
public:
	/**
	 * @brief Broadcast by Input Manager when Mouse is clicked
	 * 
	 * @param button 
	 * @param state 
	 */
	MouseClickEvent(Uint8 button, bool state) :
		m_button(button),
		m_state(state)
	{
		//DEBUG_LOG("Position: %f,%f; Delta: %f,%f, State: %d\n", pos.x, pos.y, delta.x, delta.y, state);
	}
	virtual ~MouseClickEvent() {}
	virtual void operator()() override
	{
		OnMouseClick()(m_button, m_state);
	}

	static Multicast<void(Uint8, bool)>& OnMouseClick()
	{
		static Multicast<void(Uint8, bool)> m;
		return m;
	}
	/**
	 * @brief Mouse Clicked Button (Check SDLBUTTON)
	 * 
	 */
	Uint8 m_button;
	/**
	 * @brief Button Current State
	 * 
	 */
	bool m_state;
};