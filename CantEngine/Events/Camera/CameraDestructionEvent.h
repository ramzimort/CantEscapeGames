#pragma once

#include "Events/Event.h"

class Camera;

class CameraDestructionEvent : public Event<CameraDestructionEvent>
{
public:
	CameraDestructionEvent(size_t id) :
		m_id(id)
	{
		DEBUG_LOG("Removing Camera: %d\n", id);
	}
	virtual ~CameraDestructionEvent() { }

	size_t m_id;
};