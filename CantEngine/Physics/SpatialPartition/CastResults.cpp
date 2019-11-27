/**
 * @file CastResults.cp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */
 #include "CastResults.h"

//-----------------------------------------------------------------------------CastResult
CastResult::CastResult()
{
	m_Time = -1;
	m_ClientData = nullptr;
}

CastResult::CastResult(void* clientData)
{
	m_ClientData = clientData;
	m_Time = -1;
}

CastResult::CastResult(void* clientData, float time)
{
	m_ClientData = clientData;
	m_Time = time;
}

bool CastResult::operator<(const CastResult& rhs) const
{
	// Sort first by time and then resolve conflicts by the client data
	if (m_Time == rhs.m_Time)
		return m_ClientData < rhs.m_ClientData;
	return m_Time < rhs.m_Time;
}

//-----------------------------------------------------------------------------CastResults
void CastResults::AddResult(const CastResult& result)
{
	m_Results.push_back(result);

	// As long as the result before has a larger T value then this new item should be before it
	size_t index = m_Results.size() - 1;
	while (index > 0 && m_Results[index - 1].m_Time > result.m_Time)
	{
		m_Results[index] = m_Results[index - 1];
		--index;
	}

	m_Results[index] = result;
}