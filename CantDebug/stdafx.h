// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <d3d11.h>
#include <assert.h>
#include <stdio.h>
#include <string>




// reference additional headers your program requires here
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_dx11.h"