/* Start Header------------------------------------------------------ -
Copyright(C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
Primary Author : Jose Rosenbluth
Other Authors : <None>
-End Header--------------------------------------------------------*/

#pragma once


/// PENDING
//		MAKE IT SAFE IN CASE OF OBJ DELETION
//		ADD LAMBDA SUPPORT


template <typename T>
class delegate;


template <typename RET, typename ... PARAMS>
class delegate<RET(PARAMS ...)>
{

public:
	using Callback_Type = RET(*)(void *this_, PARAMS ...);

private:
	Callback_Type m_callback = nullptr;
	void *m_thisObj = nullptr;

public:
	//FOR METHODS
	template<typename OBJ, RET(OBJ::*func_ptr)(PARAMS ...)>
	static delegate Create(OBJ *this_obj)
	{
		return delegate <RET(PARAMS...)>(this_obj, &delegate::Method_Stub<OBJ, func_ptr>);
	}

	//FOR FUNCTORS
	template<typename OBJ>
	static delegate Create(OBJ *this_obj)
	{
		return delegate <RET(PARAMS...)>(this_obj, &delegate::Functor_Stub<OBJ>);
	}

	//FOR FUNCTIONS
	template<RET(*func_ptr)(PARAMS ...)>
	static delegate Create()
	{
		return delegate <RET(PARAMS...)> ( nullptr,  &delegate::Function_Stub<func_ptr>);
	}

	//SAME AS FOR FUNCTIONS (Test with lua)
	template<typename RET, typename T1, typename T2>
	static delegate Create_Function(RET(*func_ptr)(T1, T2))
	{
		return delegate <RET(PARAMS...)>(nullptr, &delegate::Function_Stub<func_ptr>);
	}

	RET operator() (PARAMS ... params)
	{
		return m_callback(m_thisObj, params ...);
	}

	bool operator ==(delegate const& rhs) const
	{
		return (m_thisObj == rhs.m_thisObj) && (m_callback == rhs.m_callback);
	}

	bool operator !=(delegate const& rhs) const
	{
		return (m_thisObj != rhs.m_thisObj) || (m_callback != rhs.m_callback);
	}

	delegate(delegate const& rhs) :
		m_thisObj(rhs.m_thisObj), m_callback(rhs.m_callback)
	{
		//std::cout << "COPYING DELEGATE" << std::endl;
	}

	delegate& operator =(delegate const& rhs)
	{
		if (*this != rhs)
		{
			this->m_thisObj = rhs.m_thisObj;
			this->m_callback = rhs.m_callback;
		}
		else
		{
			std::cout << "Self Assignment of delegate" << std::endl;
		}

		return *this;
	}

	~delegate()
	{
		//std::cout << "DELETING DELEGATE" << std::endl;
	}


private:

	delegate() { }

	template<typename OBJ, RET(OBJ::*Method)(PARAMS ...)>
	static RET Method_Stub(void *this_, PARAMS ... params)
	{
		OBJ *caller = static_cast<OBJ*>(this_);
		return (caller->*Method)(params ...);
	}

	template<RET(*function)(PARAMS ...)>
	static RET Function_Stub(void *this_, PARAMS ... params)
	{
		return (*function)(params ...);
	}

	template<typename OBJ>
	static RET Functor_Stub(void *this_, PARAMS ... params)
	{
		OBJ *caller = static_cast<OBJ*>(this_);
		return (*caller)(params ...);
	}

	delegate(void *obj, Callback_Type cback) : m_thisObj(obj), m_callback(cback)
	{
		//std::cout << "DELEGATE CTOR" << std::endl;
	}

	template<typename OBJ, RET function_ptr(PARAMS ...)>
	void Method_Stub()
	{

	}
};