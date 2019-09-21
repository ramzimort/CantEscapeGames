#include "StackAllocator.h"

StackAllocator::StackAllocator(uint32_t stackSize_bytes) :
	m_mem0(new char[stackSize_bytes]),
	m_marker(0)
{ }

StackAllocator::~StackAllocator()
{
	delete m_mem0;
}

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
