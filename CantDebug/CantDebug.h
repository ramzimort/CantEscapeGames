#pragma once

#ifdef CANTDEBUG_EXPORTS
#define CANTDEBUG_API __declspec(dllexport)
#else
#define CANTDEBUG_API __declspec(dllimport)
#endif

namespace CantDebug
{
	CANTDEBUG_API void InitDebugWindow(SDL_Window* d_window);

	CANTDEBUG_API void UpdateDebugWindow();

	CANTDEBUG_API void CloseDebugWindow();

	CANTDEBUG_API void Log();

	CANTDEBUG_API void Print(std::string data);
}