#pragma once

#include "Events/Event.h"

class Camera;

class CameraDestructionEvent : public Event<CameraDestructionEvent>
{
public:
	CameraDestructionEvent(Camera* camera, size_t id) :
		m_pCamera(camera),
		m_id(id)
	{
		DEBUG_LOG("Removing Camera: %d\n", id);
	}
	virtual ~CameraDestructionEvent() { }
	virtual void operator()() override
	{

	}
	Camera* m_pCamera;
	size_t m_id;
};