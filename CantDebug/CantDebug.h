#pragma once

#ifdef CANTDEBUG_EXPORTS
#define CANTDEBUG_API __declspec(dllexport)
#else
#define CANTDEBUG_API __declspec(dllimport)
#endif

struct SDL_Window;

namespace CantDebug
{
	CANTDEBUG_API void InitDebugWindow(SDL_Window* d_window);

	CANTDEBUG_API void UpdateDebugWindow();

	CANTDEBUG_API void CloseDebugWindow();

	CANTDEBUG_API void ImLog();

	CANTDEBUG_API void ImPrint(std::string data);
}