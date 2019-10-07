
#include "DataQueue.h"

DataQueue::DataQueue()
{
}


DataQueue::~DataQueue()
{
	while (!m_printQ.empty())
		m_printQ.pop();
}

void DataQueue::Push(const char* data)
{
	m_printQ.push(data);
}

std::string DataQueue::Pop()
{
	std::string data = m_printQ.front();
	m_printQ.pop();
	return data;
}

bool DataQueue::Empty()
{
	return m_printQ.empty();
}
