
#include "CantEngine.h"
#include "Managers/EventManager.h"
#include "Managers/FrameManager.h"
#include "Directory/User.h"


namespace CantEngineAPI
{
	void InitializeEngine(const std::string& levelPath, size_t w, size_t h)
	{
		EventManager* World = EventManager::Get();
		World->Initialize(levelPath, w, h);
		FrameManager frame_manager;
		bool done = false;
		float dt;
		while (!World->IsQuit())
		{
			frame_manager.StartFrame();
			dt = static_cast<float>(frame_manager.GetFloatFrameTime());
			World->Update(dt);
			frame_manager.EndFrame();
		}
	}
	void InitializeUserSettings(const std::string& gameName)
	{
		CantDirectory::FindOrCreateUserDataFolder("Albot");
	}

}
