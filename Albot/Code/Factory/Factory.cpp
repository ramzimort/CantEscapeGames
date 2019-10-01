/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "Factory.h"
#include "GameObjects/GameObject.h"
#include "Managers/SystemManager.h"
#include "Managers/GameObjectManager.h"
#include "Managers/ResourceManager.h"
#include "Reflection/Serialization.h"
#include "Reflection/Helpers.h"

//TODO: albert stuff




Factory::Factory(std::string fileName, GameObjectManager *goMgr, SystemManager *sysMgr, ResourceManager* resMgr)
	: m_pResourceManager(resMgr)
{
	//If either of these is nullptr, we have to stop
	if (!goMgr || !sysMgr)
	{
		//TODO - Call debug stuff here for error
		DEBUG_LOG("Factory couldn't load goMgr or sysMgr");
		return;
	}

	const std::string path = (Constant::LevelDir + fileName);
	const std::string levelJson = CantReflect::StringifyJson(path);
	rapidjson::Document lvlDoc;

	assert(!lvlDoc.Parse(levelJson).HasParseError());
	assert(lvlDoc.IsObject());
	assert(lvlDoc["Resources"].IsObject());

	const auto& resourcesObj = lvlDoc["Resources"].GetObjectA();
	LoadResources(resourcesObj, resMgr);

	assert(lvlDoc["Objects"].IsArray());
	const auto& objsArray = lvlDoc["Objects"].GetArray();
	for (auto it = objsArray.begin(); it != objsArray.end(); ++it)
	{
		const std::string& prefab = it->GetObjectA()["prefab"].GetString();
		const std::string& tag = it->GetObjectA()["tag"].GetString();
		LoadObject(prefab, tag, goMgr, resMgr);
	}
}

Factory::~Factory()
{
}

void Factory::LoadResources(const rapidjson::Value::Object& resObj, ResourceManager* resMgr)
{
	DEBUG_LOG("Loading Resources...\n");

	// Load Textures
	assert(resObj["Textures"].IsArray());
	const auto& textureArr = resObj["Textures"].GetArray();
	for (auto it = textureArr.begin(); it != textureArr.end(); ++it)
		resMgr->LoadTexture(it->GetString());

	// Load Models
	assert(resObj["Models"].IsArray());
	const auto& modelArr = resObj["Models"].GetArray();
	for (auto it = modelArr.begin(); it != modelArr.end(); ++it) 
		resMgr->LoadModel(it->GetString());

	// Load Materials
	assert(resObj["Materials"].IsArray());
	const auto& matArr = resObj["Materials"].GetArray();
	for (auto it = matArr.begin(); it != matArr.end(); ++it)
		resMgr->LoadMaterial(it->GetString());

	// Load Audio
	assert(resObj["Audio"].IsArray());
	const auto& audioArr = resObj["Audio"].GetArray();
	for (auto it = audioArr.begin(); it != audioArr.end(); ++it)
		resMgr->LoadAudio(it->GetString());

	// Load Prefabs
	assert(resObj["Prefabs"].IsArray());
	const auto& prefabArr = resObj["Prefabs"].GetArray();
	for (auto it = prefabArr.begin(); it != prefabArr.end(); ++it)
		resMgr->LoadPrefab(it->GetString());
}

// Helpers
rttr::variant ReadComponent(rapidjson::Value::MemberIterator& itr, const rttr::type &t, GameObject* go)
{
	std::vector<rttr::type> params;
	params.push_back(rttr::type::get<GameObject*>());
	rttr::constructor ctor = t.get_constructor(params);
	auto inst_type = ctor.get_instantiated_type();
	std::vector<rttr::argument> args;
	args.push_back(go);
	rttr::variant obj = ctor.invoke_variadic(args);
	return obj;
}


void Factory::LoadObject(const std::string& prefabName, const std::string& tag, GameObjectManager *goMgr, ResourceManager* resMgr)
{
	DEBUG_LOG("Loading Object: %s, tag: %s...\n", prefabName, tag);
	StringId prefabId = StringId(prefabName);
	GameObjectDesc desc;
	desc.tag = tag;
	desc.initializeComponentSetup = [resMgr, prefabId](GameObject* go)
	{
		rapidjson::Document objDoc;
		const std::string prefab = resMgr->GetPrefab(prefabId);
		assert(!objDoc.Parse(prefab).HasParseError());

		const std::vector<rttr::argument> args = { go };
		auto componentIterator = objDoc.GetObjectA().begin();
		for (componentIterator; componentIterator != objDoc.GetObjectA().end(); componentIterator++)
		{
			// Construct prefab containing default arguments
			std::string compName = componentIterator->name.GetString();
			rttr::variant comp = CantReflect::ReadVariant(componentIterator, rttr::type::get_by_name(compName), args);
			BaseComponent* baseComp = comp.get_value<BaseComponent*>();
			comp.get_type().get_method("Init", { rttr::type::get<ResourceManager*>() }).invoke(comp, resMgr);
			go->LinkComponent(baseComp);

			// TODO: Override parameters from the overrides 


			// TODO: Need to call Init() may need to pass resMgr into Init() to connect any models
		}

	};
	goMgr->Queue_GameObject_Instantiation(&desc);
}