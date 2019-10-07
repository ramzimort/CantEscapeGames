#include "stdafx.h"
#include "CantDebug.h"
#include "src/DataQueue.h"
#include "src/SliderFloatQueue.h"
#include "src/Logger.h"
#include "src/MemoryProfiler.h"

// Data
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
static HWND						g_hwnd = NULL;
static HWINEVENTHOOK			g_hook;
static WNDCLASSEX				g_wc;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Custom Data
static DataQueue*				g_traceQueue = NULL;
static DataQueue*				g_logQueue = NULL;
static DataQueue*				g_memoryQueue = NULL;
static MemoryProfiler*			g_memoryProfiler = NULL;
static SliderFloatQueue*		g_sliderFloatQueue = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

// Our State
bool _update = true;
ImGuiTextBuffer	g_buf;
bool _showDemoWindow = false;
ImVec4 clear_color = ImVec4(0.15f, 0.1f, 0.90f, 1.00f);


// Forward declarations of imgui helper functions
void Render();
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

// Forward declarations of custom helper functions
void UpdateLog();
void UpdateTrace();
void UpdateMemoryProfile();
void UpdateGraphicsSettings();



namespace CantDebugAPI
{
	void InitDebugWindow(SDL_Window* gameWindow)
	{
		// Create application window
		g_wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Debug"), NULL };
		::RegisterClassEx(&g_wc);
		g_hwnd = ::CreateWindow(g_wc.lpszClassName, _T("Debug Window"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, g_wc.hInstance, NULL);

		// Initialize Direct3D
		if (!CreateDeviceD3D(g_hwnd))
		{
			CleanupDeviceD3D();
			::UnregisterClass(g_wc.lpszClassName, g_wc.hInstance);
			return ;
		}

		// Show the window
		::ShowWindow(g_hwnd, SW_SHOWDEFAULT);
		::UpdateWindow(g_hwnd);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(g_hwnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		g_logQueue = new DataQueue();
		g_traceQueue = new DataQueue();
		g_memoryProfiler = new MemoryProfiler();
		g_sliderFloatQueue = new SliderFloatQueue();
	}

	void UpdateDebugWindow()
	{
		if (!_update)
			return;		

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Checkbox("Demo Window", &_showDemoWindow);
		if (_showDemoWindow)
			ImGui::ShowDemoWindow();

		UpdateGraphicsSettings();
		UpdateLog();
		UpdateTrace();
		UpdateMemoryProfile();
		
		Render();
	}

	void CloseDebugWindow()
	{
		// Cleanup
		CleanupDeviceD3D();
		::DestroyWindow(g_hwnd);
		::UnregisterClass(g_wc.lpszClassName, g_wc.hInstance);
	}

	void Log(const char* data)
	{
		g_logQueue->Push(data);
	}

	void Trace(const char* data)
	{
		g_traceQueue->Push(data);
	}

	void MemoryLog(const char* pool, const void* address)
	{
		g_memoryProfiler->AddElement(pool, address);
	}

	void MemoryFree(const char* pool, const void* address)
	{
		g_memoryProfiler->FreeElement(pool, address);
	}

	void MemoryFreeAll(const char* pool, const void* address)
	{
		g_memoryProfiler->FreeAllElements(pool, address);
	}

	void SliderFloat(const char* sliderName, float* pData, float min, float max)
	{
		g_sliderFloatQueue->m_queueData.push(SliderFloatQueue::SliderFloatData{ sliderName, pData, min, max });
	}
}

// Helper functions
void UpdateLog()
{
	static Logger log;
	ImGui::Begin("Log");
	while (!g_logQueue->Empty())
		log.AddLog(g_logQueue->Pop().c_str());
	ImGui::End();
	log.Draw("Log");
}

void UpdateTrace()
{
	ImGui::Begin("Trace");
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	while (!g_traceQueue->Empty())
		ImGui::Text(g_traceQueue->Pop().c_str());
	ImGui::End();
}

void UpdateGraphicsSettings()
{
	ImGui::Begin("Graphics Settings");

	while (!g_sliderFloatQueue->m_queueData.empty())
	{
		const SliderFloatQueue::SliderFloatData& data = g_sliderFloatQueue->m_queueData.front();
		ImGui::SliderFloat(data.m_sliderFloatName.c_str(), data.m_pData, data.m_min, data.m_max);
		g_sliderFloatQueue->m_queueData.pop();
	}
	ImGui::End();
}

void UpdateMemoryProfile()
{
	g_memoryProfiler->Update();
}

void Render()
{
	// Rendering
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

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}