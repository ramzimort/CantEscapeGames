#pragma once

#include <queue>
class LogData;

class LogQueue
{
public:
	LogQueue();
	~LogQueue();

	void Push(const char* data);
	std::string Pop();
	bool Empty();

private:
	std::queue<std::string> m_printQ;
};
