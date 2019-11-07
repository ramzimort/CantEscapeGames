#pragma once

#include "StackAllocator.h"
#include "PoolAllocator.h"

#define PAGESIZE 4096

namespace CantMemory
{
	class StackResource
	{
		typedef size_t Marker;
	public:
		template <typename T, typename ...Args>
		static void* Allocate(uint32_t size_bytes, Args&&... args)
		{
			return m_stackAllocator.Allocate<T>(size_bytes, std::forward<Args>(args));
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
	class PoolResource
	{
	public:
		PoolResource<T>() : m_pPool(new Pool<T>(PAGESIZE)) { };
		~PoolResource<T>() { delete m_pPool; }

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
	Pool<T>* const PoolResource<T>::m_pPool(new Pool<T>(PAGESIZE));
}