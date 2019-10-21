/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


// Main includes
#include "ScriptingManager.h"

// BINDING INCLUDES
#include "Graphics/Camera.h"
#include "Events/Multicast.h"
#include "Managers/GameObjectManager.h"
#include "Managers/SystemManager.h"
#include "GameObjects/GameObject.h"
#include "Components/AllComponentHeaders.h"
#include "ResourceManager.h"
#include "Events/Input/Input.h"


//We will use this to print from LUA to out stream
void ScriptOutput(const std::string& msg)
{
	OutputDebugString(msg.c_str());
	//DEBUG_LOG(msg.c_str());
}

void ScriptLog(const std::string& msg)
{
	DEBUG_LOG(msg.c_str());
}

void ScriptTrace(const std::string& msg)
{
	DEBUG_TRACE(msg.c_str());
}


#define SOL_CHECK_ARGUMENTS 1

ScriptingManager::ScriptingManager(ResourceManager* pResourcemanager) : 
	m_pResourceManager(pResourcemanager)
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
		luaState["OutputPrint"] = &ScriptOutput;
		luaState["LOG"] = &ScriptLog;
		luaState["TRACE"] = &ScriptTrace;
		luaState.script_file("Scripts/LuaGlobalSetups.lua");
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName);
		DEBUG_LOG("\n");
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
			-- does not need class instance: was bound to lua with one Y
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


sol::table ScriptingManager::GetScriptDeepCopy(StringId scriptId)
{
	try
	{
		sol::table orig = m_pResourceManager->GetScript(scriptId);
		sol::table copy = luaState["deepcopy"](orig);
		return copy;
	}
	catch (const sol::error& e)
	{
		const char *errorName = e.what();
		DEBUG_LOG(errorName); //TODO - erase this
		return sol::lua_nil;
	}
}


