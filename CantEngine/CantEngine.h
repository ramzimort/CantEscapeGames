
#pragma once

#ifdef CANTENGINE_EXPORTS
#define CANTENGINE_API __declspec(dllexport)
#else
#define CANTENGINE_API __declspec(dllimport)
#endif

namespace CantEngineAPI
{
	CANTENGINE_API 	void InitializeUserSettings(const std::string& gameName);

	/**
	 * @brief Initialize the engine with startup level and window parameters
	 * 
	 * @param levelPath 
	 * @param fullscreen 
	 * @param w 
	 * @param h 
	 * @return CANTENGINE_API InitializeEngine 
	 */
	CANTENGINE_API void InitializeEngine(const std::string& levelPath, bool fullscreen, int w, int h);
}