#include "pch.h"
#include "MemoryGem.h"

#include "ColliderBase.h"
#include "PlayerStateMachine.h"
#include "SoundManager.h"

namespace Heracles
{
	MemoryGem::MemoryGem(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
	{
	}

	void MemoryGem::Start()
	{
		this->transform = this->GetGameObject()->GetComponent<Transform>();
	}

	void MemoryGem::Update(float tick)
	{
		static float theta = 0.f;
		float yPos = std::sinf(theta);
		
		Vector pos = Vector(this->initialPos.x, yPos+1.f, this->initialPos.z);
		this->transform->SetPosition(pos, Space::LOCAL);
		theta += 5.f*tick;
	}

	void MemoryGem::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		if (other->GetGameObject()->GetName() == TEXT("Player"))
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> dist(1, 3);

			SoundManager::GetInstance()->PlayEffect("GetMemoryGem");
			other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->memoryPieceNum += dist(gen);
			this->GetGameObject()->SetGameObjectEnable(false);
		}
	}
	void MemoryGem::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
	}
	void MemoryGem::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
	}
}