#include "pch.h"
#include "PlayerCollider.h"
#include "PlayerState.h"

//#include "MonsterCollider.h"
#include "MonsterState.h"

#include "PlayerStateMachine.h"
namespace Heracles
{
	using namespace Monster;
	namespace Player
	{
		PlayerCollider::PlayerCollider(std::weak_ptr<GameObject> obj)
			:BoxCollider(obj)
		{
		}
	}

}
