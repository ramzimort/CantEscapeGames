#pragma once

class Editor
{
public:
	Editor();
	~Editor();

	void Update();

private:
	std::map<std::string, std::vector<std::string>> m_memoryTable;
};