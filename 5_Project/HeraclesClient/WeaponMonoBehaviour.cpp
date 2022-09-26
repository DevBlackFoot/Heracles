#include "pch.h"
#include "WeaponMonoBehaviour.h"
#include "BoxCollider.h"

#include "MonsterState.h"
#include "PlayerStateMachine.h"
#include "BossTree.h"

namespace Heracles
{
	namespace Weapon
	{
		WeaponMonoBehaviour::WeaponMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
		}
		void WeaponMonoBehaviour::Start()
		{
			this->damage = this->GetGameObject()->GetParent()->GetComponent<Player::PlayerStateMachine>()->attackDamage;
		}

		void WeaponMonoBehaviour::PreUpdate(float tick)
		{
			this->damage = this->GetGameObject()->GetParent()->GetComponent<Player::PlayerStateMachine>()->attackDamage;
			this->isattackState = false;
		}

		void WeaponMonoBehaviour::LateUpdate(float tick)
		{

		}

		void WeaponMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			auto obj = other->GetGameObject();
			if (obj == nullptr)
			{
				return;
			}
			else
			{
				if (this->isattackState)
				{
					if (obj->GetName().compare(TEXT("Monster")) == 0 || obj->GetName()==TEXT("BigMonster"))
					{
						obj->GetComponent<Monster::MonsterState>()->GetDamage(this->damage);
					}
					else if (obj->GetName() == TEXT("Boss"))
					{
						obj->GetComponent<Boss::BossTree>()->GetDamage(this->damage);
					}
				}

			}
		}

		void WeaponMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			auto obj = other->GetGameObject();
			if (obj == nullptr)
			{
				return;
			}
			else
			{
				if (this->isattackState)
				{
					if (obj->GetName().compare(TEXT("Monster")) == 0 || obj->GetName() == TEXT("BigMonster"))
					{
						obj->GetComponent<Monster::MonsterState>()->GetDamage(this->damage);
						
					}
					else if (obj->GetName() == TEXT("Boss"))
					{
						obj->GetComponent<Boss::BossTree>()->GetDamage(this->damage);
					}
				}

			}
		}
		void WeaponMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
		}
	}
}

