#pragma once


typedef enum ELightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	ELIGHT_TYPE_TOTAL_COUNT
}ELightType;

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

	const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetColor() const;
	float GetIntensity() const;
	ELightType GetLightType() const;
	float GetRadius() const;
	Vector2 GetAttenuationConstant() const;
	float GetShadowIntensity() const { return m_shadow_intensity; };

	void SetPosition(const Vector3& position) { m_position = position; }
	void SetColor(const Vector3& color) { m_color = color; };
	void SetIntensity(float intensity) { m_intensity = intensity; };
	void SetRadius(float radius) { m_radius = radius; };
private:
	ELightType m_light_type;
	Vector3 m_color;
	Vector3 m_position;
	float m_intensity;
	float m_radius;
	float m_shadow_intensity;
	bool m_enabled;
	bool m_cast_shadow;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

