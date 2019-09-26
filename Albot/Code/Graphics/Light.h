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

	const Vector3& GetColor() const;
	float GetIntensity() const;
	ELightType get_light_type() const;
	float get_radius() const;
	Vector2 get_attenuation_constant() const;
	float GetShadowIntensity() const { return m_shadow_intensity; };
private:
	ELightType m_light_type;
	Vector3 m_color;
	float m_intensity;
	float m_radius;
	float m_shadow_intensity;
	bool m_enabled;
	bool m_cast_shadow;

	RTTR_ENABLE();
	RTTR_REGISTRATION_FRIEND;
};

