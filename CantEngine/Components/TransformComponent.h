/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

///HEADER STUFF

#pragma once

///INCLUDES
#include "BaseComponent.h"


class GameObjectManager;


class TransformComponent : public BaseComponent
{

//Friend classes and typedefs
public:
	friend class Factory;
	friend class TransformSystem;
//METHODS
public:
	//ctor and dtor
	TransformComponent(GameObject *owner);
	virtual ~TransformComponent();

	//Interface
	void Translate(float dx, float dy, float dz);
	void Translate(Vector3 const &translate);
	void Rotate(float pitch, float yaw, float roll);
	void Rotate(Vector3 const& rotation);
	void Scale(float val);
	void Scale(float x, float y, float z);
	void Scale(Vector3 const &scale);

	//getters
	Vector3 const& GetWorldPosition() const;
	Vector3 const& GetPosition() const;
	Vector3 const& GetRotation() const;
	Vector3 const& GetScale() const;
	Matrix const& GetModel() const;
	Matrix const& GetRotationMatrix() const;
	Matrix const& GetScaleMatrix() const;

	//Setters
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(const Vector3& position);
	void SetLocalRotation(float eulerX, float eulerY, float eulerZ);


private:
	//Setup methods
	virtual void Init(ResourceManager* resMgr, DXRenderer* dxrenderer) override;
	virtual void Begin(GameObjectManager *goMgr) override;
	
	//dirty flag
	void SetModel(Matrix const& model);
	bool NeedToRecalculate();
	void ResetDirtyFlag();


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
	Matrix m_rotMatrix;
	Matrix m_scaleMatrix;

	RTTR_ENABLE(BaseComponent);
	RTTR_REGISTRATION_FRIEND;
};