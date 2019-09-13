
#include "FrameManager.h"
#include "SDL2/SDL_timer.h"

FrameManager::FrameManager(unsigned int maxFrameRate) :
	m_tick_start(0),
	m_tick_end(0),
	m_current_frame_time(0),
	m_ticks_per_frame(0)
{
	SetFrameRate(maxFrameRate);
}

FrameManager::~FrameManager()
{
}

void FrameManager::StartFrame()
{
	m_tick_start = SDL_GetTicks();
}

void FrameManager::EndFrame()
{
	m_tick_end = SDL_GetTicks();
	while (m_tick_end - m_tick_start < m_ticks_per_frame)
		m_tick_end = SDL_GetTicks();

	m_current_frame_time = m_tick_end - m_tick_start;
}

unsigned int FrameManager::GetFrameTime()
{
	return m_current_frame_time;
}

void FrameManager::SetFrameRate(unsigned int maxFrameRate)
{
	if (maxFrameRate == 0)
		m_ticks_per_frame = 0;
	else
		m_ticks_per_frame = 1000 / maxFrameRate;
}
