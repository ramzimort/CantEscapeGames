/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author:
- End Header --------------------------------------------------------*/

#include "Factory.h"
#include "GameObjects/GameObject.h"
#include "Managers/GameObjectManager.h"
#include "Managers/ResourceManager.h"
#include "Managers/ScriptingManager.h"
#include "Reflection/Serialization.h"
#include "Reflection/Helpers.h"
#include "Components/AllComponentHeaders.h"

ResourceManager* Factory::m_pResourceManager;
DXRenderer* Factory::m_pDXRenderer;
ScriptingManager* Factory::m_pScriptingManager;

// Helper function
void RecursiveRead(rapidjson::Value::Object& _prefabList, rapidjson::Value::Object& _overrideList, rapidjson::Document& doc);
rttr::variant GetComponent(GameObject* go, const std::string& name);
void LoadScripts(const rapidjson::Value::Array& scripts, GameObject* go);//, ScriptingManager* luaMgr);

void Factory::LoadLevel(const std::string& path, GameObjectManager* goMgr)
{
	const std::string levelJson = CantReflect::StringifyJson(path);
	rapidjson::Document lvlDoc;

	lvlDoc.Parse(levelJson);
	assert(!lvlDoc.HasParseError());
	assert(lvlDoc.IsObject());
	assert(lvlDoc["Resources"].IsObject());

	const auto& resourcesObj = lvlDoc["Resources"].GetObjectA();
	LoadResources(resourcesObj, m_pResourceManager);

	assert(lvlDoc["Objects"].IsArray());
	const auto& objsArray = lvlDoc["Objects"].GetArray();
	rapidjson::StringBuffer buffer;
	for (auto it = objsArray.begin(); it != objsArray.end(); ++it)
	{
		auto gameObjJson = it->GetObjectA();
		if (gameObjJson.HasMember("prefab"))
		{
			// Load the prefab string as document
			const std::string& prefabName = gameObjJson["prefab"].GetString();

			StringId prefabId = StringId(prefabName);
			const std::string prefabJson = m_pResourceManager->GetPrefab(prefabId);
			rapidjson::Document prefabDoc;
			prefabDoc.Parse(prefabJson);
			assert(!prefabDoc.HasParseError());
			auto prefabList = prefabDoc.GetObjectA();

			// Load the override string as object
			auto overrideList = gameObjJson["overrides"].GetObjectA();

			RecursiveRead(prefabList, overrideList, prefabDoc);

			// Stringify and pass to lambda for later instantiation
			rapidjson::StringBuffer buffer;
			buffer.Clear();
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			prefabDoc.Accept(writer);
			const std::string objSetup = std::string(buffer.GetString());
			const std::string tag = gameObjJson["tag"].GetString();

			// Load the object
			DEBUG_LOG("Loading Object: %s, tag: %s...\n", prefabName.c_str(), tag.c_str());
			LoadObject(objSetup, prefabName, tag, goMgr);
		}
	}
}

void Factory::LoadObject(GameObject* gameObject, const std::string& path)
{
	const std::string& compSetup = m_pResourceManager->GetPrefab(path);
	rapidjson::Document objDoc;
	objDoc.Parse(compSetup);
	assert(!objDoc.HasParseError());

	const std::vector<rttr::argument> args = { gameObject };
	auto componentIterator = objDoc.GetObjectA().begin();
	for (componentIterator; componentIterator != objDoc.GetObjectA().end(); componentIterator++)
	{
		// Construct prefab containing default arguments
		std::string compName = componentIterator->name.GetString();
		if (compName == "Scripts")
		{
			LoadScripts(componentIterator->value.GetArray(), gameObject);//, luaMgr);
		}
		else
		{
			rttr::variant comp = GetComponent(gameObject, compName);
			CantReflect::ReadRecursive(comp, componentIterator->value);
			BaseComponent* baseComp = comp.get_value<BaseComponent*>();
		}
	}
}


