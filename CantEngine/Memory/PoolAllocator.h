#pragma once

/**
 * @brief Memory Pool class
 * 
 * @tparam T - Pool Object Type
 */
template<typename T>
struct Pool 
{
	/**
	 * @brief PoolItem Alligned based on sizeof(T) as an array of bytes. 
 	 * Either contains data or a pointer to the next free element in the pool.
	 * 
	 */
	union PoolItem
	{
	public:
		PoolItem *GetNextItem() const { return mp_next; }
		void SetNextItem(PoolItem *n) { mp_next = n; }
		T* GetStorage() { return reinterpret_cast<T*>(m_data); }
		static PoolItem* StorageToItem(T *t)
		{
			PoolItem* current_item = reinterpret_cast<PoolItem*>(t);
			return current_item;
		}
	private:
		using StorageType = char[sizeof(T)];
		alignas(T) StorageType m_data;
		PoolItem* mp_next;
	};

	/**
	 * @brief Allocates and initializes linked list of poolitems (as pointers) based on page size
	 * Contains unique pointer to array of Poolitems 
	 * Contains unique poninter to next pool as well. 
	 * When unique pointers go out of scope, all allocated memory is destroyed
	 * 
	 */
	struct PoolPage 
	{			
	public:
		PoolPage(size_t n) :
			mp_storage(new PoolItem[n])
		{
			for (size_t i = 1; i < n; i++)
			{
				mp_storage[i - 1].SetNextItem(&mp_storage[i]);
			}
			mp_storage[n - 1].SetNextItem(nullptr);
		}
		PoolItem* GetStorage() const { return mp_storage.get(); }
			
		void SetNextPage(std::unique_ptr<PoolPage> &&n) 
		{
			assert(!mp_next);
			mp_next.reset(n.release());
		}

	private:
		std::unique_ptr<PoolItem[]> mp_storage;
		std::unique_ptr<PoolPage> mp_next;
	}; 


	size_t m_page_size;
	std::unique_ptr<PoolPage> m_page;

	PoolItem* m_free_list;

	/**
	 * @brief Constructs a new memory pool for the given type allocating a page of size page_size
	 * Will create the array of elements by dividing page size over the sizeof(T)
	 * 
	 * @param page_size 
	 */
	Pool(size_t page_size)	: 
		m_page_size(page_size),
		m_page(new PoolPage(page_size/static_cast<size_t>(sizeof(T)))),
		m_free_list(m_page->GetStorage())
	{ }

	/**
	 * @brief Used for allocating an element in the pool
	 * Performs safety check whether end of page before allocating
	 * 
	 * @tparam Args 
	 * @param args 
	 * @return T* 
	 */
	template <typename... Args> T *Allocate(Args &&... args) 
	{
		if (m_free_list == nullptr)
		{
			std::unique_ptr<PoolPage> new_page(new PoolPage(m_page_size));
			new_page->SetNextPage(std::move(m_page));
			m_page.reset(new_page.release());
			m_free_list = m_page->GetStorage();
		}
		PoolItem* current_item = m_free_list;
		m_free_list = current_item->GetNextItem();
		T* result = current_item->GetStorage();
		new (result) T(std::forward<Args>(args)...);
		DEBUG_ALLOC(typeid(*this).name(), static_cast<const void*>(result));
		return result;
	}

	/**
	 * @brief Removes element from the pool. 
	 * It becomes the free item for allocation, settings its next free item to what was there beforehand free item
	 * 
	 * @param t 
	 */
	void Free(T *t)
	{
		DEBUG_FREE(typeid(*this).name(), static_cast<const void*>(t));
		t->T::~T();
		PoolItem* current_item = PoolItem::StorageToItem(t);
		current_item->SetNextItem(m_free_list);
		m_free_list = current_item;
	}
};