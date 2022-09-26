#pragma once
#include"MonoBehaviour.h"

namespace Heracles
{
	namespace Boss
	{
		class BossMonoBehaviour : public MonoBehaviour
		{
		public:

			//close attack
			bool attacking = false;

			float attackDamage = 5;

		public:
			BossMonoBehaviour(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;
			virtual void PreUpdate(float tick) override;


			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

		};
	}
}

