#include "pch.h"
#include "BossAttackMonoBehaviour.h"

#include "PlayerStateMachine.h"
#include "GameState.h"
#include "ColliderBase.h"

namespace Heracles
{
	namespace Boss
	{
		BossAttackMonoBehaviour::BossAttackMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
		}
		void BossAttackMonoBehaviour::Start()
		{
		}
		void BossAttackMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			tstring name = other->GetGameObject()->GetName();

			if (this->attacking == true)
			{
				if (name == TEXT("Player"))
				{
					other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->attackDamage);
					this->attacking = false;
				}
			}
		}
		void BossAttackMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			tstring name = other->GetGameObject()->GetName();
			if (this->attacking == true)
			{
				if (name == TEXT("Player"))
				{
					other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->attackDamage);
					this->attacking = false;
				}
			}
		}
		void BossAttackMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			tstring name = other->GetGameObject()->GetName();
			if (this->attacking == true)
			{
				if (name == TEXT("Player"))
				{
					other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->attackDamage);
					this->attacking = false;
				}
			}
		}
	}
}