
#pragma once

#ifdef CANTENGINE_EXPORTS
#define CANTENGINE_API __declspec(dllexport)
#else
#define CANTENGINE_API __declspec(dllimport)
#endif

namespace CantEngineAPI
{
	CANTENGINE_API void InitializeEngine();
}