#pragma once

#include <queue>

class DataQueue
{
public:
	DataQueue();
	~DataQueue();

	void Push(const char* data);
	std::string Pop();
	bool Empty();

private:
	std::queue<std::string> m_printQ;
};
