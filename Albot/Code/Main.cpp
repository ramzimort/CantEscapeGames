
#include "Managers/EventManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"
#include "Managers/ScriptingManager.h"

class ScriptingManager *luaMgr;

int CALLBACK WinMain(
	__in  HINSTANCE hInstance,
	__in  HINSTANCE hPrevInstance,
	__in  LPSTR lpCmdLine,
	__in  int nCmdShow
)
{
	luaMgr = new ScriptingManager();
	EventManager* World = EventManager::Get();
	World->Initialize();
	FrameManager frame_manager;
	bool done = false;
	float dt;
	while (!World->IsQuit())
	{
		frame_manager.StartFrame();
		dt = static_cast<float>(frame_manager.GetFloatFrameTime());
		World->Update(dt);
		frame_manager.EndFrame();
		DEBUG_UPDATE
	}
	return 0;
}
