
#pragma once

#ifdef CANTENGINE_EXPORTS
#define CANTENGINE_API __declspec(dllexport)
#else
#define CANTENGINE_API __declspec(dllimport)
#endif

namespace CantEngineAPI
{
	CANTENGINE_API void InitializeEngine(const std::string& gameName, const std::string& levelPath, bool fullscreen, int w, int h);

	//CANTENGINE_API void InitializeUserSystem();
}