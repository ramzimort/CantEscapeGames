#pragma once

class BaseCallback {
public:
	typedef std::unique_ptr<BaseCallback> Ptr;
	
	BaseCallback(void* subscriber) : 
		m_subscriber(subscriber) { }
	virtual ~BaseCallback() = default;

	const void* GetId() const { return m_subscriber; }
	virtual void Call(const void *event) = 0;
protected:

private:
	const void* m_subscriber;
};

template <class T>
class EventCallBack : BaseCallback {
public:
	typedef typename std::function<void(const T*)> Callback;
	typedef typename std::unique_ptr<EventCallBack<T>> Ptr;
public:
	EventCallBack<T>() : 
		BaseCallback(nullptr),
		m_callback([](const T* event_p) {}) { }

	EventCallBack<T>(void* objPtr, const Callback& callback) : 
		BaseCallback(objPtr),
		m_callback(callback) { }

	~EventCallBack() = default;

	void operator()(const T* event)
	{
		m_callback(event);
	}

	virtual void Call(const void *event)
	{
		const T* derived_event = static_cast<T*>(event);
		m_callback(derived_event);
	}
private:
	Callback m_callback;
};