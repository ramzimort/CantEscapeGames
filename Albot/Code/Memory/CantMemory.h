#pragma once

#include "StackAllocator.h"
#include "PoolAllocator.h"

namespace CantMemory
{
	namespace Stack
	{
		typedef uint32_t Marker; 

		StackAllocator g_stackAllocator(1048576U);

		void* Allocate(uint32_t size_bytes)
		{
			return g_stackAllocator.Allocate(size_bytes);
		}
		void Free(Marker marker)
		{
			g_stackAllocator.FreeToMarker(marker);
		}
		Marker GetMarker()
		{
			return g_stackAllocator.GetMarker();
		}
		void Clear()
		{
			g_stackAllocator.Clear();
		}
	}

	namespace PoolAlloc
	{
		template<typename T>
		class GlobalPool {
		public:
			template <typename... Args>
			static T* Allocate(Args &&... args)
			{
				static std::auto_ptr<Pool<T>>m_pPool(new Pool<T>(16384));
				return m_pPool.get()->Allocate();
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
			return GlobalPool<T>::Free(T* ptr);
		}
	}
}