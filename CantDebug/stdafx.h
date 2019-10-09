// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define DIRECTINPUT_VERSION 0x0800

// Windows Header Files
#include <windows.h>
#include <assert.h>
#include <string>
#include <dinput.h>
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>


// reference additional headers your program requires here
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_win32.h"