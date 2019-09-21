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

//Friend classes
public:
	friend class Factory;

//Public interface
public:
	Transform(GameObject *owner);
	virtual ~Transform();

	virtual void Init() override;

	//To modify the object
	void Translate(float x, float y, float z);
	void Rotate(float angle_deg/*, Vector4 const& axis*/);
	void Scale(float val);
	void Scale(float x, float y, float z);

public:
	//To compare when using templates
	static ComponentId const static_type;

private:
	//Flag for knowing if the object moved
	bool m_needToRecalculateModel;
};