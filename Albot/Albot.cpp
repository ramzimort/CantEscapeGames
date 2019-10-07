// Albot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "..\CantEngine\CantEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// The screen size

	// Initial state path (json file)
    CantEngineAPI::InitializeEngine();

}