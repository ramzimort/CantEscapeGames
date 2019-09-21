#pragma once

#include "StackAllocator.h"
#include "PoolAllocator.h"
#include "CantDebug/CantDebug.h"

namespace CantMemory
{
	namespace Stack
	{
		StackAllocator g_stackAllocator(1048576U);

		template <typename T> void* Allocate(uint32_t size_bytes)
		{
			g_stackAllocator.Allocate(size_bytes)
		}
		template <typename T> void Free()
		{

		}
	}

	namespace PoolAlloc
	{
		template<typename T>
		class GlobalPool {
		public:
			template <typename... Args> T *alloc(Args &&... args)
			static T* Shared() {
				static std::auto_ptr<T>pVar(new T);
				return pVar.get();
			}

		private:
			Pool<T> m_pool;
			GlobalPool() { }
			~GlobalPool() { }
			GlobalPool(const GlobalPool& rhs) { }
			GlobalPool& operator=(const GlobalPool& rhs) { }
		};

		template<typename T>
		class GlobalPool_orig {
		public:
			static T* const pVar;
		private:
			GlobalPool_orig() { }
			GlobalPool_orig(const GlobalPool_orig& rhs) { }
			GlobalPool_orig& operator=(const GlobalPool_orig& rhs) { }
		};

		template<typename T>T* const GlobalPool_orig<T>::pVar(new T);

		template <typename T> void* Allocate()
		{
			GlobalPool<T>::Shared();
		}
		template <typename T> void Free()
		{

		}
	}
}