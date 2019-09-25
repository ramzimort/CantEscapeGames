// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>


//std libraries
#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <bitset>
#include <queue>

//DirectX and DirectXTK
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <DirectXTK/Inc/SimpleMath.h>
#include <DirectXTK/Inc/DirectXHelpers.h>
#include <DirectXTK/Inc/DDSTextureLoader.h>
#include <DirectXTK/Inc/WICTextureLoader.h>
#include <DirectXTK/Inc/CommonStates.h>


// SDL
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_events.h>

using namespace DirectX::SimpleMath;

#include "Helper/MathUtil.h"
#include "Helper/Constant.h"

//typedef is used here to have a matching
//name with HLSL typedef so we can share same struct
//accross HLSL shading language and C++ source files
typedef Vector4 float4;
typedef Vector3 float3;
typedef Vector2 float2;
typedef Matrix float4x4;

template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}

template<typename T>
inline void SafeReleaseDelete(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		delete ptr;
		ptr = NULL;
	}
}