
#pragma once

#include "Events/Event.h"

class Camera;

class CameraRegistrationEvent : public Event<CameraRegistrationEvent>
{
public:
	CameraRegistrationEvent(Camera& rCamera, size_t xPos, size_t yPos) :
		m_camera(rCamera),
		m_xViewPortPos(xPos),
		m_yViewportPos(yPos)
	{
		DEBUG_LOG("Registering Camera: %p\n", &rCamera);
	}
	virtual ~CameraRegistrationEvent() { }

	Camera& m_camera;
	size_t m_xViewPortPos;
	size_t m_yViewportPos;
};