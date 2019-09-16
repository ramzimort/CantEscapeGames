/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#include "Camera.h"
#include <iostream>

//Macros for std constructor
#define STD_NEAR		0.01f
#define STD_FAR			1000.0f
#define STD_FOV			75
#define STD_WIDTH		1280
#define STD_HEIGHT		720



Camera::Camera() : m_width(STD_WIDTH), m_height(STD_HEIGHT), 
	m_near(STD_NEAR), m_far(STD_FAR), m_fov(STD_FOV), 
	/*m_eye(), m_look(),*/ m_isOrtho(false)
{
	initCamera();
}

Camera::Camera(int width, int height, float fov, 
	float near, float far, Vector4& eye, 
	Vector4& look, bool isOrtho) : m_width(width), 
	m_height(height), m_near(near), m_far(far), m_fov(fov), 
	m_eye(eye), m_look(look), m_isOrtho(isOrtho)
{
	initCamera();
}

Camera::~Camera()
{
	//DELETE STUFF IF NEEDED
}

void Camera::initCamera()
{
	//Common initialization code
	this->m_aspect = static_cast<float>(m_width) / m_height;
}

void Camera::Update(float dt)
{
	std::cout << dt << std::endl;
}

void Camera::GetView(Matrix4& view)
{
	//CALCULATE VIEW
}

void Camera::GetProj(Matrix4& proj)
{
	if (m_isOrtho)
		GetOrthographic(proj);
	else
		GetPerspective(proj);
}

void Camera::GetPerspective(Matrix4& proj)
{
	//CALCULATE PERSP
}

void Camera::GetOrthographic(Matrix4& proj)
{
	//CALCULATE ORTHO
}
