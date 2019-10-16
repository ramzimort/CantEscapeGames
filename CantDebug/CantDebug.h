#pragma once

#ifdef CANTDEBUG_EXPORTS
#define CANTDEBUG_API __declspec(dllexport)
#else
#define CANTDEBUG_API __declspec(dllimport)
#endif

struct SDL_Window;

namespace CantDebugAPI
{
	CANTDEBUG_API void InitDebugWindow(const std::string& name, SDL_Window* pWindow, ID3D11Device* device, ID3D11DeviceContext* context);

	CANTDEBUG_API void InitializeAssetDirectories(const std::string& texturesDir,
		const std::string& modelDir,
		const std::string& materialDir,
		const std::string& scriptDir,
		const std::string& prefabDir,
		const std::string& audioDir);

	CANTDEBUG_API void UpdateDebugWindow();

	CANTDEBUG_API void Log(const char* data);

	CANTDEBUG_API void Trace(const char* data);

	CANTDEBUG_API void ProcessIO(SDL_Event& e, bool& m_quit);

	CANTDEBUG_API void MemoryLog(const char* pool, const void* address);

	CANTDEBUG_API void MemoryFree(const char* pool, const void* address);

	CANTDEBUG_API void MemoryFreeAll(const char* pool, const void* address);

	CANTDEBUG_API void SliderFloat(const char* sliderName, float* pData, float min, float max);

	CANTDEBUG_API void CheckboxUI(const char* checkboxName, bool* pFlag);

}