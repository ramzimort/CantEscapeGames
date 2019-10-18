
#pragma once

#ifdef CANTENGINE_EXPORTS
#define CANTENGINE_API __declspec(dllexport)
#else
#define CANTENGINE_API __declspec(dllimport)
#endif

namespace CantEngineAPI
{
	CANTENGINE_API 	void InitializeUserSettings(const std::string& gameName);
	CANTENGINE_API void InitializeEngine(const std::string& levelPath, size_t scr_width, size_t scr_height);

	//CANTENGINE_API void InitializeUserSystem();
}