#include "pch.h"
#include "MonsterMonoBehaviour.h"

#include "MonsterState.h"


#include "PlayerStateMachine.h"
#include "PlayerCollider.h"

#include "UIManager.h"

namespace Heracles
{
	namespace Monster
	{
		const static float playerStopTime = 5.f;

		MonsterMonoBehaviour::MonsterMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
			debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");
		}
		void MonsterMonoBehaviour::Awake()
		{
		}
		void MonsterMonoBehaviour::PreUpdate(float tick)
		{

		}
		void MonsterMonoBehaviour::LateUpdate(float tick)
		{
			if (this->detectPlayer)
			{
				UIManager::GetInstance()->SetTextUIText("PlayerDetected", "PlayerDetected : True -> following", debugCanvas);
				UIManager::GetInstance()->SetTextUIColor("PlayerDetected", Vector(1.f,0.f,0.f), debugCanvas);
			}
		}
		void MonsterMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			auto player = std::dynamic_pointer_cast<Player::PlayerCollider>(other);
			if (player != nullptr )
			{
				this->detectPlayer = true;

				auto state = this->GetGameObject()->GetComponent<MonsterState>();
				if (state->currState != State::ATTACK)
				{
					state->enterAttackState = true;
					state->currEvent = Event::DETECT;
				}
			}
		}

		void MonsterMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			auto player = std::dynamic_pointer_cast<Player::PlayerCollider>(other);
			if (player != nullptr)
			{
				auto state = this->GetGameObject()->GetComponent<MonsterState>();
				if(state->currState != State::ATTACK)
				{
					state->enterAttackState = true;
					state->currEvent = Event::DETECT;
				}
			}
		}

		void MonsterMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
		}
	}
}