#include "stdafx.h"
#include "HaloEffect.h"


RTTR_REGISTRATION
{
	rttr::registration::class_<HaloEffect>("HaloEffect")
		.property("Position", &HaloEffect::m_position)
		.property("Radius", &HaloEffect::m_radius)
		.property("Intensity", &HaloEffect::m_intensity)
		.property("Color", &HaloEffect::m_color);
}


HaloEffect::HaloEffect()
	:m_radius(5.f),
	m_intensity(1.f),
	m_color(1.f),
	m_position(0.f, 0.f, 0.f)
{
}


HaloEffect::~HaloEffect()
{
}

void HaloEffect::SetRadius(float radius)
{
	m_radius = radius;
}

void HaloEffect::SetIntensity(float intensity)
{
	m_intensity = intensity;
}

void HaloEffect::SetColor(const Vector3& color)
{
	m_color = color;
}