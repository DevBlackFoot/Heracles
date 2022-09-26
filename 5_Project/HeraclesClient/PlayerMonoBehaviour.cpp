#include "pch.h"
#include "PlayerMonoBehaviour.h"

#include "ColliderBase.h"
#include "MonsterState.h"
#include "BossTree.h"
namespace Heracles
{
	namespace Player
	{
		PlayerMonoBehaviour::PlayerMonoBehaviour(std::weak_ptr<GameObject> obj)
			:MonoBehaviour(obj)
		{

		}
		void PlayerMonoBehaviour::Awake()
		{

		}
		void PlayerMonoBehaviour::PreUpdate(float tick)
		{

		}
		void PlayerMonoBehaviour::LateUpdate(float tick)
		{

		}
		void PlayerMonoBehaviour::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{

		}
		void PlayerMonoBehaviour::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{

		}
		void PlayerMonoBehaviour::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
		}
	}
}