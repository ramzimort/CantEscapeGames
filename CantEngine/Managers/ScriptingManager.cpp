/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Primary Author: Jose Rosenbluth
- End Header --------------------------------------------------------*/


// Main includes
#include "ScriptingManager.h"

// BINDING INCLUDES
#include "GameObjectManager.h"
#include "SystemManager.h"
#include "StateManager.h"
#include "ResourceManager.h"
#include "Directory/User.h"

#include "GameObjects/GameObject.h"
#include "Components/AllComponentHeaders.h"
#include "Graphics/Camera.h"
#include "Graphics/AppRenderer.h"
#include "Graphics/InstanceRenderData.h"
#include "Animation/AnimatorController.h"
#include "Animation/Animation.h"

#include "EventManager.h"
#include "Events/Multicast.h"
#include "Events/Input/Input.h"
#include "Events/Audio/AudioEvents.h"
#include "Events/State/StateEvents.h"

//We will use this to print from LUA to out stream
void ScriptOutput(const std::string& msg)
{
	OutputDebugString(msg.c_str());
	//DEBUG_LOG(msg.c_str());
}

float MathAcos(float dot)
{
	return acosf(dot);
}

float Abs(float val)
{
	return fabs(val);
}

int random(int min, int max) 
{
	return min + (rand() % (max - min));
}

void ScriptLog(const std::string& msg)
{
	DEBUG_LOG(msg.c_str());
}

void ScriptTrace(const std::string& msg)
{
	DEBUG_TRACE(msg.c_str());
}

ResourceManager* gResMgr;
const std::wstring& GetLString(const std::string& key)
{
	return gResMgr->GetLString(key);
}


#define SOL_CHECK_ARGUMENTS 1

ScriptingManager::ScriptingManager(ResourceManager* pResourcemanager, AppRenderer* pAppRenderer, UserManager* userManager) :
	m_pResourceManager(pResourcemanager),
	m_pAppRenderer(pAppRenderer),
	m_pUserManager(userManager)
{
	//We enter here
	luaState.open_libraries
	(
		sol::lib::base,
		sol::lib::coroutine,
		sol::lib::string,
		sol::lib::math,
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
		
		//Math helpers (cause std library of lua isn't working)
		luaState["Acos"] = &MathAcos;
		luaState["Abs"] = &Abs;
		luaState["Range"] = &random;
		
		luaState.script_file("Scripts\\LuaGlobalSetups.lua");

	}
	catch (const sol::error & e)
	{
		const char* errorName = e.what();
		DEBUG_LOG(errorName);
		DEBUG_LOG("\n");
	}
	gResMgr = pResourcemanager;
}

ScriptingManager::~ScriptingManager() { }


void ScriptingManager::Update() { }

//const std::wstring& ScriptingManager::GetString(const std::string& luaKey)
//{
//	return m_pResourceManager->GetLString(luaKey);
//}


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
	////  Helpers             ////
	//////////////////////////////

	luaState.set_function("Localize", &GetLString);
	luaState.set_function("SetSettingBool", &UserManager::SetSettingBool, m_pUserManager);
	luaState.set_function("GetSettingBool", &UserManager::GetSettingBool, m_pUserManager);
	luaState.set_function("SetSettingDouble", &UserManager::SetSettingDouble, m_pUserManager);
	luaState.set_function("GetSettingDouble", &UserManager::GetSettingDouble, m_pUserManager);
	luaState.set_function("SetSettingInt", &UserManager::SetSettingInt, m_pUserManager);
	luaState.set_function("GetSettingInt", &UserManager::GetSettingInt, m_pUserManager);
	luaState.set_function("SetSettingString", &UserManager::SetSettingString, m_pUserManager);
	luaState.set_function("GetSettingString", &UserManager::GetSettingString, m_pUserManager);


