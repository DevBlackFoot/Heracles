#pragma once

#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Boss
	{
		class BossAttackMonoBehaviour : public MonoBehaviour
		{
		public:
			//����, �뽬���ݿ� ����� �̺�Ʈ
			bool attacking = false;

			float attackDamage = 10;

		public:
			BossAttackMonoBehaviour(std::weak_ptr<GameObject> obj);
			virtual void Start() override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
		};

	}
}

