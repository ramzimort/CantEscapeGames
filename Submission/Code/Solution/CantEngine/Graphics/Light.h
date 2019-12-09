#pragma once
/**
 * @file Light.h
 * @author Albert Harley
 * @date 12/4/2019
 * @brief Defines the 3D light illumination properties
 */

typedef enum ELightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	ELIGHT_TYPE_TOTAL_COUNT
}ELightType;

/**
 * @brief Defines the interface for light
 * 
 */
class Light
{
public:
	friend class AppRenderer;
	friend class DeferredRendering;
	friend class LightSystem;
	friend class Factory;
public:
	Light();
	~Light();
	/**
	 * @brief Get the Position of the Light relative to the GameObject's TransformComponent's position
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetPosition() const { return m_position; }
	/**
	 * @brief Get the Color of the Light
	 * 
	 * @return const Vector3& 
	 */
	const Vector3& GetColor() const;
	/**
	 * @brief Get the Intensity of the Light
	 * 
	 * @return float 
	 */
	float GetIntensity() const;
	/**
	 * @brief Get the Light Type
	 * 
	 * @return ELightType 
	 */
	ELightType GetLightType() const;
	/**
	 * @brief Get the Radius of the Light. Only applicable for Point Light
	 * 
	 * @return float 
	 */
	float GetRadius() const;
	/**
	 * @brief Get the Attenuation Value for Point Light
	 * 
	 * @return Vector2 
	 */
	Vector2 GetAttenuationConstant() const;
	/**
	 * @brief Get the Shadow Intensity of the Light
	 * 
	 * @return float 
	 */
	float GetShadowIntensity() const { return m_shadow_intensity; };

	/**
	 * @brief Set the Position of the Light
	 * 
	 * @param position 
	 */
	void SetPosition(const Vector3& position) { m_position = position; }
	/**
	 * @brief Set the Color of the Light
	 * 
	 * @param color 
	 */
	void SetColor(const Vector3& color) { m_color = color; };
	/**
	 * @brief Set the Intensity of the Light
	 * 
	 * @param intensity 
	 */
	void SetIntensity(float intensity) { m_intensity = intensity; };
	/**
	 * @brief Set the Radius of the Light
	 * 
	 * @param radius 
	 */
	void SetRadius(float radius) { m_radius = radius; };


	void SetShadowOrthographicSize(const Vector2& orthographicSize) { m_shadowOrthographicSize = orthographicSize; }
	Vector2 GetShadowOrthographicSize() const { return m_shadowOrthographicSize; }
	void SetShadowCasterDistanceFromFocusPoint(float casterDistanceFromFocusPoint) { m_shadowCasterDistanceFromFocusPoint = casterDistanceFromFocusPoint; }
	float GetShadowCasterDistanceFromFocusPoint() const { return m_shadowCasterDistanceFromFocusPoint; }
private:
	ELightType m_light_type;
	Vector3 m_color;
	Vector3 m_position;
	float m_intensity;
	float m_radius;
	float m_shadow_intensity;
	bool m_enabled;
	bool m_cast_shadow;

	//applicable only for directional light
	Vector2 m_shadowOrthographicSize;
	float m_shadowCasterDistanceFromFocusPoint;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

