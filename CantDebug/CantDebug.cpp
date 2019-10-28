#include "stdafx.h"
#include "CantDebug.h"
#include "src/DataQueue.h"
#include "src/SliderFloatQueue.h"
#include "src/CheckboxQueue.h"
#include "src/Logger.h"
#include "src/MemoryProfiler.h"
#include "src/InputQueue.h"
#include "src/Editor.h"
#include "src/Material.h"
#include <mutex>

// Data
static SDL_Window*				g_mainWindow = NULL;
static ID3D11Device*            g_pd3dDevice = NULL;
static ID3D11DeviceContext*     g_pd3dDeviceContext = NULL;
static IDXGISwapChain*          g_pSwapChain = NULL;
static ID3D11RenderTargetView*  g_mainRenderTargetView = NULL;
static std::string				g_iniFileName = "imgui.ini";


// Custom Data
static DataQueue*				g_traceQueue = NULL;
static DataQueue*				g_logQueue = NULL;
static DataQueue*				g_memoryQueue = NULL;
static MemoryProfiler*			g_memoryProfiler = NULL;
static SliderFloatQueue*		g_sliderFloatQueue = NULL;
static CheckboxQueue*			g_checkBoxQueue = NULL;
static InputQueue*				g_InputQueue = NULL;
static Editor*					g_Editor = NULL;
static MaterialMaker*			g_materialMaker = NULL;
std::mutex*						g_mutex = NULL;

// Our State
bool _update = true;
ImGuiTextBuffer	g_buf;
bool _showDemoWindow = false;

// Forward declarations of custom helper functions
void UpdateWindow();
void UpdateLog(const std::string& windowName);
void UpdateTrace();
void UpdateMemoryProfile();
void UpdateGraphicsSettings();

namespace CantDebugAPI
{
	void InitDebugWindow(const std::string& userDir, SDL_Window* gameWindow, ID3D11Device* device, ID3D11DeviceContext* context, std::mutex& mutex)
	{
		std::lock_guard<std::mutex> lock(mutex);
		g_pd3dDevice = device;
		g_pd3dDeviceContext = context;
		g_mainWindow = gameWindow;

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(g_mainWindow, &wmInfo);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		g_iniFileName = userDir +"imgui.ini";
		io.IniFilename = g_iniFileName.c_str();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Setup Dear ImGui style
		//ImGui::StyleColorsDark();
		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForD3D(g_mainWindow);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

		g_logQueue = new DataQueue();
		g_traceQueue = new DataQueue();
		g_memoryProfiler = new MemoryProfiler();
		g_sliderFloatQueue = new SliderFloatQueue();
		g_checkBoxQueue = new CheckboxQueue();
		g_InputQueue = new InputQueue();
		g_Editor = new Editor();
		g_materialMaker = new MaterialMaker();

		g_mutex = &mutex;
	}

	void UpdateDebugWindow()
	{
		std::lock_guard<std::mutex> lock(*g_mutex);
		if (!_update)
			return;

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL2_NewFrame(g_mainWindow);
		ImGui::NewFrame();
		//ImGui::Checkbox("Demo Window", &_showDemoWindow);
		//if (_showDemoWindow)
		//	ImGui::ShowDemoWindow();

		UpdateWindow();
		
		// Rendering
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
	void CheckboxUI(const char* checkboxName, bool* pFlag)
	{
		g_checkBoxQueue->m_queueData.push(CheckboxQueue::CheckboxData{ checkboxName, pFlag });
	}
	void ProcessIO(SDL_Event& e, bool& quit)
	{
		ImGuiIO& io = ImGui::GetIO();
		{
			switch (e.type)
			{
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					_update = !_update;
				if (io.WantCaptureKeyboard && _update)
				{
					ImGui_ImplSDL2_ProcessEvent(&e);
					if (e.key.keysym.scancode != SDL_SCANCODE_DELETE)
						e = SDL_Event();
				}
				break;
			case SDL_KEYDOWN:
				if (io.WantCaptureKeyboard)
				{
					ImGui_ImplSDL2_ProcessEvent(&e);
					if (e.key.keysym.scancode != SDL_SCANCODE_DELETE)
						e = SDL_Event();
				}
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if (io.WantCaptureMouse)
				{
					e = SDL_Event();
					ImGui_ImplSDL2_ProcessEvent(&e);
					break;
				}
				else
					break;
			default:
				ImGui_ImplSDL2_ProcessEvent(&e);
				break;
			}
		}
	}

	void EditorSetting(DebugConfig config)
	{
		g_Editor->RegisterSettings(config);
	}

	void ResourceSetting(const char* dir, const char* file, bool* pFlag)
	{
		g_Editor->UpdateResources(dir, file, pFlag);
	}
	void ResetResources()
	{
		g_Editor->Clear();
	}
	void PrefabButtonList(const char* button, bool* pFlag)
	{
		g_Editor->UpdatePrefabs(button, pFlag);
	}
	void ObjectButtonList(const char* id, const char* button, bool* pFlag, bool* pDoubleClick, bool created)
	{
		g_Editor->UpdateObjects(id, button, pFlag, pDoubleClick, created);
	}

	void ComponentData(PropertyInfo info)
	{
		g_Editor->UpdateComponents(info);

	}

	void MaterialData(MaterialInfo info)
	{
		g_materialMaker->UpdateInfo(info);
	}
}

void UpdateWindow()
{
	ImGuiIO& io = ImGui::GetIO();
	const float MARGIN = 2.0f;
	const float MAXWIDTH = io.DisplaySize.x;
	ImVec2 window_pos = ImVec2(MARGIN, MARGIN);
	ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, io.DisplaySize.y),ImVec2(MAXWIDTH, io.DisplaySize.y));
	if (ImGui::Begin("CantDebug", 0, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::SetWindowSize(ImVec2(ImGui::GetWindowWidth(),io.DisplaySize.y));
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Edit"))
			{
				g_Editor->Update();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Graphics"))
			{
				UpdateGraphicsSettings();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Memory"))
			{
				UpdateMemoryProfile();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Material Maker"))
			{
				g_materialMaker->Update();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Console"))
			{
				UpdateTrace();
				ImGui::Separator();
				UpdateLog("Console");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	ImGui::End();

}
// Helper functions
void UpdateLog(const std::string& windowName)
{
	static Logger log;
	while (!g_logQueue->Empty())
		log.AddLog(g_logQueue->Pop().c_str());
	log.Draw(windowName.c_str());
}

void UpdateTrace()
{
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	while (!g_traceQueue->Empty())
		ImGui::Text(g_traceQueue->Pop().c_str());
}

void UpdateGraphicsSettings()
{
	ImGui::Separator();
	ImGui::Text("Checkbox settings");

	while (!g_checkBoxQueue->m_queueData.empty())
	{
		const CheckboxQueue::CheckboxData& data = g_checkBoxQueue->m_queueData.front();
		ImGui::Checkbox(data.m_checkBoxName.c_str(), data.m_pFlag);
		g_checkBoxQueue->m_queueData.pop();
	}

	ImGui::Separator();
	while (!g_sliderFloatQueue->m_queueData.empty())
	{
		const SliderFloatQueue::SliderFloatData& data = g_sliderFloatQueue->m_queueData.front();
		ImGui::SliderFloat(data.m_sliderFloatName.c_str(), data.m_pData, data.m_min, data.m_max);
		g_sliderFloatQueue->m_queueData.pop();
	}
}

void UpdateMemoryProfile()
{
	g_memoryProfiler->Update();
}

void UpdateMaterialMaker()
{

}
