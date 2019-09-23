#pragma once

#include "stdafx.h"
#include "MemoryProfiler.h"
#include <list>


void MemoryProfiler::AddElement(const char* poolName, const void* memData)
{
	m_memoryTable[poolName].insert(memData);
}

void MemoryProfiler::FreeElement(const char* poolName, const void* memData)
{
	m_memoryTable[poolName].erase(memData);
}

void MemoryProfiler::FreeAllElements(const char* poolName, const void* startAddr)
{
	auto it_begin = m_memoryTable[poolName].find(startAddr);
	auto it_end = m_memoryTable[poolName].end();

	while (it_begin != it_end) 
	{
		if (*it_begin >= startAddr) 
		{
			it_begin = m_memoryTable[poolName].erase(it_begin);
		}
		else 
		{
			++it_begin;
		}
	}
}

void MemoryProfiler::Update()
{
	for (auto it = m_memoryTable.begin(); it != m_memoryTable.end(); ++it)
	{
		if (ImGui::TreeNode(it->first))
		{
			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
			{
				ImGui::Text("address: %x", *it2);
			}
			ImGui::TreePop();
		}
	}
}
