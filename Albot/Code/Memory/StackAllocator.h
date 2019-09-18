#pragma once

#include <chrono>
#include <memory>

namespace CantMemory
{
	typedef uint32_t Marker;

	class StackAllocator
	{
	public:
		explicit StackAllocator(uint32_t stackSize_bytes = 1048576);

		void* Allocate(uint32_t size_bytes);
		Marker GetMarker();
		void FreeToMarker(Marker marker);
		void Clear();

	private:
		char* m_mem0;
		Marker m_marker = 0;
	};
}