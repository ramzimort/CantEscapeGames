#pragma once

#include <cstdarg>
#include <string>
#include "../../CantDebug/CantDebug.h"

#ifndef DEVELOPER
#define DEBUG_INIT()
#define DEBUG_TRACE(fmt, ...)
#define DEBUG_UPDATE
#define DEBUG_QUIT(SDL_EVENT)
#else
#define DEBUG_INIT() CantDebug::InitDebugWindow()
#define DEBUG_TRACE(FMT, ...) CantDebug::Print(FMT, __VA_ARGS__)
#define DEBUG_UPDATE CantDebug::UpdateDebugWindow();
#define DEBUG_QUIT(SDL_EVENT) CantDebug::CloseDebugWindow(SDL_EVENT);

namespace CantDebug
{
	template<typename... Args> 
	inline void Print(const char * fmt, Args... args)
	{
		char buffer[128];
		sprintf_s(buffer, fmt, args...);
		CantDebug::ImPrint(std::string(buffer));
	}
}

#endif
