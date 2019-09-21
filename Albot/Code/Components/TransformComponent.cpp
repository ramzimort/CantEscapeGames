/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#include "TransformComponent.h"
unsigned const Transform::static_type = BaseComponent::numberOfTypes++;


Transform::Transform(GameObject *owner) :
	BaseComponent(owner, Transform::static_type),
	m_needToRecalculateModel(0)
{
}


Transform::~Transform()
{
}


void Transform::Init()
{
}


void Transform::Translate(float x, float y, float z)
{
	m_needToRecalculateModel = 1;
}


void Transform::Rotate(float angle_deg/*, Vector4 const& axis*/)
{
	m_needToRecalculateModel = 1;
}


void Transform::Scale(float val)
{
	Scale(val, val, val);
}


void Transform::Scale(float x, float y, float z)
{
	m_needToRecalculateModel = 1;
}