/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#include "Camera.h"


RTTR_REGISTRATION
{
	rttr::registration::enumeration<CameraProjectionType>("CameraProjectionType")(
		rttr::value("Projection_Perspective", CameraProjectionType::PROJECTION_PERSPECTIVE),
		rttr::value("Projection_Orthographic", CameraProjectionType::PROJECTION_ORTHOGRAPHIC),
		rttr::value("Projection_Orthographic_Viewport", CameraProjectionType::PROJECTION_ORTHOGRAPHIC_VIEWPORT)
		);

	rttr::registration::enumeration<CameraRenderObjectType>("CameraRenderObjectType")(
		rttr::value("Render_Meshes", CameraRenderObjectType::CAMERA_RENDER_MESHES),
		rttr::value("Render_UI", CameraRenderObjectType::CAMERA_RENDER_UI),
		rttr::value("Render_UI_And_Meshes", CameraRenderObjectType::CAMERA_RENDER_UI_AND_MESHES)
		);

	rttr::registration::class_<Camera>("Camera")
		.property("Near", &Camera::m_near)
		.property("Far", &Camera::m_far)
		.property("FOV", &Camera::m_fov)
		.property("Position", &Camera::m_position)
		.property("CameraName", &Camera::m_cameraName)
		.property("CameraProjectionType", &Camera::m_cameraProjectionType)
		.property("OrthographicWidth", &Camera::m_orthographicWidth)
		.property("OrthographicHeight", &Camera::m_orthographicHeight)
		.property("RenderObjectType", &Camera::m_cameraRenderObjectType)
		.property("LookDir", &Camera::m_lookDir)
		.property("ZOrder", &Camera::m_zOrder)
		.property("ViewportRenderInfo", &Camera::m_viewportRenderInformation)
	;
}

Camera::Camera() :
	m_id(ID()),
	m_near(0.f),
	m_far(0.f),
	m_fov(0.f),
	m_position(0.f, 0.f, 0.f),
	m_lookDir(0.f, 0.f, -1.f),
	m_upDir(0.f, 1.f, 0.f),
	m_rightDir(1.f, 0.f, 0.f),
	m_cameraName(""),
	m_zOrder(-1),
	m_viewportRenderInformation(0.f, 0.f, 1.f, 1.f),
	m_cameraProjectionType(PROJECTION_PERSPECTIVE),
	m_orthographicWidth(100),
	m_orthographicHeight(100),
	m_cameraRenderObjectType(CAMERA_RENDER_MESHES)
{
}

Camera::Camera(float fov,
	float nearVal, float farVal, const Vector3& position) : 
	m_id(ID()),
	m_near(nearVal), 
	m_far(farVal), 
	m_fov(fov), 
	m_position(position), 
	m_lookDir(0.f, 0.f, -1.f),
	m_upDir(0.f, 1.f, 0.f),
	m_rightDir(1.f, 0.f, 0.f),
	m_cameraName(""),
	m_zOrder(-1),
	m_viewportRenderInformation(0.f, 0.f, 1.f, 1.f),
	m_cameraProjectionType(PROJECTION_PERSPECTIVE),
	m_orthographicWidth(100),
	m_orthographicHeight(100),
	m_cameraRenderObjectType(CAMERA_RENDER_MESHES)
{ 
}

Camera::Camera(const Camera& lhs) :
	m_id(ID()),
	m_near(lhs.m_near),
	m_far(lhs.m_far),
	m_fov(lhs.m_fov),
	m_position(lhs.m_position),
	m_lookDir(lhs.m_lookDir),
	m_upDir(lhs.m_upDir),
	m_rightDir(lhs.m_rightDir),
	m_cameraName(lhs.m_cameraName),
	m_zOrder(lhs.m_zOrder),
	m_viewportRenderInformation(lhs.m_viewportRenderInformation),
	m_cameraProjectionType(lhs.m_cameraProjectionType),
	m_orthographicWidth(lhs.m_orthographicWidth),
	m_orthographicHeight(lhs.m_orthographicHeight),
	m_cameraRenderObjectType(lhs.m_cameraRenderObjectType)
{ 
}

Camera::~Camera()
{ }

size_t Camera::GetId()
{
	return m_id;
}

void Camera::SetFOV(float fov)
{
	m_fov = fov;
	CalibrateFOV();
}

void Camera::IncreaseFOV(float fov)
{
	m_fov += fov;
	CalibrateFOV();
}

void Camera::DecreaseFOV(float fov)
{
	m_fov -= fov;
	CalibrateFOV();
}

void Camera::SetLook(Vector3 look)
{
	this->m_lookDir = look;
}

