#pragma once
#include "Events/Event.h"

class WindowFocusEvent : public Event<WindowFocusEvent>
{
public:
	WindowFocusEvent(bool gainedFocus) :
		m_gainedFocus(gainedFocus) { }
	virtual ~WindowFocusEvent() { }
	virtual void operator()() override
	{

	}

	bool m_gainedFocus;
};

class WindowSizeEvent : public Event<WindowSizeEvent>
{
public:

	WindowSizeEvent(int width, int height, float xScaleFactor, float yScaleFactor) :
		m_width(width), m_height(height), m_xScaleFactor(xScaleFactor), m_yScaleFactor(yScaleFactor) {
		//DEBUG_LOG("w: %d, h: %d", width, height);
	}
	virtual ~WindowSizeEvent() { }

	virtual void operator()() override
	{
		OnWindowSizeEvent()(m_width, m_height);
	}

	static Multicast<void(int, int)>& OnWindowSizeEvent()
	{
		static Multicast<void(int, int)> m;
		return m;
	}

	int m_width;
	int m_height;
	
	float m_xScaleFactor, m_yScaleFactor;
};