void ScriptingManager::ManageBindings()
{
	//////////////////////////////
	////  MULTICAST           ////
	//////////////////////////////

	luaState.new_usertype<Multicast<void(int, bool)>>
	(
		"KeyEventMulticast",
		"Bind", &Multicast<void(int, bool)>::BindLuaFunction,
		"Unbind", &Multicast<void(int, bool)>::UnbindLuaFunction
	);
	
	luaState.new_usertype< Multicast<void(Vector2, Vector2)>>
	(
		"MouseMotionMulticast",
		"Bind", &Multicast<void(Vector2, Vector2)>::BindLuaFunction,
		"Unbind", &Multicast<void(Vector2, Vector2)>::UnbindLuaFunction
	);
	
	luaState.new_usertype< Multicast<void(uint8_t, bool)>>
	(
		"MouseClickMulticast",
		"Bind", &Multicast<void(uint8_t, bool)>::BindLuaFunction,
		"Unbind", &Multicast<void(uint8_t, bool)>::UnbindLuaFunction
	);


#pragma region SCANCODE
	
	luaState["SCANCODE"] = luaState.create_table_with(
		"A", SDL_SCANCODE_A,
		"B", SDL_SCANCODE_B,
		"C", SDL_SCANCODE_C,
		"D", SDL_SCANCODE_D,
		"E", SDL_SCANCODE_E,
		"F", SDL_SCANCODE_F,
		"G", SDL_SCANCODE_G,
		"H", SDL_SCANCODE_H,
		"I", SDL_SCANCODE_I,
		"J", SDL_SCANCODE_J,
		"K", SDL_SCANCODE_K,
		"L", SDL_SCANCODE_L,
		"M", SDL_SCANCODE_M,
		"N", SDL_SCANCODE_N,
		"O", SDL_SCANCODE_O,
		"P", SDL_SCANCODE_P,
		"Q", SDL_SCANCODE_Q,
		"R", SDL_SCANCODE_R,
		"S", SDL_SCANCODE_S,
		"T", SDL_SCANCODE_T,
		"U", SDL_SCANCODE_U,
		"V", SDL_SCANCODE_V,
		"W", SDL_SCANCODE_W,
		"X", SDL_SCANCODE_X,
		"Y", SDL_SCANCODE_Y,
		"Z", SDL_SCANCODE_Z,
		"1", SDL_SCANCODE_1,
		"2", SDL_SCANCODE_2,
		"3", SDL_SCANCODE_3,
		"4", SDL_SCANCODE_4,
		"5", SDL_SCANCODE_5,
		"6", SDL_SCANCODE_6,
		"7", SDL_SCANCODE_7,
		"8", SDL_SCANCODE_8,
		"9", SDL_SCANCODE_9,
		"0", SDL_SCANCODE_0,
		"UP", SDL_SCANCODE_UP,
		"DOWN", SDL_SCANCODE_DOWN,
		"RIGHT", SDL_SCANCODE_RIGHT,
		"LEFT", SDL_SCANCODE_LEFT,
		"RETURN", SDL_SCANCODE_RETURN,
		"ESCAPE", SDL_SCANCODE_ESCAPE,
		"BACKSPACE", SDL_SCANCODE_BACKSPACE,
		"TAB", SDL_SCANCODE_TAB,
		"SPACE", SDL_SCANCODE_SPACE,
		"MINUS", SDL_SCANCODE_MINUS,
		"EQUALS", SDL_SCANCODE_EQUALS,
		"LEFTBRACKET", SDL_SCANCODE_LEFTBRACKET,
		"RIGHTBRACKET", SDL_SCANCODE_RIGHTBRACKET,
		"BLACKSLASH", SDL_SCANCODE_BACKSLASH
		);
#pragma endregion
	
#pragma region EVENTS
	luaState.set_function("OnKeyEvent", &KeyEvent::OnKeyEvent);
	luaState.set_function("OnMouseMotion", &MouseMotionEvent::OnMouseMotion);
	luaState.set_function("OnMouseClick", &MouseClickEvent::OnMouseClick);
#pragma endregion

#pragma region HELPERS
	luaState.set_function("GetRotationMatrix", &MathUtil::GetRotationMatrix);

#pragma endregion

	//luaState.new_usertype<KeyEvent>
	//(
	//	"KeyEvent",
	//	"OnKeyEvent", std::ref(KeyEvent::OnKeyEvent())
	//);


	//////////////////////////////
	////  VECTORS & MATRICES  ////
	//////////////////////////////
	luaState.new_usertype<Vector2>
		(
			"Vector2",
			sol::constructors<Vector2(), Vector2(float), Vector2(float x, float y), Vector2(const Vector2& rhs) >(),
			"x", &Vector2::x,
			"y", &Vector2::y,
			"dot", &Vector2::Dot,
			// we use 'sol::resolve' cause other operator+ can exist in the (global) namespace
			sol::meta_function::addition, sol::resolve<Vector2(Vector2 const&, Vector2 const&)>(operator+),
			sol::meta_function::subtraction, sol::resolve<Vector2(Vector2 const&, Vector2 const&)>(operator-),
			sol::meta_function::multiplication, sol::resolve<Vector2(float, Vector2 const&)>(operator*),
			sol::meta_function::multiplication, sol::resolve<Vector2(Vector2 const&, float)>(operator*)
	);
	luaState.new_usertype<Vector3>
	(
		"Vector3",
		sol::constructors<Vector3(), Vector3(float), Vector3(float x, float y, float z), Vector3(Vector3 const&) >(),
		"x", &Vector3::x,
		"y", &Vector3::y,
		"z", &Vector3::z,
		"dot", &Vector3::Dot,
		// we use 'sol::resolve' cause other operator+ can exist in the (global) namespace
		sol::meta_function::addition, sol::resolve<Vector3(Vector3 const&, Vector3 const&)>(operator+),
		sol::meta_function::subtraction, sol::resolve<Vector3(Vector3 const&, Vector3 const&)>(operator-),
		sol::meta_function::multiplication, sol::resolve<Vector3(float, Vector3 const&)>(operator*),
		sol::meta_function::multiplication, sol::resolve<Vector3(Vector3 const&, float)>(operator*)
	);
	
	luaState.new_usertype<Matrix>
	(
		"Matrix",
		sol::constructors< Matrix(), Matrix(Vector3 const&, Vector3 const&, Vector3 const&),
			Matrix(Vector4 const&, Vector4 const&, Vector4 const&, Vector4 const&),
			Matrix(float, float, float, float, float, float, float, float, 
				float, float, float, float, float, float, float, float)>(),
		"a00", &Matrix::_11, "a01", &Matrix::_12, "a02", &Matrix::_13, "a03", &Matrix::_14,
		"a11", &Matrix::_21, "a12", &Matrix::_22, "a13", &Matrix::_23, "a14", &Matrix::_24,
		"a21", &Matrix::_31, "a22", &Matrix::_32, "a23", &Matrix::_33, "a24", &Matrix::_34,
		"a31", &Matrix::_41, "a32", &Matrix::_42, "a33", &Matrix::_43, "a34", &Matrix::_44,
		sol::meta_function::addition, sol::resolve<Matrix(Matrix const&, Matrix const&)>(operator+),
		sol::meta_function::subtraction, sol::resolve<Matrix(Matrix const&, Matrix const&)>(operator-),
		sol::meta_function::multiplication, sol::resolve<Matrix(float, Matrix const&)>(operator*)
	);

	////////////////////
	////  SYSTEMS   ////
	////////////////////

	//SYSTEM MANAGER
	luaState.new_usertype<SystemManager>
	(
		"SystemManager"
	);

	///////////////////////
	////  GAMEOBJECTS  ////
	///////////////////////
	//GAMEOBJECT
	auto go_type = luaState.new_usertype<GameObject>
	(
		"GameObject",
		//sol::constructors< GameObject(GameObjectManager *goMgr) >(),
		"GetTag", &GameObject::GetTag,
		"GetId", &GameObject::GetId,
		"Manager", &GameObject::GetGOManager,
		//Static Instantiation Methods
		"Instantiate", sol::overload(
			sol::resolve<GameObject*(GameObjectManager *)>(&GameObject::Instantiate),
			sol::resolve<GameObject*(GameObjectManager*, std::string const&)>(&GameObject::Instantiate)),
		//Get scripted and engine components
		"GetCustomComp",          &GameObject::LuaGetCustomComponent,
		"GetRigidbodyComp",       &GameObject::GetComponent<RigidbodyComponent>,
		"GetRendererComp",        &GameObject::GetComponent<RendererComponent>,
		"GetMeshComp",            &GameObject::GetComponent<MeshComponent>,
		"GetLightComp",           &GameObject::GetComponent<LightComponent>,
		"GetCameraComp",          &GameObject::GetComponent<CameraComponent>,
		"GetParticleEmitterComp", &GameObject::GetComponent<ParticleEmitterComponent>,
		"GetAnimationComp",		  &GameObject::GetComponent<AnimationComponent>,
		"GetTransformComp",       &GameObject::GetComponent<TransformComponent>,
		//Add scripted and engine components
		"AddCustomComp",          &GameObject::LuaAddCustomComponent,
		"AddRigidbodyComp",       &GameObject::AddComponent<RigidbodyComponent>,
		"AddRendererComp",        &GameObject::AddComponent<RendererComponent>,
		"AddMeshComp",            &GameObject::AddComponent<MeshComponent>,
		"AddLightComp",           &GameObject::AddComponent<LightComponent>,
		"AddCameraComp",          &GameObject::AddComponent<CameraComponent>,
		"AddParticleEmitterComp", &GameObject::AddComponent<ParticleEmitterComponent>,
		"AddAnimationComp",       &GameObject::AddComponent<AnimationComponent>,
		"AddTransformComp",       &GameObject::AddComponent<TransformComponent>
	);

	//GAMEOBJECTMANAGER
	luaState.new_usertype<GameObjectManager>
	(
		"GameObjectManager",
		"FindGameObjectById", &GameObjectManager::FindGameObjectById,
		"FindGameObject", &GameObjectManager::FindGameObject
	);

	//////////////////////
	////  COMPONENTS  ////
	//////////////////////

	//TRANSFORM
	luaState.new_usertype<TransformComponent>
	(
		"TransformComponent",
		//Interface
		"Translate", sol::overload(
			sol::resolve<void(Vector3 const&)>(&TransformComponent::Translate),
			sol::resolve<void(float, float, float)>(&TransformComponent::Translate)),
		"Rotate", sol::overload(
			sol::resolve<void(Vector3 const&)>(&TransformComponent::Rotate), 
			sol::resolve<void(float, float, float)>(&TransformComponent::Rotate)),
		"Scale", sol::overload(
			sol::resolve<void(Vector3 const&)>(&TransformComponent::Scale), 
			sol::resolve<void(float, float, float)>(&TransformComponent::Scale),
			sol::resolve<void(float)>(&TransformComponent::Scale)),
		//Getters
		"GetWorldPosition", &TransformComponent::GetWorldPosition,
		"GetPosition", &TransformComponent::GetPosition,
		"GetRotation", &TransformComponent::GetRotation,
		"GetScale", &TransformComponent::GetScale,
		"GetModel", &TransformComponent::GetModel,
		"GetRotationMatrix", &TransformComponent::GetRotationMatrix,
		"GetScaleMatrix", &TransformComponent::GetScaleMatrix,
		//Setters
		"SetLocalPosition", sol::overload(
			sol::resolve<void(Vector3 const&)>(&TransformComponent::SetLocalPosition),
			sol::resolve<void(float, float, float)>(&TransformComponent::SetLocalPosition)),
		"SetLocalRotation", &TransformComponent::SetLocalRotation
	);

	//ANIMATION
	luaState.new_usertype<AnimationComponent>
	(
		"AnimationComponent",
		"SwitchAnimation", &AnimationComponent::SwitchAnimation
	);

	//RIGIDBODY
	luaState.new_usertype<RigidbodyComponent>
	(
		"RigidbodyComponent"	
	);

	//RENDERER
	luaState.new_usertype<RendererComponent>
	(
		"RendererComponent"
	);

	//MESH
	luaState.new_usertype<MeshComponent>
	(
		"MeshComponent"
	);

	//LIGHT
	luaState.new_usertype<LightComponent>
	(
		"LightComponent"
	);

	//CAMERA
	luaState.new_usertype<CameraComponent>
	(
		"CameraComponent",
		"GetCamera", &CameraComponent::GetCamera
	);

	luaState.new_usertype<Camera>
	(
		"Camera",
		"GetForward", &Camera::GetForward,
		"GetRight", &Camera::GetRight,
		"SetCameraPosition", sol::overload(
			sol::resolve<void(Vector3 const&)>(&Camera::SetCameraPosition),
			sol::resolve<void(float, float, float)>(&Camera::SetCameraPosition)),
		"ApplyRotation", &Camera::ApplyRotation
	);

	//PARTICLE_EMITTER
	luaState.new_usertype<ParticleEmitterComponent>
	(
		"ParticleEmitterComponent"
	);
}