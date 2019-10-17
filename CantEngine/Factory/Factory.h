/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/

#pragma once

class GameObject;
class GameObjectManager;
class SystemManager;
class ResourceManager;
class DXRenderer;
class ScriptingManager;

class Factory
{

public:
	Factory() = default;
	~Factory() = default;

	static void Initialize(ResourceManager* resMgr, DXRenderer* dxRenderer, ScriptingManager* luaMgr);
	static void LoadLevel(const std::string& path, GameObjectManager* goMgr);
	static void LoadObject(GameObject* gameObject, const std::string& path);

	Factory(Factory const& rhs) = delete;
private:
	static void LoadResources(const rapidjson::Value::Object& resObj, ResourceManager* resMgr);
	static void LoadObject(const std::string& prefabName, const std::string& tag,
		GameObjectManager *goMgr, ResourceManager* resMgr, DXRenderer* dxrenderer, ScriptingManager *luaMgr);

	static ResourceManager* m_pResourceManager;
	static DXRenderer* m_pDXRenderer;
	static ScriptingManager* m_pScriptingManager;
};