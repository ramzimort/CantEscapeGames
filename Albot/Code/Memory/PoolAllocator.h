#pragma once

template<typename T>
struct Pool 
{
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

		PoolItem* mp_next;
		alignas(T) StorageType m_data;
	};

	struct PoolPage 
	{			
	public:
		PoolPage(size_t page_size) :
			mp_storage(new PoolItem[page_size])
		{
			for (size_t i = 1; i < page_size; i++)
			{
				mp_storage[i - 1].SetNextItem(&mp_storage[i]);
			}
			mp_storage[page_size - 1].SetNextItem(nullptr);
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

	Pool(size_t page_size)	: 
		m_page_size(page_size),
		m_page(new PoolPage(page_size)),
		m_free_list(m_page->GetStorage())
	{ }

	template <typename... Args> T *Allocate(Args &&... args) {
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

		return result;
	}

	void Free(T *t)
	{
		t->T::~T();
		PoolItem* current_item = PoolItem::StorageToItem(t);
		current_item->SetNextItem(m_free_list);
		m_free_list = current_item;
	}
};