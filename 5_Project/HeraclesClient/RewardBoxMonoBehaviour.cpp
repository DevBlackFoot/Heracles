#include "pch.h"
#include "RewardBoxMonoBehaviour.h"

#include "GameState.h"
#include "ColliderBase.h"
#include "Animator.h"
#include "MemoryGem.h"
#include "Money.h"
#include "IncreaseHp.h"

#include "SoundManager.h"

namespace Heracles
{
	RewardBoxMonoBehaviour::RewardBoxMonoBehaviour(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
	{
	}

	void RewardBoxMonoBehaviour::Update(float tick)
	{

	}

	void RewardBoxMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		if (other->GetGameObject()->GetName() == TEXT("Player"))
		{
			if (this->triggered == false)
			{
				SoundManager::GetInstance()->PlayEffect("ItemDrop");

				this->GetGameObject()->GetComponent<Animator>()->Play(type);
				this->reward->SetGameObjectEnable(true);
				Vector t = this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();

				if (this->reward->GetName() == TEXT("MemoryGem"))
				{
					this->reward->GetComponent<MemoryGem>()->initialPos = t + Vector(0.f, 0.f, 3.f);
				}
				else if (this->reward->GetName() == TEXT("Money"))
				{
					this->reward->GetComponent<Money>()->initialPos = t + Vector(3.f, 0.f, 0.f);
				}
				else if (this->reward->GetName() == TEXT("Hp"))
				{
					this->reward->GetComponent<IncreaseHp>()->initialPos = t + Vector(3.f, 0.f, 0.f);
				}
				this->triggered = true;
			}
		}
	}
	void RewardBoxMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{

	}
	void RewardBoxMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
	}
}