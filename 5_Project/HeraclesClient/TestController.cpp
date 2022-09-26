#include "pch.h"
#include "TestController.h"

#include "PlayerStateMachine.h"
#include "GameState.h"
#include "SceneManager.h"
namespace Heracles
{
	const float speed = 20;
	TestController::TestController(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj,ComponentType::MONOBEHAVIOR)
	{
	}
	TestController::~TestController()
	{
	}

	void TestController::Awake()
	{
		this->transform = this->GetGameObject()->GetComponent<Transform>();
		this->input = GameEngineSpace::InputManager::GetInstance();
	}

	void TestController::Update(float tick)
	{
		//상호작용 입력은 여기서 받는다

		if (GameState::GetInstance()->interacting)
		{
			return;
		}

		Player::EventBase* e = this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetEventData();

		if (this->input->GetInputState(' ', KeyState::DOWN))
		{
			if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->dodgeTimer < 0.001f)
			{
				e->externalEventType = Player::ExternalEvent::DODGE;
				return;
			}
		}

		if (this->input->GetInputState('G', KeyState::DOWN))
		{
			if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->attackTimer < 0.001f)
			{
				e->externalEventType = Player::ExternalEvent::ATTACK;
				return;
			}
		}

		if (this->input->GetInputState('W', KeyState::STAY))
		{
			if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->canMove)
			{
				e->externalEventType = Player::ExternalEvent::MOVE;
				this->transform->SetPosition(this->transform->GetWorldPosition() + Vector(0.0f, 0.0f, 1.0f) * tick * speed);
			}
		}
		else if (this->input->GetInputState('A', KeyState::STAY))
		{
			if(this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->canMove)
			{
				e->externalEventType = Player::ExternalEvent::MOVE;
				this->transform->SetPosition(this->transform->GetWorldPosition() + Vector(-1.0f, 0.0f, 0.0f) * tick * speed);
			}
		}
		else if (this->input->GetInputState('S', KeyState::STAY))
		{
			if(this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->canMove)
			{
				e->externalEventType = Player::ExternalEvent::MOVE;
				this->transform->SetPosition(this->transform->GetWorldPosition() + Vector(0.0f, 0.0f, -1.0f) * tick * speed);
			}
		}
		else if (this->input->GetInputState('D', KeyState::STAY))
		{
			if(this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->canMove)
			{
				e->externalEventType = Player::ExternalEvent::MOVE;
				this->transform->SetPosition(this->transform->GetWorldPosition() + Vector(1.0f, 0.0f, 0.0f) * tick * speed);
			}
		}
		else if (this->input->GetInputState('F', KeyState::STAY))
		{
			if (this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->canMove)
			{
				e->externalEventType = Player::ExternalEvent::MOVE;
				this->transform->AddRotation(Vector(0.f, 0.02f, 0.f));
			}
		}
		else
		{
			e->externalEventType = Player::ExternalEvent::NONE;
		}
	}
	void TestController::LateUpdate(float tick)
	{
		//if (InputManager::GetInstance()->GetInputState(VK_ESCAPE, KeyState::DOWN))
		//{
		//	this->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->SavePlayerDataToGameState();
		//	SceneManager::GetInstance()->LoadScene(TEXT("Test"));
		//}
	}
}
