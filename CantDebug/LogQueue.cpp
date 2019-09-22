#include "stdafx.h"
#include "LogQueue.h"
#include <string>


LogQueue::LogQueue()
{
}


LogQueue::~LogQueue()
{
	while (!m_printQ.empty())
		m_printQ.pop();
}

void LogQueue::Push(const char* data)
{
	m_printQ.push(data);
}

std::string LogQueue::Pop()
{
	std::string data = m_printQ.front();
	m_printQ.pop();
	return data;
}

bool LogQueue::Empty()
{
	return m_printQ.empty();
}
