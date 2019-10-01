/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class GameObjectManager;
class SystemManager;
class ResourceManager;


class Factory
{

public:
	Factory(std::string path, GameObjectManager *goMgr, SystemManager *sysMgr, ResourceManager* resMgr);
	Factory() = delete;
	Factory(Factory const& rhs) = delete;
	~Factory();

private:
	void LoadResources(const rapidjson::Value::Object& resObj, ResourceManager* resMgr);
	void LoadObject(const std::string& prefabName, const std::string& tag, GameObjectManager *goMgr, ResourceManager* resMgr);

	ResourceManager* m_pResourceManager;
};