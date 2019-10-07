#pragma once

#include "..\stdafx.h"

class SliderFloatQueue
{
public:
	struct SliderFloatData
	{
		std::string m_sliderFloatName;
		float* m_pData;
		float m_min;
		float m_max;
	};

	SliderFloatQueue();
	~SliderFloatQueue();


	std::queue<SliderFloatData> m_queueData;
};

