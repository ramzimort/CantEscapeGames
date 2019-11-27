/**
 * @file QueryResults.cpp
 * @author Aleksey Perfilev
 * @date 12/4/2019
 * @copyright Copyright(C) 2019 DigiPen Institute of Technology
 */

#include "QueryResults.h"

///////////////////////////// QueryResult ////////////////////////////////////
QueryResult::QueryResult() : m_clientData0(nullptr), m_clientData1(nullptr)
{
}

QueryResult::QueryResult(void* clientData0, void* clientData1) : m_clientData0(clientData0), m_clientData1(clientData1)
{
}

bool QueryResult::operator==(const QueryResult& rhs) const
{
	return ((m_clientData0 == rhs.m_clientData0 && m_clientData1 == rhs.m_clientData1) ||
		(m_clientData0 == rhs.m_clientData1 && m_clientData1 == rhs.m_clientData0));
}

///////////////////////////// QueryResults ////////////////////////////////////
void QueryResults::AddResult(const QueryResult& result)
{
	m_results.push_back(result);
}

void QueryResults::DeleteDuplicates()
{
	auto end = m_results.end();

	for (auto it = m_results.begin(); it != end; ++it)
	{
		end = std::remove(it + 1, end, *it);
	}

	m_results.erase(end, m_results.end());
}
