#include "Pipeline.h"


Pipeline::Pipeline(const PipelineDesc& desc)
	:m_desc(desc),
	m_input_layout(nullptr)
{
}


Pipeline::~Pipeline()
{
}


const PipelineDesc& Pipeline::get_desc() const
{
	return m_desc;
}

void Pipeline::Release()
{
	SafeRelease(m_input_layout);
}