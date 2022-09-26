#include "pch.h"
#include "Money.h"

#include "ColliderBase.h"
#include "PlayerStateMachine.h"
#include "SoundManager.h"

namespace Heracles
{
	Money::Money(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
	{
	}

	void Money::Start()
	{
		this->transform = this->GetGameObject()->GetComponent<Transform>();
	}

	void Money::Update(float tick)
	{
		static float theta = 0.f;
		float yPos = std::sinf(theta);

		Vector pos = Vector(this->initialPos.x, yPos + 1.f, this->initialPos.z);
		this->transform->SetPosition(pos, Space::LOCAL);
		theta += 5.f * tick;
	}

	void Money::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		if (other->GetGameObject()->GetName() == TEXT("Player"))
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(1, 3);
			SoundManager::GetInstance()->PlayEffect("GetMoney");

			other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->money += dist(gen);
			this->GetGameObject()->SetGameObjectEnable(false);
		}
	}
	void Money::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
	}
	void Money::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
	}
}