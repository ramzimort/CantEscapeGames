/**
 * @file AudioEvents.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Factory Class responsible for main json deserialization
 */

#pragma once

class GameObject;
class GameObjectManager;
class SystemManager;
class ResourceManager;
class DXRenderer;
class ScriptingManager;

/**
 * @brief Class responsible for main json deserialization
 * 
 */
class Factory
{
	friend class DebugManager;
public:
	Factory() = default;
	~Factory() = default;
	/**
	 * @brief Initializes factory's pointers to the main resource management and related structures
	 * 
	 * @param resMgr 
	 * @param dxRenderer 
	 * @param luaMgr 
	 */
	static void Initialize(ResourceManager* resMgr, DXRenderer* dxRenderer, ScriptingManager* luaMgr);
	/**
	 * @brief Loads the level JSON and adds the objects to a specific gameobject manager
	 * 
	 * @param path 
	 * @param goMgr 
	 */
	static void LoadLevel(const std::string& path, GameObjectManager* goMgr);
	/**
	 * @brief Store given paramters in JSON Path (Prefab) to a gameobject 
	 * 
	 * @param gameObject 
	 * @param path 
	 */
	static void LoadObject(GameObject* gameObject, const std::string& path);
	/**
	 * @brief Creates a json style string containing prefab info overriden (Does a diff)
	 * 
	 * @param _prefabList 
	 * @param _overrideList 
	 * @param output 
	 */
	static void RecursiveRead(rapidjson::Value::Object& _prefabList, const rapidjson::Value::Object& _overrideList, rapidjson::Document& output);
	/**
	 * @brief Gets the gameobject's new component as a variant
	 * 
	 * @param go 
	 * @param name 
	 * @return rttr::variant 
	 */
	static rttr::variant GetComponent(GameObject* go, const std::string& name);

	Factory(Factory const& rhs) = delete;
private:
	static void LoadResources(const rapidjson::Value::Object& resObj, ResourceManager* resMgr);
	static void LoadObject(const std::string& compSetup, const std::string& prefabName, const std::string& tag, GameObjectManager *goMgr);
	

	static ResourceManager* m_pResourceManager;
	static DXRenderer* m_pDXRenderer;
	static ScriptingManager* m_pScriptingManager;

#ifdef DEVELOPER
	friend class CantDebug::DebugManager;
#endif
};