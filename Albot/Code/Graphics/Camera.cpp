/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#include "Camera.h"

//Macros for std constructor
#define STD_NEAR		0.01f
#define STD_FAR			1000.0f
#define STD_FOV			75
#define STD_WIDTH		1280
#define STD_HEIGHT		720

RTTR_REGISTRATION
{
	rttr::registration::class_<Camera>("Camera")
		.property("Width", &Camera::m_width)
		.property("Height", &Camera::m_height)
		.property("Width", &Camera::m_near)
		.property("Width", &Camera::m_far)
		.property("Width", &Camera::m_fov)
		.property("Width", &Camera::m_position);
}

Camera::Camera(int width, int height, float fov,
	float nearVal, float farVal, const Vector3& position) : m_width(width),
	m_height(height), m_near(nearVal), m_far(farVal), m_fov(fov), 
	m_position(position), 
	m_lookDir(0.f, 0.f, -1.f),
	m_upDir(0.f, 1.f, 0.f),
	m_rightDir(1.f, 0.f, 0.f)
{
	m_aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
}

Camera::~Camera()
{
}


void Camera::SetCameraPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void Camera::SetCameraPosition(const Vector3& new_position)
{
	m_position = new_position;
}

Vector3 Camera::GetCameraPosition() const
{
	return m_position;
}

void Camera::Reset()
{
	//TODO:
}

void Camera::ApplyRotation(const Matrix& transformation_mat)
{
	using namespace DirectX;

	m_lookDir = XMVector3Normalize(XMVector3TransformNormal(m_lookDir, transformation_mat));

	m_upDir = XMVector3Normalize(XMVector3TransformNormal(m_upDir, transformation_mat));

	m_rightDir = XMVector3Cross(m_lookDir, m_upDir);
	m_upDir = XMVector3Cross(m_rightDir, m_lookDir);
}

void Camera::update_view_matrix()
{
	Vector3 focus_point = m_position + m_lookDir;
	m_viewMatrix = DirectX::XMMatrixLookAtRH(m_position, focus_point, m_upDir);
	m_invViewMatrix = DirectX::XMMatrixInverse(nullptr, m_viewMatrix);
}

void Camera::update_projection_matrix()
{
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(
		DirectX::XMConvertToRadians(m_fov), m_aspect, m_near, m_far);
	m_invProjectionMatrix =	DirectX::XMMatrixInverse(nullptr, m_projectionMatrix);
}

void Camera::update_view_projection_matrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	m_invViewProjectionMatrix = DirectX::XMMatrixInverse(nullptr, m_viewProjectionMatrix);
}