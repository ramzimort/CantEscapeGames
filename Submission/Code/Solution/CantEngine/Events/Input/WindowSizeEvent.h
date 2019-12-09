#pragma once
#include "Events/Event.h"

/**
 * @brief Broadcasted by input manager when window focus changes
 * 
 */
class WindowFocusEvent : public Event<WindowFocusEvent>
{
public:
	/**
	 * @brief Broadcasted by input manager when window focus changes
	 * 
	 * @param gainedFocus 
	 */
	WindowFocusEvent(bool gainedFocus) :
		m_gainedFocus(gainedFocus) { }
	virtual ~WindowFocusEvent() { }
	virtual void operator()() override
	{

	}
	/**
	 * @brief Focus State
	 * 
	 */
	bool m_gainedFocus;
};

/**
 * @brief Broadcasted by input manager when window size changes
 * 
 */
class WindowSizeEvent : public Event<WindowSizeEvent>
{
public:
	/**
	 * @brief Broadcasted by input manager when window size changes
	 * 
	 * @param width 
	 * @param height 
	 * @param xScaleFactor 
	 * @param yScaleFactor 
	 */
	WindowSizeEvent(int width, int height, float xScaleFactor, float yScaleFactor) :
		m_width(width), m_height(height), m_xScaleFactor(xScaleFactor), m_yScaleFactor(yScaleFactor) {
		//DEBUG_LOG("w: %d, h: %d", width, height);
	}
	virtual ~WindowSizeEvent() { }

	virtual void operator()() override
	{
		OnWindowSizeEvent()(m_width, m_height, m_xScaleFactor, m_yScaleFactor);
	}

	static Multicast<void(int, int, float, float)>& OnWindowSizeEvent()
	{
		static Multicast<void(int, int, float, float)> m;
		return m;
	}
	/**
	 * @brief Screen Width
	 * 
	 */
	int m_width;
	/**
	 * @brief Screen Height
	 * 
	 */
	int m_height;
	/**
	 * @brief Screen Scaling Factor After Resizing in x
	 * 
	 */
	float m_xScaleFactor;
	/**
	 * @brief Screen Scaling Factor After Resizing in y
	 * 
	 */
	float m_yScaleFactor;
};

/**
 * @brief This is to send requests to the Input Manager for resizing
 * 
 */
class GameWindowSizeEvent : public Event<GameWindowSizeEvent>
{
public:
	/**
	 * @brief This is to send requests to the Input Manager for resizing
	 * 
	 * @param width 
	 * @param height 
	 */
	GameWindowSizeEvent(int width, int height) :
		m_width(width), m_height(height) {
		//DEBUG_LOG("w: %d, h: %d", width, height);
	}
	virtual ~GameWindowSizeEvent() { }

	virtual void operator()() override
	{
	}
	/**
	 * @brief Requested Width
	 * 
	 */
	int m_width;
	/**
	 * @brief Requested Height
	 * 
	 */
	int m_height;
};