#pragma region EVENTS
	//////////////////////////////
	////  MULTICAST           ////
	//////////////////////////////
	luaState.new_usertype<Multicast<void(int, bool)>>
		(
			"KeyEventMulticast",
			"Bind", &Multicast<void(int, bool)>::BindLuaFunction,
			"Unbind", &Multicast<void(int, bool)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(Vector2, Vector2)>>
		(
			"MouseMotionMulticast",
			"Bind", &Multicast<void(Vector2, Vector2)>::BindLuaFunction,
			"Unbind", &Multicast<void(Vector2, Vector2)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(uint8_t, bool)>>
		(
			"MouseClickMulticast",
			"Bind", &Multicast<void(uint8_t, bool)>::BindLuaFunction,
			"Unbind", &Multicast<void(uint8_t, bool)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(int32_t, int32_t)>>
		(
			"MouseScrollMulticast",
			"Bind", &Multicast<void(int32_t, int32_t)>::BindLuaFunction,

			"Unbind", &Multicast<void(int32_t, int32_t)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(int, int, float, float)>>
		(
			"WindowSizeMulticast",
			"Bind", &Multicast<void(int, int, float, float)>::BindLuaFunction,
			"Unbind", &Multicast<void(int, int, float, float)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(GameObject*, GameObject*)>>
		(
			"RigidbodyMultiCast",
			"Bind", &Multicast<void(GameObject*, GameObject*)>::BindLuaFunction,
			"Unbind", &Multicast<void(GameObject*, GameObject*)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(GameObject*, GameObject*)>>
		(
			"TriggerMultiCast",
			"Bind", &Multicast<void(GameObject*, GameObject*)>::BindLuaFunction,
			"Unbind", &Multicast<void(GameObject*, GameObject*)>::UnbindLuaFunction
			);
	luaState.new_usertype<Multicast<void(const uint32_t, const uint32_t, const float)>>
		(
			"JoystickMotionMulticast",
			"Bind", &Multicast<void(const uint32_t, const uint32_t, const float)>::BindLuaFunction,
			"Unbind", &Multicast<void(const uint32_t, const uint32_t, const float)>::UnbindLuaFunction
			);

	luaState.new_usertype<Multicast<void(const uint32_t, const uint32_t, const bool)>>
		(
			"JoystickButtonMulticast",
			"Bind", &Multicast<void(const uint32_t, const uint32_t, const bool)>::BindLuaFunction,
			"Unbind", &Multicast<void(const uint32_t, const uint32_t, const bool)>::UnbindLuaFunction
			);


	luaState.set_function("OnKeyEvent", &KeyEvent::OnKeyEvent);
	luaState.set_function("OnMouseMotion", &MouseMotionEvent::OnMouseMotion);
	luaState.set_function("OnMouseScroll", &MouseScrollEvent::OnMouseScroll);
	luaState.set_function("OnMouseClick", &MouseClickEvent::OnMouseClick);
	luaState.set_function("OnWindowSize", &WindowSizeEvent::OnWindowSizeEvent);
	luaState.set_function("OnResourcesLoaded", &ResourcesLoadedEvent::OnResourcesLoaded);
	luaState.set_function("OnJoystickMotion", &JoystickMotionEvent::OnJoyMotionEvent);
	luaState.set_function("OnJoystickButton", &JoystickButtonEvent::OnJoystickButtonEvent);

	//Solution so scripting can access stuff, even though the rest of the engine cant
	luaState.new_usertype<EventManager>
		(
			"EventManager",
			"Get", &EventManager::Get,
			"Quit", &EventManager::EnqueueEvent<QuitEvent, bool>,
			// Resize Window Event
			"SetWindowSize", &EventManager::EnqueueEvent <GameWindowSizeEvent, bool, int, int>,


			// State Events
			"PushState", &EventManager::EnqueueEvent <PushStateEvent, bool, const std::string>,
			"PopState", &EventManager::EnqueueEvent <PopStateEvent, bool>,
			"LoadState", &EventManager::EnqueueEvent <LoadStateEvent, bool, const std::string>,
			"PushLoadedState", &EventManager::EnqueueEvent <PushLoadedStateEvent, bool>,

			//Audio Events
			"PlaySong", &EventManager::EnqueueEvent <PlaySongEvent, bool, const std::string>,
			"PlaySFX", &EventManager::EnqueueEvent<PlaySFXEvent, bool, const std::string>
			);



#pragma endregion

#pragma region FONTTYPE
	luaState["FONT_TYPE"] = luaState.create_table_with(
		"COURIER_NEW", FontType::COURIER_NEW,
		"COURIER_NEW_BOLD", FontType::COURIER_NEW_BOLD
	);
#pragma endregion

#pragma region CONTROLLER
	luaState["CONTROLLER"] = luaState.create_table_with(
		"A", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A,
		"B", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B,
		"X", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X,
		"Y", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y,
		"LB", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK,
		"RB", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_GUIDE,
		"Select", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START,
		"Start", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSTICK,
		"LS", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		"RS", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		"DUP", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP,
		"DDOWN", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		"DLEFT", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		"DRIGHT", SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT
	);
#pragma endregion
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
		"BLACKSLASH", SDL_SCANCODE_BACKSLASH,
		"ENTER", SDL_SCANCODE_RETURN
	);
#pragma endregion
#pragma region CollisionMaskEnum
#define COLLISION_MASK(ENTRY) #ENTRY, CollisionTable::CollisionMask::ENTRY,
	luaState["CollisionMask"] = luaState.create_table_with(
		#include "Physics\CollisionMaskTypes.def"
		"NUM", CollisionTable::CollisionMask::NUM
	);
#undef COLLISION_MASK
#pragma endregion

#pragma region HELPERS
	luaState.set_function("ConvertToDegrees", &DirectX::XMConvertToDegrees);
	luaState.set_function("ConvertToRadians", &DirectX::XMConvertToRadians);
	luaState.set_function("CreateTranslation", sol::overload(
		sol::resolve<Matrix(const Vector3&)>(&Matrix::CreateTranslation),
		sol::resolve<Matrix(float, float, float)>(&Matrix::CreateTranslation)
		)
	);
	luaState.set_function("CreateRotationMatrixFromDegrees", &MathUtil::CreateRotationMatrixFromDegrees);
	luaState.set_function("GetRotationMatrix", &MathUtil::GetRotationMatrix);
	luaState.set_function("MatrixToRadEulerAngles", &MathUtil::MatrixToRadEulerAngles);
	luaState.set_function("MatrixToDegreeEulerAngles", &MathUtil::MatrixToDegreeEulerAngles);
	luaState.set_function("PiecewiseProd", sol::overload(
		sol::resolve<Vector2(const Vector2&, const Vector2&)>(&MathUtil::PiecewiseProd),
		sol::resolve<Vector3(const Vector3&, const Vector3&)>(&MathUtil::PiecewiseProd),
		sol::resolve<Vector4(const Vector4&, const Vector4&)>(&MathUtil::PiecewiseProd)
		)
	);
	luaState.set_function("RandF", sol::overload(sol::resolve<float(void)> (&MathUtil::RandF), sol::resolve<float(float, float)>(&MathUtil::RandF)));
	luaState.set_function("RandI", &MathUtil::RandI);
	
	luaState.set_function("TransformVector", sol::overload(
		sol::resolve<Vector2(const Vector2&, const Matrix&)>(&Vector2::Transform),
		sol::resolve<Vector3(const Vector3&, const Matrix&)>(&Vector3::Transform),
		sol::resolve<Vector4(const Vector4&, const Matrix&)>(&Vector4::Transform)
		)
	);

	luaState.set_function("TransformVectorNormal", sol::overload(
		sol::resolve<Vector2(const Vector2&, const Matrix&)>(&Vector2::TransformNormal),
		sol::resolve<Vector3(const Vector3&, const Matrix&)>(&Vector3::TransformNormal)
	)
	);

#pragma endregion


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
		"len", &Vector2::Length,
		"len2", &Vector2::LengthSquared,
		"normalize", sol::overload(
			sol::resolve<void(void)>(&Vector2::Normalize)),
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
		"len", &Vector3::Length, 
		"len2", &Vector3::LengthSquared,
		"normalize", sol::overload(
			sol::resolve<void(void)>(&Vector3::Normalize)),
		// we use 'sol::resolve' cause other operator+ can exist in the (global) namespace
		sol::meta_function::addition, sol::resolve<Vector3(Vector3 const&, Vector3 const&)>(operator+),
		sol::meta_function::subtraction, sol::resolve<Vector3(Vector3 const&, Vector3 const&)>(operator-),
		sol::meta_function::multiplication, sol::resolve<Vector3(float, Vector3 const&)>(operator*),
		sol::meta_function::multiplication, sol::resolve<Vector3(Vector3 const&, float)>(operator*)
	);
	luaState.new_usertype<Vector4>
		(
			"Vector4",
			sol::constructors<Vector4(), Vector4(float), Vector4(float x, float y, float z, float w), Vector4(Vector4 const&) >(),
			"x", &Vector4::x,
			"y", &Vector4::y,
			"z", &Vector4::z,
			"w", &Vector4::w,
			"dot", &Vector4::Dot,
			"len", &Vector4::Length,
			"len2", &Vector4::LengthSquared,
			// we use 'sol::resolve' cause other operator+ can exist in the (global) namespace
			sol::meta_function::addition, sol::resolve<Vector4(Vector4 const&, Vector4 const&)>(operator+),
			sol::meta_function::subtraction, sol::resolve<Vector4(Vector4 const&, Vector4 const&)>(operator-),
			sol::meta_function::multiplication, sol::resolve<Vector4(float, Vector4 const&)>(operator*),
			sol::meta_function::multiplication, sol::resolve<Vector4(Vector4 const&, float)>(operator*)
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
		"Translation", sol::overload(sol::resolve<void(const Vector3&)>(&Matrix::Translation), sol::resolve<Vector3()const>(&Matrix::Translation)),
		sol::meta_function::addition, sol::resolve<Matrix(Matrix const&, Matrix const&)>(operator+),
		sol::meta_function::subtraction, sol::resolve<Matrix(Matrix const&, Matrix const&)>(operator-),
		sol::meta_function::multiplication, sol::resolve<Matrix(float, Matrix const&)>(operator*),
		sol::meta_function::multiplication, sol::resolve<Matrix(Matrix const&, Matrix const&)>(operator*)
	);

	////////////////////
	////  MANAGERS  ////
	////////////////////
	//SYSTEM MANAGER
	luaState.new_usertype<SystemManager>
	(
		"SystemManager"
	);

	luaState.new_usertype<MomentShadowMapRendering>
	(
			"MomentShadowMapRendering",
			"SetFocusPoint", &MomentShadowMapRendering::SetFocusPoint
		);
	luaState.new_usertype<AppRenderer>
	(
		"AppRenderer",
		"GetMomentShadowMap", &AppRenderer::GetMomentShadowMap,
		"RegisterTextFontInstance", sol::overload(
			sol::resolve<void(const std::string& , uint32_t ,
				const Vector2& , const Vector3& , const Vector3&, float )>(&AppRenderer::RegisterTextFontInstance) )
	);


	//GAMEOBJECTMANAGER
	luaState.new_usertype<GameObjectManager>
	(
		"GameObjectManager",
		"FindGameObjectById", &GameObjectManager::FindGameObjectById,
		"FindGameObject", &GameObjectManager::FindGameObject
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
		"Destroy", &GameObject::Destroy,
		//Static Instantiation Methods
		"Instantiate", sol::overload(
			sol::resolve<GameObject*(GameObjectManager *)>(&GameObject::Instantiate),
			sol::resolve<GameObject*(GameObjectManager*, std::string const&)>(&GameObject::Instantiate)),
		//Get scripted and engine components
		"GetCustomComp",          &GameObject::LuaGetCustomComponent,
		"GetRigidbodyComp",       &GameObject::GetComponent<RigidbodyComponent>,
		"GetTriggerComp",         &GameObject::GetComponent<TriggerComponent>,
		"GetRendererComp",        &GameObject::GetComponent<RendererComponent>,
		"GetMeshComp",            &GameObject::GetComponent<MeshComponent>,
		"GetLightComp",           &GameObject::GetComponent<LightComponent>,
		"GetHaloEffectComp",	  &GameObject::GetComponent<HaloEffectComponent>,
		"GetCameraComp",          &GameObject::GetComponent<CameraComponent>,
		"GetParticleEmitterComp", &GameObject::GetComponent<ParticleEmitterComponent>,
		"GetAnimationComp",		  &GameObject::GetComponent<AnimationComponent>,
		"GetTransformComp",       &GameObject::GetComponent<TransformComponent>,
		"GetUiComp",			  &GameObject::GetComponent<UIComponent>,
		"GetFollowCurvesPathComp",&GameObject::GetComponent<FollowCurvesPathComponent>,
		
		//Add scripted and engine components
		"AddCustomComp",          &GameObject::LuaAddCustomComponent,
		"AddRigidbodyComp",       &GameObject::AddComponent<RigidbodyComponent>,
		"AddTriggerComp",         &GameObject::AddComponent<TriggerComponent>,
		"AddRendererComp",        &GameObject::AddComponent<RendererComponent>,
		"AddMeshComp",            &GameObject::AddComponent<MeshComponent>,
		"AddLightComp",           &GameObject::AddComponent<LightComponent>,
		"AddCameraComp",          &GameObject::AddComponent<CameraComponent>,
		"AddParticleEmitterComp", &GameObject::AddComponent<ParticleEmitterComponent>,
		"AddAnimationComp",       &GameObject::AddComponent<AnimationComponent>,
		"AddTransformComp",       &GameObject::AddComponent<TransformComponent>
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
		"GetForward", &TransformComponent::GetForward,
		"GetRight", &TransformComponent::GetRight,
		"GetPosition", &TransformComponent::GetPosition,
		"GetRotation", &TransformComponent::GetRotation,
		"GetScale", &TransformComponent::GetScale,
		"GetPositionNormalized", &TransformComponent::GetPositionNormalized,
		"GetScaleNormalized", &TransformComponent::GetScaleNormalized,
		"GetModel", &TransformComponent::GetModel,
		"GetRotationMatrix", &TransformComponent::GetRotationMatrix,
		"GetScaleMatrix", &TransformComponent::GetScaleMatrix,
		//Setters
		"SetLocalPosition", sol::overload(
			sol::resolve<void(Vector3 const&)>(&TransformComponent::SetLocalPosition),
			sol::resolve<void(float, float, float)>(&TransformComponent::SetLocalPosition)),
		"SetLocalRotation", &TransformComponent::SetLocalRotation,
		"SetLocalRotationMatrix", &TransformComponent::SetLocalRotationMatrix
	);

	// UIComponent
	luaState.new_usertype<UIComponent>
		("UIComponent",
			"GetButtonIndex", &UIComponent::GetButtonIndex,
			"GetParent", &UIComponent::GetParent,
			"GetParentName", &UIComponent::GetParentName,
			"GetChild", &UIComponent::GetChild,
			"GetNumberOfChildren", &UIComponent::GetNumberOfChildren,


			"GetRenderEnable", &UIComponent::GetRenderEnable,
			"SetRenderEnable", sol::overload(
				sol::resolve<void(bool)>(&UIComponent::SetRenderEnable)),
			"GetChild", &UIComponent::GetChild,
			"GetInitialPosition", &UIComponent::GetInitialPosition,
			"GetFinalPosition", &UIComponent::GetFinalPosition,
			"GetInitialPositionNormalized", &UIComponent::GetInitialPositionNormalized,
			"GetFinalPositionNormalized", &UIComponent::GetFinalPositionNormalized,
			"SetInitialPosition", sol::overload(
				sol::resolve<void(Vector3 const&)>(&UIComponent::SetInitialPosition)),
			"SetFinalPosition", sol::overload(
				sol::resolve<void(Vector3 const&)>(&UIComponent::SetFinalPosition)),
			"SetTouchedScale", sol::overload(
				sol::resolve<void(Vector3 const&)>(&UIComponent::SetTouchedScale)),
			"SetUnTouchedScale", sol::overload(
				sol::resolve<void(Vector3 const&)>(&UIComponent::SetUnTouchedScale)),
			"GetTouchedScale", &UIComponent::GetTouchedScale,
			"GetUnTouchedScale", &UIComponent::GetUnTouchedScale,
			"GetUnTouchedScaleNormalized", &UIComponent::GetUnTouchedScaleNormalized,
			"GetTouchedScaleNormalized", &UIComponent::GetTouchedScaleNormalized,
			"GetDeltaTime", &UIComponent::GetDeltaTime,
			"GetVelocity", &UIComponent::GetVelocity,
			"GetInitialRotation", &UIComponent::GetInitialRotation,
			"GetFinalRotation", &UIComponent::GetFinalRotation,
			"GetRotationRate", &UIComponent::GetRotationRate,
			"SetTextScale", &UIComponent::SetTextScale,
			"GetTextScale", &UIComponent::GetTextScale,
			"SetText", &UIComponent::SetText,
			"GetText", &UIComponent::GetText
			);



	//ANIMATION
	luaState.new_usertype<AnimationComponent>
	(
		"AnimationComponent",
		//"SwitchAnimation", &AnimationComponent::SwitchAnimation,
		//Interface for animator (state machine)
		"CreateState", sol::overload
		(
			sol::resolve<AnimState*(std::string, std::string)>(&AnimationComponent::CreateState),
			sol::resolve<AnimState*(std::string, std::string, float)>(&AnimationComponent::CreateState)
		),
		"SetEntryState", &AnimationComponent::SetEntryState,
		"SetTrigger", &AnimationComponent::SetTrigger,
		"AddAnimEvent", &AnimationComponent::AddAnimEvent,
		"AddAnimEndEvent", &AnimationComponent::AddAnimEndEvent,
		"AddInterruptEvent", &AnimationComponent::AddInterruptEvent
	);

	//RIGIDBODY
	luaState.new_usertype<RigidbodyComponent>
	(
		"RigidbodyComponent",
		"GetVelocity", &RigidbodyComponent::GetVelocity,
		"SetVelocity", &RigidbodyComponent::SetVelocity,
		"GetAngularVelocity", &RigidbodyComponent::GetAngularVelocity,
		"SetAngularVelocity", &RigidbodyComponent::SetAngularVelocity,
		"GetMass", &RigidbodyComponent::GetMass,
		"SetMass", &RigidbodyComponent::SetMass,

		"GetCollisionMask", &RigidbodyComponent::GetCollisionMask,
		"OnCollision", &RigidbodyComponent::m_onCollision
	);

	//TRIGGER
	luaState.new_usertype<TriggerComponent>
		(
			"TriggerComponent",
			"GetScale", &TriggerComponent::GetScale,
			"SetScale", &TriggerComponent::SetScale,
			"GetOffset", &TriggerComponent::GetOffset,
			"SetOffset", &TriggerComponent::SetOffset,

			"SetCollisionMask", &TriggerComponent::SetCollisionMask,
			"GetCollisionMask", &TriggerComponent::GetCollisionMask,
			"OnEnter", &TriggerComponent::m_onEnter,
			"OnExit", &TriggerComponent::m_onExit
			);

	//RENDERER
	luaState.new_usertype<RendererComponent>
	(
		"RendererComponent",
		"SetEnableRendering", &RendererComponent::SetEnableRendering
	);

	//MESH
	luaState.new_usertype<MeshComponent>
	(
		"MeshComponent"
	);

	//LIGHT
	luaState.new_usertype<LightComponent>
	(
		"LightComponent",
		"SetColor", &LightComponent::SetColor
	);

	//LIGHT
	luaState.new_usertype<HaloEffectComponent>
	(
		"HaloEffectComponent",
		"SetColor", &HaloEffectComponent::SetColor
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
		"SetFOV", &Camera::SetFOV,
		"GetWidthHeight", &Camera::GetWidthHeight,
		"IncreaseFOV", &Camera::IncreaseFOV,
		"DecreaseFOV", &Camera::DecreaseFOV,
		"GetFOV", &Camera::GetFOV,
		"GetForward", &Camera::GetForward,
		"GetUp", &Camera::GetUp,
		"GetViewMatrix", &Camera::GetViewMatrix,
		"GetViewProjectionMatrix", &Camera::GetViewProjectionMatrix,
		"GetProjectionMatrix", &Camera::GetProjectionMatrix,
		"GetInvViewMatrix", &Camera::GetInvViewMatrix,
		"GetInvViewMatrixCopy", &Camera::GetInvViewMatrixCopy,
		"GetScreenWidth", &Camera::GetScreenWidth,
		"GetScreenHeight", &Camera::GetScreenHeight,
		"GetRight", &Camera::GetRight,
		"SetLook", &Camera::SetLook,
		"SetCameraPosition", sol::overload(
			sol::resolve<void(Vector3 const&)>(&Camera::SetCameraPosition),
			sol::resolve<void(float, float, float)>(&Camera::SetCameraPosition)),
		"GetCameraPosition", &Camera::GetCameraPosition
	);

	luaState.new_usertype<ParticleEmitterComponent>
		(
			"ParticleEmitterComponent",
			"Emit", &ParticleEmitterComponent::Emit,
			"SetEmitterSpreadAngleYaw", &ParticleEmitterComponent::SetEmitterSpreadAngleYaw,
			"SetEmitterSpreadAnglePitch", &ParticleEmitterComponent::SetEmitterSpreadAnglePitch,
			"SetEmitterDirection", &ParticleEmitterComponent::SetEmitterDirection
			);

	luaState.new_usertype<FollowCurvesPathComponent>(
			"FollowCurvesPathComponent",
			"SetBeforeInitCurveGameObjectToFollow", &FollowCurvesPathComponent::SetBeforeInitCurveGameObjectToFollow,
			"SetCurveGameObjectToFollow", &FollowCurvesPathComponent::SetCurveGameObjectToFollow,
			"SetMotionSpeed", &FollowCurvesPathComponent::SetMotionSpeed,
			"GetMotionSpeed", &FollowCurvesPathComponent::GetMotionSpeed,
			"SetEnableMotionAlongPath", &FollowCurvesPathComponent::SetEnableMotionAlongPath,
			"IsMotionAlongPathEnabled", &FollowCurvesPathComponent::IsMotionAlongPathEnabled,
			"SetEnableMotionOrientation", &FollowCurvesPathComponent::SetEnableMotionOrientation,
			"IsMotionOrientationEnabled", &FollowCurvesPathComponent::IsMotionOrientationEnabled,
			"ResetMotionTime", &FollowCurvesPathComponent::ResetMotionTime,
			"SetOffsetFollowPathPosition", sol::overload(
				sol::resolve<void(float, float, float)>(&FollowCurvesPathComponent::SetOffsetFollowPathPosition),
				sol::resolve<void(const Vector3&)>(&FollowCurvesPathComponent::SetOffsetFollowPathPosition)),
			"GetOffSetFollowPathPosition", &FollowCurvesPathComponent::GetOffSetFollowPathPosition
		);

	///////////////////////
	// ANIMATION HELPERS //
	///////////////////////
	luaState.new_usertype<Animation>
	(
		"Animation",
		"AddAnimEvent", &Animation::AddAnimEvent,
		"AddAnimEndEvent", &Animation::AddAnimEndEvent
	);

	luaState.new_usertype<AnimState>
	(
		"AnimState",
		"SetTransition", &AnimState::SetTransition
	);
}