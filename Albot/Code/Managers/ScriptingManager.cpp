/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


// Main includes
#include "ScriptingManager.h"

// BINDING INCLUDES
#include "Managers/GameObjectManager.h"
#include "Managers/SystemManager.h"
#include "GameObjects/GameObject.h"
#include "Components/AllComponentHeaders.h"


//We will use this to print from LUA to out stream
void printDebugAndLog(std::string msg)
{
	OutputDebugString(msg.c_str());
}


ScriptingManager::ScriptingManager()
{
	//We enter here
	luaState.open_libraries
	(
		sol::lib::base,
		sol::lib::coroutine, 
		sol::lib::string, 
		sol::lib::io
	);

	//Class bindings
	ManageBindings();
	
	//Global function definition
	try
	{
		// BIND GLOBAL PRINT FUNCTION ( TODO - REMOVE LATER)
		luaState["OutputPrint"] = &printDebugAndLog;
		luaState.script_file("Code/Scripts/LuaGlobalSetups.lua");
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); // ( TODO - REMOVE LATER)
	}

	//-------------------------------------
	/// 
	/// // sol::function - takes a variable number/types of arguments
	/// sol::protected_function fx = luaState["f"];
	/// ///sol::function fx = luaState["f"];
	/// int c = fx(1, 5);

	/// // put an instance of "some_class" into lua
	/// // just know here that it works and is
	/// // put into lua as a userdata
	/// luaState.set("Obj01", CppObject()); //Sets the userdata, as a pointer
	
	/*
	// binds just the member function
	luaState["method01"] = &CppObject::memberFunc;

	// binds the class to the type
	luaState.set_function("method02", &CppObject::memberFunc, CppObject{} );

	// binds just the member variable as a function
	luaState["v1"] = &CppObject::v;

	// binds class with member variable as function
	luaState.set_function("v2", &CppObject::v, CppObject{} );
	try
	{
		luaState.script
		(
			R"(
			-- need class instance if you don't bind it with the function
			print( method01(Obj01, 21, 3) )		-- 24.0
			-- does not need class instance: was bound to lua with one 
			print( method02(3, 21) )			-- 24.0
	
			-- need class instance if you don't bind it with the function
			print( v1(Obj01) ) -- 30
			-- does not need class instance. it was bound with one 
			print( v2() )      -- 30

			-- can set, still requires instance
			v1(Obj01, 212)
			-- can set, does not need class instance: was bound with one 
			v2(254)

			print( v1(Obj01) ) -- 212
			print( v2() )      -- 254
			)"
		);
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName);
	}
	//---------------------------------------

	try 
	{
		auto result1 = luaState.safe_script("bad.code");
	}
	catch (const sol::error& e) 
	{
		const char *errorName = e.what();
		OutputDebugString(errorName);
	}

	//----------------------------------------------

	///luaState.set_function("printDebugAndLog", &printDebugAndLog);
	///luaState.do_string(" printDebugAndLog(123)--'CHUPALA LUAAAA') ");
	///c_assert(value == -99);

	//----------------------------------------------
	//BINDINGS - TestComp test
	
	luaState.new_usertype<GameObjectManager>
	(
		"GameobjectManager"
	);

	luaState.new_usertype<GameObject>
	(
		"GameObject",
		sol::constructors< GameObject(GameObjectManager *goMgr) >()
	);

	sol::state_view testComp_type = luaState.new_usertype<TestComp>
	(
		"TestComp",
		sol::constructors<TestComp(GameObject *owner)>() 
	);

	// Methods (1 way)
	testComp_type["Init"] = &TestComp::Init;
	testComp_type["Begin"] = &TestComp::Begin;
	
	//Setting variables ( GET - SET )
	testComp_type["hp"]   = sol::property(&TestComp::hp, &TestComp::hp);
	
	// read-write vars (public)
	testComp_type["name"] = &TestComp::name;
	
	// Other way to set shit (const?)
	testComp_type.set( "hp2", sol::readonly(&TestComp::hp2) );
	
	
	//Test calling execution of some code on the state
	
	try
	{
		//luaState.do_file("Code/Scripts/lua_script_test_01.lua"); //Loads and runs the script
		///luaState.script_file("Code/Scripts/TestCompScript.lua");
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName);
	}

	//--------------------------------------
	luaState["number"] = 123;
	sol::object number_obj = luaState.get<sol::object>("number");
	
	// sol::type::number
	sol::type t1 = number_obj.get_type();

	c_assert(t1 == sol::type::number);

	//---------------------------
	luaState["a_function"] = []()->int
	{
		int a = 123;
		int b = 432;
		return a + b;
	};
	sol::object function_obj = luaState["a_function"];

	// sol::type::function
	sol::type t2 = function_obj.get_type();
	c_assert(t2 == sol::type::function);
	
	bool is_it_really = function_obj.is<std::function< int(void)> >();
	c_assert(is_it_really);

	// will not contain data
	sol::optional<int> check_for_me = luaState["a_function"];
	c_assert(check_for_me == sol::nullopt);

	//----------------------
	luaState.script(
		R"(
			abc = { [0] = 24 }
			def = 
			{ 
				ghi = 
				{ 
					bark = 50, 
					woof = abc 
				} 
			}
		)");

	sol::table abc = luaState["abc"];
	sol::table def = luaState["def"];
	sol::table ghi = luaState["def"]["ghi"];

	//-----------------------------
	sol::optional<int> will_not_error = luaState["abc"]["DOESNOTEXIST"]["ghi"];
	c_assert(will_not_error == sol::nullopt);

	int also_will_not_error = luaState["abc"]["def"]["ghi"]["jklm"].get_or(25);
	c_assert(also_will_not_error == 25);
	//*/
	//-------------------------------------
}


