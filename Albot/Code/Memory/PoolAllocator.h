#pragma once

namespace CantMemory
{
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

		// List of free elements. The list can be threaded between different arenas
		// depending on the deallocation pattern.
		PoolItem* m_free_list;

		// Creates a new pool that will use arenas of arena_size.
		Pool(size_t page_size)	: 
			m_page_size(page_size),
			m_page(new PoolPage(page_size)),
			m_free_list(m_page->GetStorage())
		{ }

		// Allocates an object in the current arena.
		template <typename... Args> T *alloc(Args &&... args) {
			if (m_free_list == nullptr)
			{
				// If the current arena is full, create a new one.
				std::unique_ptr<PoolPage> new_page(new PoolPage(m_page_size));
				// Link the new arena to the current one.
				new_page->SetNextPage(std::move(m_page));
				// Make the new arena the current one.
				m_page.reset(new_page.release());
				// Update the free_list with the storage of the just created arena.
				m_free_list = m_page->GetStorage();
			}

			// Get the first free item.
			PoolItem* current_item = m_free_list;

			// Update the free list to the next free item.
			m_free_list = current_item->GetNextItem();

			// Get the storage for T.
			T* result = current_item->GetStorage();
			// Construct the object in the obtained storage.
			new (result) T(std::forward<Args>(args)...);

			return result;
		}

		void free(T *t) {
			// Destroy the object.
			t->T::~T();

			// Convert this pointer to T to its enclosing pointer of an item of the
			// arena.
			PoolItem* current_item = PoolItem::StorageToItem(t);

			// Add the item at the beginning of the free list.
			current_item->SetNextItem(m_free_list);
			m_free_list = current_item;
		}
	};
}