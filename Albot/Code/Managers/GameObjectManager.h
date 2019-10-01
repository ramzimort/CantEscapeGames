/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class GameObject;
class SystemManager;

struct GameObjectDesc 
{
	typedef std::function<void(GameObject*)> InitializeComponentSetup;
	
	std::string tag;
	InitializeComponentSetup initializeComponentSetup;

	GameObjectDesc() :
		tag(""),
		initializeComponentSetup([](GameObject*) {})
	{	}
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
	std::queue<GameObjectDesc> m_instantiationQueue;
	std::queue<size_t> m_destructionQueue;

	SystemManager *m_systemMgr;
};