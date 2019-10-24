
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

		std::thread th1(&EventManager::Update2, World);
		World->Update();
		th1.join();
	}
	void InitializeUserSettings(const std::string& gameName)
	{
		CantDirectory::FindOrCreateUserDataFolder("Albot");
	}

}
