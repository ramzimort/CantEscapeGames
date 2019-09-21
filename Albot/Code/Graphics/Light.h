#pragma once


typedef enum ELightType
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
	ELIGHT_TYPE_TOTAL_COUNT
}ELightType;

class Light{
public:
	friend class AppRenderer;
	friend class DeferredRendering;
public:
	Light();
	~Light();


	const Vector3& get_color() const;
	float get_intensity() const;
	ELightType get_light_type() const;
	float get_radius() const;
	Vector2 get_attenuation_constant() const;
private:
	ELightType m_light_type;
	Vector3 m_color;
	float m_intensity;
	float m_radius;
	bool m_enabled;
	bool m_cast_shadow;

	
};