void Factory::Initialize(ResourceManager* resMgr, DXRenderer* dxRenderer, ScriptingManager* luaMgr)
{
	m_pResourceManager = resMgr;
	m_pDXRenderer = dxRenderer;
	m_pScriptingManager = luaMgr;
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
	
	// Load Scripts
	assert(resObj["Scripts"].IsArray());
	const auto& scriptArr = resObj["Scripts"].GetArray();
	for (auto it = scriptArr.begin(); it != scriptArr.end(); ++it)
		resMgr->LoadScript(it->GetString());


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

void Factory::LoadObject(const std::string& compSetup, const std::string& prefabName, const std::string& tag, GameObjectManager* goMgr)
{
	GameObjectDesc desc;
	desc.tag = tag;
	desc.prefabName = prefabName;
	ResourceManager* resMgr = m_pResourceManager;
	DXRenderer* dxrenderer = m_pDXRenderer;
	ScriptingManager* luaMgr = m_pScriptingManager;
	desc.initializeComponentSetup = [resMgr, compSetup, dxrenderer, luaMgr](GameObject* go)
	{
		rapidjson::Document objDoc;
		objDoc.Parse(compSetup);
		assert(!objDoc.HasParseError());

		const std::vector<rttr::argument> args = { go };
		auto componentIterator = objDoc.GetObjectA().begin();
		for (componentIterator; componentIterator != objDoc.GetObjectA().end(); componentIterator++)
		{
			// Construct prefab containing default arguments
			std::string compName = componentIterator->name.GetString();
			if (compName == "Scripts")
			{
				LoadScripts(componentIterator->value.GetArray(), go);//, luaMgr);
			}
			else
			{
				rttr::variant comp = GetComponent(go, compName);
				CantReflect::ReadRecursive(comp, componentIterator->value);
				BaseComponent* baseComp = comp.get_value<BaseComponent*>();
			}
		}
	};
	goMgr->Queue_GameObject_Instantiation(&desc);
}

// Helpers
void Factory::RecursiveRead(rapidjson::Value::Object& _prefabList, const rapidjson::Value::Object& _overrideList, rapidjson::Document& doc)
{
	for (auto compIt = _prefabList.begin(); compIt != _prefabList.end(); ++compIt)
	{
		const char* memberName = compIt->name.GetString();
		if (_overrideList.HasMember(memberName))
		{
			auto& _member = _prefabList[memberName];
			auto& _override = _overrideList[memberName];
			auto a = _member.GetType();
			auto b = _override.GetType();
			switch (_member.GetType())
			{
			case rapidjson::kObjectType:
			{
				auto _prefabObjList = _member.GetObjectA();
				auto _overrideObjList = _override.GetObjectA();
				RecursiveRead(_prefabObjList, _overrideObjList, doc);
				break;
			}
			case rapidjson::kStringType:
			{
				std::string val = _override.GetString();
				_member.SetString(val.c_str(), doc.GetAllocator());
				break;
			}
			case rapidjson::kNullType:     break;
			case rapidjson::kFalseType:
			case rapidjson::kTrueType:
			{
				_member.SetBool(_override.GetBool());
				break;
			}
			case rapidjson::kNumberType:
			{
				if (_member.IsInt())
					_member.SetInt(_override.GetInt());
				else if (_member.IsDouble())
					_member.SetDouble(_override.GetDouble());
				else if (_member.IsUint())
					_member.SetUint(_override.GetUint());
				else if (_member.IsInt64())
					_member.SetInt64(_override.GetInt64());
				else if (_member.IsUint64())
					_member.SetUint64(_override.GetUint64());
				break;
			}
			// No 2d Arrays!
			case rapidjson::kArrayType:
			{
				auto it2 = _override.GetArray().begin();
				for (auto it = _member.GetArray().begin(); it != _member.GetArray().end() && it2 != _override.GetArray().end(); ++it, ++it2)
				{
					switch (it->GetType())
					{
					case rapidjson::kObjectType:
					{
						auto _prefabObjList = it->GetObjectA();
						auto _overrideObjList = it2->GetObjectA();
						RecursiveRead(_prefabObjList, _overrideObjList, doc);
						break;
					}
					case rapidjson::kStringType:
					{
						std::string val = it2->GetString();
						it2->SetString(val.c_str(), static_cast<rapidjson::SizeType>(val.length()));
						break;
					}
					case rapidjson::kNullType:     break;
					case rapidjson::kFalseType:
					case rapidjson::kTrueType:
					{
						it->SetBool(it2->GetBool());
						break;
					}
					case rapidjson::kNumberType:
					{
						if (it->IsInt())
							it->SetInt(it2->GetInt());
						else if (it->IsDouble())
							it->SetDouble(it2->GetDouble());
						else if (it->IsUint())
							it->SetUint(it2->GetUint());
						else if (it->IsInt64())
							it->SetInt64(it2->GetInt64());
						else if (it->IsUint64())
							it->SetUint64(it2->GetUint64());
						break;
					}
					}
				}
			}
			}
		}
	}
}

rttr::variant Factory::GetComponent(GameObject* go, const std::string& name)
{
	if (name == "TransformComponent")
		return go->AddComponent<TransformComponent>();
	else if (name == "RigidBodyComponent")
		return go->AddComponent<RigidbodyComponent>();
	else if (name == "RendererComponent")
		return go->AddComponent<RendererComponent>();
	else if (name == "MeshComponent")
		return go->AddComponent<MeshComponent>();
	else if (name == "LightComponent")
		return go->AddComponent<LightComponent>();
	else if (name == "CameraComponent")
		return go->AddComponent<CameraComponent>();
	else if (name == "ParticleEmitterComponent")
		return go->AddComponent<ParticleEmitterComponent>();
	else if (name == "HaloEffectComponent")
		return go->AddComponent<HaloEffectComponent>();
	else if (name == "UIComponent")
		return go->AddComponent<UIComponent>();
	else if (name == "AnimationComponent")
		return go->AddComponent<AnimationComponent>();
	
	return rttr::variant();
}


void LoadScripts(const rapidjson::Value::Array& scripts, GameObject* go)//, ScriptingManager* luaMgr)
{
	// Read Name
	auto it = scripts.begin();
	for (auto it = scripts.begin(); it != scripts.end(); ++it)
	{
		// Load script 
		auto script = it->GetObjectA();
		assert(script.HasMember("Name"));
		const std::string& scriptName = script["Name"].GetString();
		CustomComponent *c1 = go->AddCustomComponent(scriptName);// , luaMgr);
		
		// Overrides
		if (script.HasMember("Overrides"))
		{
			for (auto it2 = script["Overrides"].GetObjectA().begin(); it2 != script["Overrides"].GetObjectA().end(); ++it2)
			{
				const std::string& _member = it2->name.GetString();
				const auto& _value = it2->value;
				switch (_value.GetType())
				{
				case rapidjson::kStringType:
					c1->Override(_member, _value.GetString());
					break;
				case rapidjson::kFalseType:
				case rapidjson::kTrueType:
					c1->Override(_member, _value.GetBool());
					break;
				case rapidjson::kNumberType:
					if (_value.IsInt())
						c1->Override(_member, _value.GetInt());
					else if (_value.IsDouble())
						c1->Override(_member, _value.GetDouble());
					else if (_value.IsUint())
						c1->Override(_member, _value.GetUint());
					else if (_value.IsInt64())
						c1->Override(_member, _value.GetInt64());
					else if (_value.IsUint64())
						c1->Override(_member, _value.GetUint64());
					break;
				default:
					break;
				}
			}
		}
	}

}