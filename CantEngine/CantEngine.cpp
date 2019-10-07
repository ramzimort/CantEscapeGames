
#include "CantEngine.h"
#include "Managers/EventManager.h"
#include "Managers/FrameManager.h"
#include "CantDebug/CantDebug.h"


namespace CantEngineAPI
{
	void InitializeEngine()
	{
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
	}
}
