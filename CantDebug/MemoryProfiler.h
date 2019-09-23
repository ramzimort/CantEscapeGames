#pragma once

#include <map>
#include <set>


class MemoryProfiler
{
public:
	void AddElement(const char* poolName, const void* addr);
	void FreeElement(const char* poolName, const void* addr);
	void FreeAllElements(const char* poolName, const void* startAddr);
	void Update();

private:
	typedef std::map<const char *, std::set<const void*>> MemoryTable;
	MemoryTable m_memoryTable;

};