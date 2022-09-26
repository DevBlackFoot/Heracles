#include "pch.h"
#include "MonsterDirection.h"

#include "ColliderBase.h"
#include "MonsterState.h"

namespace Heracles
{
	namespace Monster
	{
		void MonsterDirection::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetTag() == TEXT("Obstacle"))
			{
				this->GetGameObject()->GetParent()->GetComponent<MonsterState>()->wall = other->GetGameObject();
			}
		}
		void MonsterDirection::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetTag() == TEXT("Obstacle"))
			{
				this->GetGameObject()->GetParent()->GetComponent<MonsterState>()->wall = other->GetGameObject();
			}
		}
		void MonsterDirection::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetTag() == TEXT("Obstacle"))
			{
				this->GetGameObject()->GetParent()->GetComponent<MonsterState>()->wall = other->GetGameObject();
			}
		}
	}
}