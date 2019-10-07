#pragma once

#pragma once

#include "Events/Event.h"

class WindowSizeEvent : public Event<WindowSizeEvent>
{
public:
	WindowSizeEvent(size_t width, size_t height) :
		m_width(width), m_height(height) { }
	virtual ~WindowSizeEvent() { }

	size_t m_width;
	size_t m_height;
};