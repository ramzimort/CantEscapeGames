#pragma once

class BaseCallback {
public:
	typedef std::unique_ptr<BaseCallback> Ptr;
	
	BaseCallback(const void* _owner) : m_owner(_owner) { }
	virtual ~BaseCallback() = 0;

	virtual void Call(const void *event) = 0;
	const void* m_owner;
};

template <class T>
class EventCallback : BaseCallback {
public:
	typedef typename std::function<void(const T*)> Callback;
	typedef typename std::unique_ptr<EventCallback<T>> Ptr;
public:
	EventCallback<T>(const void* owner, const Callback& callback) :
		BaseCallback(owner),
		m_callback(callback) { }
	
	EventCallback<T>(const EventCallback<T>& cb) :
		BaseCallback(cb.m_owner),
		m_callback(cb.m_callback) { }

	virtual ~EventCallback() = default;

	virtual void Call(const void *event)
	{
		const T* derived_event = static_cast<const T*>(event);
		m_callback(derived_event);
	}
private:
	Callback m_callback;
};