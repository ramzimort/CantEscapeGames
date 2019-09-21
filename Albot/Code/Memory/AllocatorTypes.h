#pragma once

#include "CantMemory.h"

using namespace CantMemory;

template <class T>
class StackAllocatorType
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	StackAllocatorType() {}
	StackAllocatorType(const StackAllocatorType&) {}

	pointer   allocate(size_type n, const void * = 0) 
	{
		T* t = (T*)Stack::Allocate(n * sizeof(T));
		return t;
	}

	void      deallocate(void* p, size_type) { }

	pointer           address(reference x) const { return &x; }
	const_pointer     address(const_reference x) const { return &x; }
	StackAllocatorType<T>&  operator=(const StackAllocatorType&) { return *this; }
	void              construct(pointer p, const T& val)
	{
		new ((T*)p) T(val);
	}
	void              destroy(pointer p) { }

	size_type         max_size() const { return size_t(-1); }

	template <class U>
	struct rebind { typedef StackAllocatorType<U> other; };

	template <class U>
	StackAllocatorType(const StackAllocatorType<U>&) {}

	template <class U>
	StackAllocatorType& operator=(const StackAllocatorType<U>&) { return *this; }
};

template <class T>
class PoolAllocatorType
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

	PoolAllocatorType() {}
	PoolAllocatorType(const PoolAllocatorType&) {}

	pointer   allocate(size_type n, const void * = 0)
	{
		T* t = (T*)Pool::Allocate(n * sizeof(T));
		return t;
	}

	void      deallocate(void* p, size_type) { }

	pointer           address(reference x) const { return &x; }
	const_pointer     address(const_reference x) const { return &x; }
	StackAllocatorType<T>&  operator=(const StackAllocatorType&) { return *this; }
	void              construct(pointer p, const T& val)
	{
		new ((T*)p) T(val);
	}
	void              destroy(pointer p) { }

	size_type         max_size() const { return size_t(-1); }

	template <class U>
	struct rebind { typedef StackAllocatorType<U> other; };

	template <class U>
	StackAllocatorType(const StackAllocatorType<U>&) {}

	template <class U>
	StackAllocatorType& operator=(const StackAllocatorType<U>&) { return *this; }
};