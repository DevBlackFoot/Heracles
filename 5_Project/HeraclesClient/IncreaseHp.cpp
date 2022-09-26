#include "pch.h"
#include "IncreaseHp.h"

#include "ColliderBase.h"
#include "PlayerStateMachine.h"
#include "SoundManager.h"

namespace Heracles
{
	IncreaseHp::IncreaseHp(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
	{
	}
	void IncreaseHp::Start()
	{
		this->transform = this->GetGameObject()->GetComponent<Transform>();
	}
	void IncreaseHp::Update(float tick)
	{
		static float theta = 0.f;
		float yPos = std::sinf(theta);

		Vector pos = Vector(this->initialPos.x, yPos + 1.f, this->initialPos.z);
		this->transform->SetPosition(pos, Space::LOCAL);
		theta += 5.f * tick;
	}
	void IncreaseHp::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		if (other->GetGameObject()->GetName() == TEXT("Player"))
		{
			float target = other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->currHp + this->hp;
			
			if (target >= other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->maxHp)
			{

			}
			else
			{
				other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->currHp += this->hp;
			}

			SoundManager::GetInstance()->PlayEffect("GetHpItem");
			this->GetGameObject()->SetGameObjectEnable(false);
		}
	}
	void IncreaseHp::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
	}
	void IncreaseHp::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
	}
}