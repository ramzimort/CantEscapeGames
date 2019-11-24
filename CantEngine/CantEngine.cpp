
#include "CantEngine.h"
#include "Managers/EventManager.h"
#include "Managers/FrameManager.h"
#include "Directory/User.h"
#include "Graphics/d3dUtils.h"

namespace CantEngineAPI
{
	void InitializeEngine(const std::string& gameName, const std::string& levelPath, bool fullscreen, int w, int h)
	{
		//This is not dirty, it is needed to allow multi-thread DirectXTK texture loading
		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
		if (FAILED_HR(hr))
		{
			assert(0);
		}

		EventManager* World = EventManager::Get();
		World->Initialize(gameName, levelPath, fullscreen, w, h);

		std::thread th1(&EventManager::RunGameThread, World);
		World->RunInputThread();
		th1.join();

		delete World;
	}
}
