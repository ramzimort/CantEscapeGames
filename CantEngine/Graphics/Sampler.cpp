#include "Sampler.h"


Sampler::Sampler(const SamplerDesc& sampler_desc)
	:m_desc(sampler_desc),
	m_sampler_state(nullptr)
{
}


Sampler::~Sampler()
{
}


void Sampler::Release()
{
	SafeRelease(m_sampler_state);
}