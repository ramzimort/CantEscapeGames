#pragma once

#include <exception>
namespace DX
{
	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		virtual const char* what() const override
		{
			static char s_str[64] = {};
			//TODO: make this print with developer console
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			static char s_str[64] = {};
			//TODO: make this print with developer console
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(hr));
			OutputDebugString(s_str);
			throw com_exception(hr);
		}
	}
}

#if defined(DEBUG) || defined(_DEBUG) || defined(DEVELOPER)
inline bool FAILED_HR(HRESULT hr)
{
	if (FAILED(hr))												
	{															
		DX::ThrowIfFailed(hr);
		return true;
	}
	return false;
}
#else
inline bool FAILED_HR(HRESULT hr)
{
	return FAILED(hr);
}

#endif
