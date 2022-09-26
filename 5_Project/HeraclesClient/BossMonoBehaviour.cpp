#include "pch.h"
#include "BossMonoBehaviour.h"
#include "BossTree.h"

#include "PlayerStateMachine.h"

#include "PlayerCollider.h"
#include "UIManager.h"
#include "GameState.h"

namespace Heracles
{
	namespace Boss
	{
		BossMonoBehaviour::BossMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{
		}
		void BossMonoBehaviour::Awake()
		{

		}
		void BossMonoBehaviour::PreUpdate(float tick)
		{
			
		}

		void BossMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			tstring name = other->GetGameObject()->GetName();
			
			if (name == TEXT("Wall"))
			{
				this->GetGameObject()->GetComponent<BossTree>()->collisionEnterToWall = true;
			}

			//if (this->attacking == true)
			//{
			//	if (name == TEXT("Player"))
			//	{
			//		other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->attackDamage);
			//		this->attacking = false;
			//	}
			//}
		}

		void BossMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			tstring name = other->GetGameObject()->GetName();

		

			if (other->GetGameObject()->GetName() == TEXT("Wall"))
			{
				this->GetGameObject()->GetComponent<BossTree>()->collisionEnterToWall = false;
			}
		}
		void BossMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetName() == TEXT("Wall"))
			{
				this->GetGameObject()->GetComponent<BossTree>()->collisionEnterToWall = false;
			}

		}
	}
}