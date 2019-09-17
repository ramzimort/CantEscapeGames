#include "stdafx.h"
#include "LogQueue.h"


LogQueue::LogQueue()
{
}


LogQueue::~LogQueue()
{
	while (!m_print_queue.empty())
		m_print_queue.pop();
}

void LogQueue::Push(std::string data)
{
	m_print_queue.push(data);
}

std::string LogQueue::Pop()
{
	std::string data = m_print_queue.front();
	m_print_queue.pop();
	return data;
}

bool LogQueue::Empty()
{
	return m_print_queue.empty();
}
