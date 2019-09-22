/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"


class Transform : public BaseComponent
{

//Friend classes and typedefs
public:
	friend class Factory;

//METHODS
public:
	//ctor and dtor
	Transform(GameObject *owner);
	virtual ~Transform();

	//Setup methods
	virtual void Init() override;
	virtual void Begin() override;


	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(const Vector3& position);
	//Interface
	void Translate(float x, float y, float z);
	void Translate(Vector3 const &translate);
	
	void Rotate(float eulerX, float eulerY, float eulerZ);

	void Scale(float val);
	void Scale(float x, float y, float z);
	void Scale(Vector3 const &scale);

	//getters
	Vector3 const& GetPosition() const;
	Vector3 const& GetRotation() const;
	Vector3 const& GetScale() const;
	Matrix const& GetModel() const;
	void SetModel(Matrix const& model);

	//dirty flag
	bool NeedToRecalculate(); 
	void ResetDirtyFlag();

//VARIABLES
public:
	//To compare when using templates
	static ComponentId const static_type;

private:
	//Flag for knowing if the object moved
	bool m_needToRecalculateModel;

	Vector3 m_position;
	Vector3 m_rotation;
	Vector3 m_scale;

	Matrix m_model;

};