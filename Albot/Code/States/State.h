/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/


#pragma once

class Factory;

//Check which managers the state has
class GameObjectManager;
class SystemManager;


class State
{
public:
	State(std::string const& path);
	virtual ~State();

	//Call update on systemManager
	virtual void Update(float dt);

	//Call draw on systemManager
	virtual void Draw(float dt);

	//Calls the gameObjMgr to check its queues
	void ProcessInstantiationAndDestruction();

private:
	Factory *m_factory;
	GameObjectManager *m_gameObjectMgr;
	SystemManager *m_systemMgr;
};

