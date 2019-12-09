#pragma once

typedef uint64_t Marker;

class StackAllocator
{
public:
	StackAllocator(size_t stackSize_bytes = 1048576);
	~StackAllocator();

	template <typename T, typename...Args>
	void* Allocate(size_t size_bytes, Args&&... args);
	Marker GetMarker();
	void FreeToMarker(Marker marker);
	void Clear();

private:
	char* m_mem0;
	Marker m_marker;
};

template <typename T, typename...Args>
void* StackAllocator::Allocate(size_t size_bytes, Args&&... args)
{
	void* result = m_mem0 + m_marker;
	m_marker += size_bytes;
	DEBUG_ALLOC(typeid(*this).name(), static_cast<const void*>(result));
	return result;
}