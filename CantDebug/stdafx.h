// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define DIRECTINPUT_VERSION 0x0800

// Windows Header Files
#include <windows.h>
#include <d3d11.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <dinput.h>
#include <tchar.h>



// reference additional headers your program requires here
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"