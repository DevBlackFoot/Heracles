#include "pch.h"
#include "Entrance.h"

#include "GameState.h"
#include "ColliderBase.h"
#include "PlayerStateMachine.h"
#include "SceneManager.h"
#include "SkinnedMeshRenderer.h"
#include "Animator.h"
#include "SoundManager.h"

namespace Heracles
{

	bool Entrance::CompareSceneName(tstring name)
	{
		if (name == this->nextSceneName)
		{
			return true;
		}
		return false;
	}




	Entrance::Entrance(std::weak_ptr<GameObject> obj)
		:MonoBehaviour(obj)
	{
	}

	void Entrance::Start()
	{
	}

	void Entrance::PreUpdate(float tick)
	{
		if (!this->GetGameObject()->GetComponent<Animator>()->IsPlaying() && this->triggerd)
		{
			this->animationPlayed = true;
			this->startTimer = true;
		}

		if (this->startTimer)
		{
			GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>()->GetEventData()->externalEventType = Player::ExternalEvent::MOVE;
			this->timer -= tick;
		}
	}

	void Entrance::Update(float tick)
	{
		if (this->animationPlayed&& this->triggerd)
		{
			if (this->timer <= 0.f)
			{
				GameState::GetInstance()->interacting = false;
				SoundManager::GetInstance()->PlayWalkSound("PlayerRun", true);
				SceneManager::GetInstance()->LoadScene(this->nextSceneName);
			}
			else
			{
				Vector dir = this->GetGameObject()->GetComponent<Transform>()->GetLook();
				auto player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>();
				player->LookAt(player->GetWorldPosition() + dir);
				player->SetPosition(player->GetWorldPosition() + dir*tick*23.f);
			}
		}
	}

	void Entrance::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
	{
		if (GameState::GetInstance()->mapCleared)
		{
			if (other->GetGameObject()->GetName() == TEXT("Player"))
			{
				GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>()->SavePlayerDataToGameState();
				GameState::GetInstance()->mapCleared = false;
				GameState::GetInstance()->exitType = this->entranceType;
				GameState::GetInstance()->mapMonster.erase(std::remove(GameState::GetInstance()->mapMonster.begin(), GameState::GetInstance()->mapMonster.end(), this->nextSceneName), GameState::GetInstance()->mapMonster.end());
				this->animationPlayed = false;
				this->triggerd = true;
				this->GetGameObject()->GetComponent<Animator>()->Play("DoorOpen");
				SoundManager::GetInstance()->PlayEffect("DoorOpen");
				GameState::GetInstance()->interacting = true;
			}
		}
	}

	void Entrance::OnTriggerStay(std::shared_ptr<ColliderBase> other)
	{
	}

	void Entrance::OnTriggerExit(std::shared_ptr<ColliderBase> other)
	{
	}
}