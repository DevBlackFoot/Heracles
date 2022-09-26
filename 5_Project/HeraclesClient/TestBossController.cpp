#include "pch.h"
#include "TestBossController.h"

#include "GameState.h"
#include "InputManager.h"
#include "Animator.h"

namespace Heracles
{
	namespace Boss
	{
		TestBossController::TestBossController(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj)
		{
		}
		void TestBossController::Awake()
		{
			this->input = InputManager::GetInstance();
		}
		void TestBossController::Start()
		{
		}
		void TestBossController::Update(float tick)
		{
			if (this->input->GetInputState('0', KeyState::DOWN))
			{
				this->GetGameObject()->GetComponent<Animator>()->Play("DoorOpen");
			}
			else if (this->input->GetInputState('L', KeyState::DOWN))
			{
				this->GetGameObject()->GetComponent<Animator>()->Play("DoorClose");
			}
		}
	}
}