ScriptingManager::~ScriptingManager() { }


void ScriptingManager::Update() { }


sol::table ScriptingManager::GetScriptDeepCopy(std::string scriptName)
{
	try
	{
		sol::table orig = LoadOrGetLuaScript(scriptName);
		sol::table copy = luaState["deepcopy"](orig);
		return copy;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		OutputDebugString(errorName); //TODO - erase this
		return sol::lua_nil;
	}
}


sol::table ScriptingManager::LoadOrGetLuaScript(std::string scriptName) 
{
	sol::table table = m_scriptTableDic[scriptName];

	if (table == sol::lua_nil) 
	{
		try
		{
			//Load the script and retrieve the table
			table = luaState.script_file("Code/Scripts/" +
				scriptName + ".lua");

			m_scriptTableDic[scriptName] = table;
		}
		catch (const sol::error& e)
		{
			const char *errorName = e.what();
			OutputDebugString(errorName); //TODO - erase this
		}
	}

	return table;
}


void ScriptingManager::ManageBindings()
{
	////////////////////
	////   VECTORS  ////
	////////////////////


	////////////////////
	////  SYSTEMS   ////
	////////////////////
	//GAMEOBJECTMANAGER
	luaState.new_usertype<SystemManager>
	(
		"SystemManager"
	);

	///////////////////////
	////  GAMEOBJECTS  ////
	///////////////////////

	//GAMEOBJECTMANAGER
	luaState.new_usertype<GameObjectManager>
	(
		"GameObjectManager",
		"FindGameObjectById", &GameObjectManager::FindGameObjectById
	);

	//////////////////////
	////  COMPONENTS  ////
	//////////////////////

	//luaState.new_usertype<TransformComponent>
	//(
	//	"TransformComponent",
	//	"Translate", &TransformComponent::Translate,
	//	"Translate", &TransformComponent::Translate,
	//	"Rotate", &TransformComponent::Rotate,
	//	"Rotate", &TransformComponent::Rotate,
	//	"Scale", &TransformComponent::Scale,
	//	"Scale", &TransformComponent::Scale,
	//	"Scale", &TransformComponent::Scale,
	//	"GetWorldPosition", &TransformComponent::GetWorldPosition,
	//	"GetPosition", &TransformComponent::GetPosition,
	//	"GetRotation", &TransformComponent::GetRotation,
	//	"GetScale", &TransformComponent::GetScale,
	//	"GetModel", &TransformComponent::GetModel,
	//	"GetRotationMatrix", &TransformComponent::GetRotationMatrix,
	//	"GetScaleMatrix", &TransformComponent::GetScaleMatrix,
	//	"SetLocalPosition", &TransformComponent::SetLocalPosition,
	//	"SetLocalPosition", &TransformComponent::SetLocalPosition,
	//	"SetLocalRotation", &TransformComponent::SetLocalRotation
	//);

	//Setting variables ( GET - SET )
	//testComp_type["hp"] = sol::property(&TestComp::hp, &TestComp::hp);
	// read-write vars (public)
	//testComp_type["testName"] = &TestComp::testName;
	// Other way to set shit (const?)
	//testComp_type.set("hp2", sol::readonly(&TestComp::hp2));


	//GAMEOBJECT
	///sol::state_view go_type = luaState.new_usertype<GameObject>
	auto go_type = luaState.new_usertype<GameObject>
	(
		"GameObject",
		sol::constructors< GameObject(GameObjectManager *goMgr) >(),
		"GetTag", &GameObject::GetTag,
		"GetCustomComp", &GameObject::LuaGetCustomComponent
	);
}