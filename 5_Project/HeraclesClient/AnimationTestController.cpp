#include "pch.h"
#include "AnimationTestController.h"

#include "GameObject.h"
#include "InputManager.h"


Heracles::AnimationTestController::AnimationTestController(std::weak_ptr<GameObject> gameObj)
	: ComponentBase(gameObj, ComponentType::MONOBEHAVIOR)
{

}

void Heracles::AnimationTestController::Awake()
{
	// Æ÷ÀÎÅÍ Ä³½Ì
	animator = this->GetGameObject()->GetComponent<Animator>();
	input = InputManager::GetInstance();

}

void Heracles::AnimationTestController::LateUpdate(float tick)
{
	if(input->GetInputState(VK_UP, KeyState::DOWN))
		animator->Play("Idle");

	if(input->GetInputState(VK_DOWN, KeyState::DOWN))
		animator->Play("Walk");

}
