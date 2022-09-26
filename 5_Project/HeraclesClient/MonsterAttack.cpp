#include "pch.h"
#include "MonsterAttack.h"

#include "ColliderBase.h"
#include "MonsterState.h"

#include "GameState.h"
#include "PlayerStateMachine.h"

namespace Heracles
{
	namespace Monster
	{
		MonsterAttack::MonsterAttack(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
		}

		void MonsterAttack::Start()
		{
			this->monsterAttackPower = this->GetGameObject()->GetParent()->GetComponent<MonsterState>()->attackPower;
		}
		void MonsterAttack::PreUpdate(float tick)
		{
		}
		void MonsterAttack::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			auto obj = other->GetGameObject();
			if (obj != nullptr)
			{
				if (this->attacking == true)
				{
					if (obj->GetName() == TEXT("Player"))
					{
						this->attacking = false;
						GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->monsterAttackPower);
					}
				}
			}
		}

		void MonsterAttack::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			if (this->attacking == true)
			{
				if (other->GetGameObject()->GetName() == TEXT("Player"))
				{
					this->attacking = false;
					GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->monsterAttackPower);

					SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(100.0f);
				}
			}
			
		}
		void MonsterAttack::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
		}
	}
}