/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

#include "Components/BaseComponent.h" //TODO check if this really needs to be here
#include <bitset>
#include <string>
#include <unordered_map>
#include <queue>

class GameObject;
class SystemManager;


//Used for storing everything needed to 
//later instantiate a gameobject
struct GameObjectDesc 
{
	using FP = void(*)(GameObject *go);

	std::string tag;
	FP componentSetup;

	GameObjectDesc() : tag(""), 
		componentSetup(0)
	{}
};


class GameObjectManager
{
public:
	GameObjectManager(SystemManager *sysMgr);
	~GameObjectManager();

	//For now, the once a frame method that will process the queues
	void ProcessQueues();

	void Queue_GameObject_Instantiation(GameObjectDesc *goDesc);
	void Queue_GameObject_Destruction(size_t go_id);

private:
	void Instantiate_Queued_GameObjects();
	void Destroy_Queued_GameObjects();

private:
	std::unordered_map<size_t, GameObject*> m_gameObjects;
	std::queue<GameObjectDesc> instantiationQueue;
	std::queue<size_t> DestructionQueue;

	SystemManager *m_systemMgr;
};