/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Ramzi Mourtada
- End Header --------------------------------------------------------*/

#pragma once
class FrameManager
{
public:
	FrameManager(unsigned int maxFrameRate = 144);
	~FrameManager();

	void StartFrame();
	void EndFrame();
	unsigned int GetFrameTime();
	void SetFrameRate(unsigned int maxFrameRate);

private:
	unsigned int m_tick_start;
	unsigned int m_tick_end;
	unsigned int m_ticks_per_frame;
	unsigned int m_current_frame_time;
};

