#include "Light.h"



Light::Light()
	:m_light_type(ELightType::POINT_LIGHT),
	m_color(1.f, 1.f, 1.f),
	m_intensity(1.f),
	m_cast_shadow(true),
	m_enabled(true),
	m_radius(1.f),
	m_shadow_intensity(1.f)
{
}


Light::~Light()
{
}

const Vector3& Light::GetColor() const
{
	return m_color;
}

float Light::GetIntensity() const
{
	return m_intensity;
}

ELightType Light::get_light_type() const
{
	return m_light_type;
}

float Light::get_radius() const
{
	return m_radius;
}

Vector2 Light::get_attenuation_constant() const
{
	Vector2 atten_cost (1.0f / (m_radius * m_radius), 2.0f / m_radius);
	return atten_cost;
}