float Camera::GetFOV() const
{
	return m_fov;
}

void Camera::CalibrateFOV()
{
	if (m_fov > 180.f)
	{
		m_fov = 180.f;
	}
	if (m_fov < 1.f)
	{
		m_fov = 1.f;
	}
}

void Camera::SetAspectRatio(size_t width, size_t height)
{
	m_screenWidth = width;
	m_screenHeight = height;
	m_aspect = static_cast<float>(width) / static_cast<float>(height);
}

uint32_t Camera::GetCameraRenderObjectType() const
{
	return m_cameraRenderObjectType;
}

void Camera::SetCameraRenderObjectType(uint32_t renderObjectType)
{
	m_cameraRenderObjectType = (CameraRenderObjectType)renderObjectType;
}

void Camera::SetProjectionType(CameraProjectionType projectionType)
{
	m_cameraProjectionType = projectionType;
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

void  Camera::SetCameraRotation(const Matrix& transformation_mat)
{
	using namespace DirectX;

	m_lookDir = transformation_mat.Forward();
	m_upDir = transformation_mat.Up();
	m_rightDir = transformation_mat.Right();
}

Vector3 Camera::GetCameraPosition() const
{
	return m_position;
}

const Vector3& Camera::GetForward() const
{
	return m_lookDir;
}

const Vector3& Camera::GetRight() const
{
	return m_rightDir;
}

const Vector3& Camera::GetUp() const
{
	return m_upDir;
}

 Vector2 Camera::GetWidthHeight() const
{
	Vector2 value = Vector2(float(m_screenWidth), float(m_screenHeight));
	return value;
}


void Camera::Reset()
{
	//TODO:
}

void Camera::InitFromComponent()
{
	assert(m_lookDir != Vector3(0.f, 0.f, 0.f));
	m_lookDir.Normalize();
	m_rightDir = m_lookDir.Cross(Vector3(0.f, 1.f, 0.f));
	m_rightDir.Normalize();
	m_upDir = m_rightDir.Cross(m_lookDir);
	m_upDir.Normalize();
}

void Camera::UpdateViewMatrix()
{
	Vector3 focus_point = m_position + m_lookDir;
	m_viewMatrix = DirectX::XMMatrixLookAtRH(m_position, focus_point, m_upDir);
	m_invViewMatrix = DirectX::XMMatrixInverse(nullptr, m_viewMatrix);
}

void Camera::UpdateProjectionMatrix()
{
	if (m_cameraProjectionType == PROJECTION_ORTHOGRAPHIC_VIEWPORT)
	{
		float finalWidth = ( (float)m_screenWidth * m_viewportRenderInformation.z);
		float finalHeight =  ((float)m_screenHeight * m_viewportRenderInformation.w);

		m_projectionMatrix = DirectX::XMMatrixOrthographicOffCenterRH(0.f, finalWidth, finalHeight, 0.f, m_near, m_far);
		//m_projectionMatrix = DirectX::XMMatrixOrthographicRH(finalWidth, finalHeight, m_near, m_far);
	}
	else if (m_cameraProjectionType == PROJECTION_ORTHOGRAPHIC)
	{
		m_projectionMatrix = DirectX::XMMatrixOrthographicRH((float)m_orthographicWidth, (float)m_orthographicHeight, m_near, m_far);
	}
	else if (m_cameraProjectionType == PROJECTION_PERSPECTIVE)
	{
		m_projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(
			DirectX::XMConvertToRadians(m_fov), m_aspect, m_near, m_far);
	}
	m_invProjectionMatrix =	DirectX::XMMatrixInverse(nullptr, m_projectionMatrix);
}

void Camera::UpdateViewProjectionMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	m_invViewProjectionMatrix = DirectX::XMMatrixInverse(nullptr, m_viewProjectionMatrix);
}

std::string Camera::GetCameraName() const
{
	return m_cameraName;
}

int32_t Camera::GetZOrder() const
{
	return m_zOrder;
}

const Vector4& Camera::GetViewportRenderInformation() const
{
	return m_viewportRenderInformation;
}

size_t Camera::GetScreenWidth() const
{
	return m_screenWidth;
}
size_t Camera::GetScreenHeight() const
{
	return m_screenHeight;
}


void Camera::SetOrthographicWidth(size_t width)
{
	m_orthographicWidth = width;
}

void Camera::SetOrthographicHeight(size_t height)
{
	m_orthographicHeight = height;
}

size_t Camera::GetOrthographicWidth() const
{
	return m_orthographicWidth;
}
size_t Camera::GetOrthographicHeight() const
{
	return m_orthographicHeight;
}