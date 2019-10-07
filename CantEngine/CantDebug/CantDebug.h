#pragma once

#include "../../CantDebug/CantDebug.h"

#ifndef DEVELOPER
#define DEBUG_INIT(SDLWND)
#define DEBUG_TRACE(fmt, ...)
#define DEBUG_LOG
#define DEBUG_LOGVec2(VEC2)
#define DEBUG_LOGVec3(VEC3) 
#define DEBUG_LOGVec4(VEC4)
#define DEBUG_LOGMat3(MAT3) 
#define DEBUG_LOGMat4(MAT4)
#define DEBUG_UPDATE
#define DEBUG_ALLOC(POOL,ADDRESS)
#define DEBUG_FREE(POOL,ADDRESS)
#define DEBUG_FREEALL(POOL,ADDRESS)
#define DEBUG_SLIDERFLOAT(NAME, DATA, MIN, MAX)
#define DEBUG_CHECKBOX(NAME, DATA)
#define DEBUG_QUIT
#else
#define DEBUG_INIT(SDLWND) CantDebugAPI::InitDebugWindow(SDLWND);
#define DEBUG_TRACE(FMT, ...) CantDebug::Trace(FMT, __VA_ARGS__);
#define DEBUG_LOG(FMT, ...) CantDebug::Log(FMT, __VA_ARGS__);
#define DEBUG_LOGVec2(VEC2) CantDebug::LogVec2(VEC2);
#define DEBUG_LOGVec3(VEC3) CantDebug::LogVec3(VEC3);
#define DEBUG_LOGVec4(VEC4) CantDebug::LogVec4(VEC4);
#define DEBUG_LOGMat3(MAT3) CantDebug::LogMat3(MAT3);
#define DEBUG_LOGMat4(MAT4) CantDebug::LogMat4(MAT4);

#define DEBUG_UPDATE CantDebugAPI::UpdateDebugWindow();
#define DEBUG_ALLOC(POOL,ADDR) CantDebugAPI::MemoryLog(POOL, ADDR);
#define DEBUG_FREE(POOL,ADDR) CantDebugAPI::MemoryFree(POOL, ADDR);
#define DEBUG_FREEALL(POOL,STARTADDR) CantDebugAPI::MemoryFreeAll(POOL,STARTADDR);
#define DEBUG_SLIDERFLOAT(NAME, DATA, MIN, MAX) CantDebugAPI::SliderFloat(NAME, DATA, MIN, MAX);
#define DEBUG_CHECKBOX(NAME, DATA) CantDebugAPI::CheckboxUI(NAME, DATA);
#define DEBUG_QUIT CantDebugAPI::CloseDebugWindow();

namespace CantDebug
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

	inline void LogVec2(const Vector2& vec2)
	{
		DEBUG_LOG("[%f, %f]\n", vec2.x, vec2.y);
	}

	inline void LogVec3(const Vector3& vec3)
	{
		DEBUG_LOG("[%f, %f, %f]\n", vec3.x, vec3.y, vec3.z);
	}

	inline void LogVec4(const Vector4& vec4)
	{
		DEBUG_LOG("[%f, %f]\n", vec4.x, vec4.y, vec4.z, vec4.w);
	}

	inline void LogMat3(const aiMatrix3x3& mat3)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				DEBUG_LOG("%f ", mat3[i][j]);
			}
			DEBUG_LOG("\n");
		}
	}

	inline void LogMat4(const aiMatrix4x4& mat4)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				DEBUG_LOG("%f ", mat4[i][j]);
			}
			DEBUG_LOG("\n");
		}
	}
}

#endif
