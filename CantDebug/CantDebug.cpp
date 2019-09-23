// CantDebug.cpp : Defines the exported functions for the DLL application.


#include "stdafx.h"
#include "CantDebug.h"
#include "LogQueue.h"
#include "Logger.h"

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
static SDL_Window*				g_mainWindow = NULL;
static LogQueue*				g_traceQueue = NULL;
static LogQueue*				g_logQueue = NULL;
static ImGuiTextBuffer			g_buf;

// Our State
bool _update = true;
bool _showDemoWindow = false;
ImVec4 clear_color = ImVec4(0.15f, 0.1f, 0.90f, 1.00f);

// Forward declarations of helper functions
void UpdateLog();
void UpdateTrace();
void UpdateMemoryProfile();
void Render();
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

namespace CantDebugAPI
{
	void InitDebugWindow()
	{
		g_mainWindow = SDL_CreateWindow("CantDebug",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			1200,
			900,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(g_mainWindow, &wmInfo);
		HWND hwnd = (HWND)wmInfo.info.win.window;

		// Initialize Direct3D
		CreateDeviceD3D(hwnd);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForD3D(g_mainWindow);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		g_logQueue = new LogQueue();
		g_traceQueue = new LogQueue();
	}

	void UpdateDebugWindow()
	{
		if (!_update)
			return;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL2_NewFrame(g_mainWindow);
		ImGui::NewFrame();

		ImGui::Checkbox("Demo Window", &_showDemoWindow);
		if (_showDemoWindow)
			ImGui::ShowDemoWindow();

		UpdateLog();
		UpdateTrace();
		UpdateMemoryProfile();
		
		Render();
	}

	void CloseDebugWindow(const SDL_Event& _event)
	{
		if (_event.type == SDL_WINDOWEVENT &&
			_event.window.event == SDL_WINDOWEVENT_CLOSE &&
			_event.window.windowID == SDL_GetWindowID(g_mainWindow))
		{
			_update = false;
			// Cleanup

			SDL_DestroyWindow(g_mainWindow);
		}
	}

	void Log(const char* data)
	{
		g_logQueue->Push(data);
	}

	void Trace(const char* data)
	{
		g_traceQueue->Push(data);
	}
}

// Helper functions
void UpdateLog()
{
	static Logger log;
	ImGui::Begin("LogWindow");
	while (!g_logQueue->Empty())
		log.AddLog(g_logQueue->Pop().c_str());
	ImGui::End();
	log.Draw("LogWindow");
}

void UpdateTrace()
{
	ImGui::Begin("Immediate Window");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	while (!g_traceQueue->Empty())
		ImGui::Text(g_traceQueue->Pop().c_str());
	ImGui::End();
}

void UpdateMemoryProfile()
{

}

void Render()
{
	ImGui::Render();
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0); // Present with vsync
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}