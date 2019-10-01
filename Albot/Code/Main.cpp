
#include "Managers/EventManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"
#include "Managers/ScriptingManager.h"

#include "Helper/Hash.h"

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
		DEBUG_TRACE("Frame Time: %f ms", dt);
		DEBUG_TRACE("Framerate: %f Hz", 1000.f / dt);

		World->Update(dt);

		frame_manager.EndFrame();
		DEBUG_UPDATE
	}
	return 0;
}
