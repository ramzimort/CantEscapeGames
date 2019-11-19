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

	/**
	 * @brief MemoryPool static class. 
	 * Creates aligned memory for any data type in pools of PAGESIZE (4K bytes)
	 * Use this allocator for any dynamically allocated class set
	 * Uses free list method
	 * Additional performance benefit is made when iterating through pool objects
	 * 
	 * 
	 * @tparam T 
	 */
	template<typename T>
	class PoolResource
	{
	public:
		PoolResource<T>() : m_pPool(new Pool<T>(PAGESIZE)) { };
		~PoolResource<T>() { delete m_pPool; }
		/**
		 * @brief Use this instead of new to call constructor 
		 * Can take any constructor arguments
		 * 
		 * @tparam Args 
		 * @param args 
		 * @return T* 
		 */
		template <typename... Args>
		static T* Allocate(Args &&... args)
		{
			return m_pPool->Allocate(std::forward<Args>(args)...);
		}
		/**
		 * @brief Use this instead of delete to call destructor (just pass pointer )
		 * 
		 * @param ptr 
		 */
		static void Free(T* ptr)
		{
			m_pPool->Free(ptr);
		}
		static Pool<T>* const m_pPool;
	};

	template<typename T>
	Pool<T>* const PoolResource<T>::m_pPool(new Pool<T>(PAGESIZE));
}