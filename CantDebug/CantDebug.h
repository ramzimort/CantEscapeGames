#pragma once

#ifdef CANTDEBUG_EXPORTS
#define CANTDEBUG_API __declspec(dllexport)
#else
#define CANTDEBUG_API __declspec(dllimport)
#endif

struct SDL_Window;

namespace CantDebugAPI
{
	enum type
	{
		STRING,
		FLOAT,
		INTEGER,
		VEC2,
		VEC3,
		VEC4,
	};

	struct MaterialInfo;

	struct PropertyInfo
	{
		PropertyInfo() = default;
		type t;
		size_t* i;
		float* f;
		std::string propValString;

		float min;
		float max;

		std::string goName;
		std::string compName;
		std::string propName;
	};

	CANTDEBUG_API void InitDebugWindow(const std::string& name, SDL_Window* pWindow, ID3D11Device* device, ID3D11DeviceContext* context);

	CANTDEBUG_API void UpdateDebugWindow();

	CANTDEBUG_API void Log(const char* data);

	CANTDEBUG_API void Trace(const char* data);

	CANTDEBUG_API void ProcessIO(SDL_Event& e, bool& m_quit);
	CANTDEBUG_API void MemoryLog(const char* pool, const void* address);

	CANTDEBUG_API void MemoryFree(const char* pool, const void* address);

	CANTDEBUG_API void MemoryFreeAll(const char* pool, const void* address);

	CANTDEBUG_API void SliderFloat(const char* sliderName, float* pData, float min, float max);

	CANTDEBUG_API void CheckboxUI(const char* checkboxName, bool* pFlag);

	CANTDEBUG_API void EditorSetting(const char* checkboxName, bool* pFlag);
	CANTDEBUG_API void ResourceSetting(const char* dir, const char* file, bool* pFlag);
	CANTDEBUG_API void PrefabButtonList(const char* button, bool* pFlag);
	CANTDEBUG_API void ObjectButtonList(const char* id, const char* button, bool* pFlag, bool* pDoubleClicked, bool created);
	CANTDEBUG_API void ComponentData(PropertyInfo info);
	CANTDEBUG_API void ResetResources();


}