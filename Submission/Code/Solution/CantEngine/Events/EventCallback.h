#pragma once

/**
 * @file EventCallback.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Event Callback Classes
 */

/**
 * @brief Base Event Callback Class that is mirrored for BaseEvents.
 * Carries function callback pointer
 * 
 */
class BaseCallback 
{
public:
	typedef std::unique_ptr<BaseCallback> Ptr;
	/**
	 * @brief Construct a new Base Callback object
	 * 
	 * @param _owner Pointer to owning object's pointer in memory (used for book-keeping)
	 */
	BaseCallback(const void* _owner) : m_owner(_owner) { }
	virtual ~BaseCallback() = default;
	/**
	 * @brief Get the owner's pointer (used for book-keeping)
	 * 
	 * @return const void* 
	 */
	const void* GetOwner() const { return m_owner; }
	/**
	 * @brief Call bound function passing inherited event in non abstract function
	 * 
	 * @param event 
	 */
	virtual void Call(const void *event) = 0;

protected:
	const void* m_owner;
};

/**
 * @brief Derived Event Callback Class that is mirrored for Event<T>.
 * 
 * @tparam T 
 */
template <class T>
class EventCallback : public BaseCallback 
{
public:
	typedef typename std::function<void(const T*)> Callback;
	typedef typename std::unique_ptr<EventCallback<T>> Ptr;
public:
	/**
	 * @brief Construct a new Event Callback object
	 * 
	 * @param owner 
	 * @param callback 
	 */
	EventCallback(const void* owner, const Callback& callback) :
		BaseCallback(owner),
		m_callback(callback) { }
	/**
	 * @brief Construct a new Event Callback object
	 * 
	 * @param cb 
	 */
	EventCallback(const EventCallback<T>& cb) :
		BaseCallback(cb.m_owner),
		m_callback(cb.m_callback) { }

	const void* GetOwner() const { return m_owner; }

	virtual ~EventCallback() = default;
	/**
	 * @brief Cast the base event to its derived type and perform the callback using the event as a parameter
	 * 
	 * @param event 
	 */
	virtual void Call(const void *event)
	{
		const T* derived_event = static_cast<const T*>(event);
		m_callback(derived_event);
	}
	void operator()(const void *event) {};
private:
	Callback m_callback;
};