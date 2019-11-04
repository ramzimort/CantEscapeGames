// Albot.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	CantEngineAPI::InitializeUserSettings("Albot");
	CantEngineAPI::InitializeEngine("Assets/Levels/JoseLevel.json", false, 1280, 720);
}