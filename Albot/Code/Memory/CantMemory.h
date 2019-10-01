#pragma once

#include "StackAllocator.h"
#include "PoolAllocator.h"

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

	namespace PoolAlloc
	{
		template<typename T>
		class GlobalPool 
		{
		public:
			template <typename... Args>
			static T* Allocate(Args &&... args)
			{
				static std::auto_ptr<Pool<T>>m_pPool(new Pool<T>(16384));
				return m_pPool.get()->Allocate(std::forward<Args>(args)...);
			}
			static void Free(T* ptr)
			{
				static std::auto_ptr<Pool<T>>m_pPool(new Pool<T>(16384));
				m_pPool.get()->Free(ptr);
			}
		private:
			GlobalPool() { }
			~GlobalPool() { }
			GlobalPool(const GlobalPool& rhs) { }
			GlobalPool& operator=(const GlobalPool& rhs) { }
		};

		template<typename T>
		class GlobalPool_orig {
		public:
			static Pool<T>* const m_pPool;
		private:
			GlobalPool_orig() { }
			GlobalPool_orig(const GlobalPool_orig& rhs) { }
			GlobalPool_orig& operator=(const GlobalPool_orig& rhs) { }
		};

		template<typename T>
		Pool<T>* const GlobalPool_orig<T>::m_pPool(new Pool<T>(16384));

		template <typename T> T* Allocate()
		{
			return GlobalPool<T>::Allocate();
		}
		template <typename T> void Free(T* ptr)
		{
			GlobalPool<T>::Free(ptr);
		}
	}
}