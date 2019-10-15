
#include "CantDebug.h"

#ifndef DEVELOPER
#else

#include "Managers/EventManager.h"

// ALL CRAZY STUFF GOES HERE
namespace CantDebug
{
	DebugManager::DebugManager()
	{
		config.SelectionTool = false;
	}

	DebugManager::~DebugManager(){ }
}

#endif
