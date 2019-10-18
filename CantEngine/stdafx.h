// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <windows.h>
#include <shlobj.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>

//std libraries
#include <assert.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <chrono>
#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <list>
#include <set>
#include <iterator>
#include <bitset>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <filesystem>


//DirectX and DirectXTK
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/DirectXHelpers.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/CommonStates.h>

// Assimp Stuff
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

// Reflection : RTTR
#include <rttr/type>
#include <rttr/rttr_enable.h>
#include <rttr/registration.h>
#include <rttr/registration_friend.h>
#include <rttr/wrapper_mapper.h>
#include <rttr/associative_mapper.h>
#include <rttr/policy.h>
#include <rttr/detail/policies/ctor_policies.h>
#include <rttr/visitor.h>

// Scripting : Lua and Sol
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

// Rapidjson
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"

using namespace DirectX::SimpleMath;

//typedef is used here to have a matching
//name with HLSL typedef so we can share same struct
//accross HLSL shading language and C++ source files
typedef Vector4 float4;
typedef Vector3 float3;
typedef Vector2 float2;
typedef Matrix float4x4;
typedef uint32_t uint;

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

// Our Libraries
#include "Helper/MathUtil.h"
#include "Helper/Constant.h"
#include "CantDebug/CantDebug.h"

