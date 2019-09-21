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


///////////////////////////////////////////////////////
////           INITIALIZATION METHODS              ////
///////////////////////////////////////////////////////
void Transform::Init()
{
}


void Transform::Begin()
{
}


///////////////////////////////////////////////////////
////           TRANSLATE METHODS                   ////
///////////////////////////////////////////////////////
void Transform::Translate(Vector3 const& translate)
{
	Translate(translate.x, translate.y, translate.z);
}


void Transform::Translate(float x, float y, float z)
{
	m_needToRecalculateModel = 1;

	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}


///////////////////////////////////////////////////////
////           ROTATE METHODS                      ////
///////////////////////////////////////////////////////
void Transform::Rotate(float angle_deg, Vector3 const& axis)
{
	m_needToRecalculateModel = 1;

	// TODO - not implemented yet
}


///////////////////////////////////////////////////////
////           SCALE METHODS                       ////
///////////////////////////////////////////////////////
void Transform::Scale(float val)
{
	Scale(val, val, val);
}


void Transform::Scale(Vector3 const& scale)
{
	Scale(scale.x, scale.y, scale.z);
}


void Transform::Scale(float x, float y, float z)
{
	m_needToRecalculateModel = 1;

	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}


///////////////////////////////////////////////////////
////           GETTERS METHODS                     ////
///////////////////////////////////////////////////////
Vector3 const& Transform::GetPosition() const
{
	return m_position;
}

Vector3 const& Transform::GetRotation() const
{
	return m_rotation;
}

Vector3 const& Transform::GetScale() const
{
	return m_scale;
}

Matrix const& Transform::GetModel() const
{
	return m_model;
}

void Transform::SetModel(Matrix const& model) 
{
	this->m_model = model;
}


///////////////////////////////////////////////////////
////           DIRTY FLAG STUFF                    ////
///////////////////////////////////////////////////////
bool Transform::NeedToRecalculate()
{
	return m_needToRecalculateModel;
}

void Transform::ResetDirtyFlag()
{
	m_needToRecalculateModel = false;
}