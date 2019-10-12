/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once



class Camera
{
public:
	friend class FPSCameraSystem;
public:
	Camera();
	Camera(const Camera& lhs);

	Camera(float fov,
		float nearVal, float farVAl, const Vector3& position);
	~Camera();

	void SetAspectRatio(size_t width, size_t height);

	void SetCameraPosition(float x, float y, float z);
	void SetCameraPosition(const Vector3& new_position);

	Vector3 GetCameraPosition() const;
	void Reset();

	void ApplyRotation(const Matrix& transformation_mat);


	const Matrix& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix& GetInvViewMatrix() const { return m_invViewMatrix; }
	const Matrix& GetProjectionMatrix() const { return m_projectionMatrix; }
	const Matrix& GetInvProjectionMatrix() const { return m_invProjectionMatrix; }
	const Matrix& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	const Matrix& GetInvViewProjectionMatrix() const { return m_invViewProjectionMatrix; }
	
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	size_t GetId();

private:
	
private:
	size_t m_id;
	std::string m_cameraName;
	Vector3 m_position, m_lookDir, m_upDir, m_rightDir;

	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;

	Matrix m_invViewMatrix;
	Matrix m_invProjectionMatrix;

	Matrix m_viewProjectionMatrix;
	Matrix m_invViewProjectionMatrix;

	float m_fov;
	float m_aspect;
	float m_near;
	float m_far;

	static size_t ID()
	{
		static size_t ID = 0;
		return ID++;
	}

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

