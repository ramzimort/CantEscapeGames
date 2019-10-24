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