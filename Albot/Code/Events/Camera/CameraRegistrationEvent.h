
#pragma once

#include "Events/Event.h"

class Camera;

class CameraRegistrationEvent : public Event<CameraRegistrationEvent>
{
public:
	CameraRegistrationEvent(Camera& rCamera, bool state) :
		m_camera(rCamera),
		m_state(state)
	{
		DEBUG_LOG("Registering Camera: %p", &rCamera);
	}
	virtual ~CameraRegistrationEvent() { }

	Camera& m_camera;
	bool m_state;
};