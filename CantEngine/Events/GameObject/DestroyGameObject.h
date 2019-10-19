#pragma once

#include "Events/Event.h"
#include "GameObjects/GameObject.h"

class DestroyGameObject : public Event<DestroyGameObject>
{
public:
	DestroyGameObject(size_t id) :
		m_ID(id) { }
	virtual ~DestroyGameObject() { }

	size_t m_ID;
};