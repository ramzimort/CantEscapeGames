#pragma once

#include "../../CantDebug/CantDebug.h"

#ifndef DEVELOPER
#define DEBUG_INIT()
#define DEBUG_TRACE(fmt, ...)
#define DEBUG_LOG
#define DEBUG_UPDATE
#define DEBUG_ALLOC(POOL,ADDRESS)
#define DEBUG_FREE(POOL,ADDRESS)
#define DEBUG_FREEALL(POOL,ADDRESS)
#define DEBUG_SLIDERFLOAT(NAME, DATA, MIN, MAX)
#define DEBUG_QUIT(SDL_EVENT)
#else
#define DEBUG_INIT(SDLWND) CantDebugAPI::InitDebugWindow(SDLWND)
#define DEBUG_TRACE(FMT, ...) CantDebugAPI::Trace(FMT, __VA_ARGS__)
#define DEBUG_LOG(FMT, ...) CantDebugAPI::Log(FMT, __VA_ARGS__)
#define DEBUG_UPDATE CantDebugAPI::UpdateDebugWindow();
#define DEBUG_ALLOC(POOL,ADDR) CantDebugAPI::MemoryLog(POOL, ADDR)
#define DEBUG_FREE(POOL,ADDR) CantDebugAPI::MemoryFree(POOL, ADDR)
#define DEBUG_FREEALL(POOL,STARTADDR) CantDebugAPI::MemoryFreeAll(POOL,STARTADDR)
#define DEBUG_SLIDERFLOAT(NAME, DATA, MIN, MAX) CantDebugAPI::SliderFloat(NAME, DATA, MIN, MAX)
#define DEBUG_QUIT(SDL_EVENT) CantDebugAPI::CloseDebugWindow(SDL_EVENT);

namespace CantDebugAPI
{
	template<typename... Args> 
	inline void Trace(const char * fmt, Args... args)
	{
		static char buffer[1024];
		sprintf_s(buffer, fmt, args...);
		CantDebugAPI::Trace(buffer);
	}

	template<typename... Args>
	inline void Log(const char * fmt, Args... args)
	{
		static char buffer[1024];
		sprintf_s(buffer, fmt, args...);
		CantDebugAPI::Log(buffer);
	}
}

#endif
