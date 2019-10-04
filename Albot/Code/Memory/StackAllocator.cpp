#include "StackAllocator.h"
#include "CantDebug/CantDebug.h"

StackAllocator::StackAllocator(size_t stackSize_bytes) :
	m_mem0(new char[stackSize_bytes]),
	m_marker(0)
{ }

StackAllocator::~StackAllocator()
{
	Clear();
}



Marker StackAllocator::GetMarker()
{
	return m_marker;
}

void StackAllocator::FreeToMarker(Marker marker)
{
	m_marker = marker;
	DEBUG_FREEALL(typeid(*this).name(), static_cast<const void*>(m_mem0 + marker));
}

void StackAllocator::Clear()
{
	m_marker = 0;
	delete m_mem0;
}
