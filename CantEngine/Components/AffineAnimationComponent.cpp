
#include "AffineAnimationComponent.h"
#include "GameObjects/GameObject.h"
#include "Managers/ResourceManager.h"
#include "Graphics/D3D11_Renderer.h"
#include "Managers/GameObjectManager.h"

unsigned const AffineAnimationComponent::static_type = BaseComponent::numberOfTypes++;


RTTR_REGISTRATION
{
	rttr::registration::class_<AffineAnimationComponent>("AffineAnimationComponent")
		.constructor<GameObject*>()(rttr::policy::ctor::as_raw_ptr)
		.property("InitialPosition", &AffineAnimationComponent::initialPos)
		.property("FinalPosition", &AffineAnimationComponent::finalPos)
		.property("DeltaTime", &AffineAnimationComponent::deltaTime)
		.method("Init", &AffineAnimationComponent::Init);
}

AffineAnimationComponent::AffineAnimationComponent(GameObject * owner) :BaseComponent(owner, static_type)
{
}

AffineAnimationComponent::~AffineAnimationComponent()
{
}

void AffineAnimationComponent::Init(ResourceManager * resMgr, DXRenderer * dxrenderer)
{
}

void AffineAnimationComponent::Begin(GameObjectManager * goMgr)
{
}

Vector3 AffineAnimationComponent::GetInitialPosition()
{
	return initialPos;
}

Vector3 AffineAnimationComponent::GetFinalPosition()
{
	return finalPos;
}
Vector3 AffineAnimationComponent::GetVelocity()
{
	Vector3 result = finalPos - initialPos;
	result.Normalize();
	return result;
}
float AffineAnimationComponent::GetDeltaTime()
{
	return deltaTime;
}
