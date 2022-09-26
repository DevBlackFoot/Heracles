#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Monster
	{
		class MonsterAttack : public MonoBehaviour
		{
		public:
			bool attacking = false;
			float monsterAttackPower;
		public:
			MonsterAttack(std::weak_ptr<GameObject> obj);
			virtual void Start() override;

			virtual void PreUpdate(float tick) override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other);
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other);
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other);

		};
	}
}

