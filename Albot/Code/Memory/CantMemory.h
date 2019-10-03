#pragma once

#include "StackAllocator.h"
#include "PoolAllocator.h"

#define PAGESIZE 16384

namespace CantMemory
{
	class StackResource
	{
		typedef uint32_t Marker;
	public:
		static void* Allocate(uint32_t size_bytes)
		{
			return m_stackAllocator.Allocate(size_bytes);
		}
		static void Free(Marker marker)
		{
			m_stackAllocator.FreeToMarker(marker);
		}
		static Marker GetMarker()
		{
			return m_stackAllocator.GetMarker();
		}
		static void Clear()
		{
			m_stackAllocator.Clear();
		}
	private:
		static StackAllocator m_stackAllocator;
	};

	template<typename T>
	class PoolAlloc
	{
	public:
		PoolAlloc<T>() : m_pPool(new Pool<T>(PAGESIZE)) { };
		~PoolAlloc<T>() { delete m_pPool; }

		template <typename... Args>
		static T* Allocate(Args &&... args)
		{
			return m_pPool->Allocate(std::forward<Args>(args)...);
		}
		static void Free(T* ptr)
		{
			m_pPool->Free(ptr);
		}
		static Pool<T>* const m_pPool;
	};

	template<typename T>
	Pool<T>* const PoolAlloc<T>::m_pPool(new Pool<T>(PAGESIZE));
}