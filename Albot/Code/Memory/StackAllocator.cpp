#include "StackAllocator.h"

namespace CantMemory
{
	char* StackAllocator::m_mem0 = new char[1048576];;
	Marker StackAllocator::m_marker = 0;

	StackAllocator::StackAllocator(uint32_t stackSize_bytes) {	}

	void* StackAllocator::Allocate(uint32_t size_bytes)
	{
		Marker temp = m_marker;
		m_marker += size_bytes;
		return m_mem0 + temp;
	}

	Marker StackAllocator::GetMarker()
	{
		return m_marker;
	}

	void StackAllocator::FreeToMarker(Marker marker)
	{
		m_marker = marker;
	}

	void StackAllocator::Clear()
	{
		delete m_mem0;
	}
}

