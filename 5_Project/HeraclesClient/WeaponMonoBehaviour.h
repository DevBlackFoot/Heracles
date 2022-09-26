#pragma once

#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Weapon
	{
		class WeaponMonoBehaviour : public MonoBehaviour
		{
		private:
			float damage;
			bool isattackState = false;
		public:
			WeaponMonoBehaviour(std::weak_ptr<GameObject> obj);

			virtual void Start() override;
			virtual void PreUpdate(float tick) override;
			virtual void LateUpdate(float tick) override;


			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

			inline void SetAttackState(bool state) { this->isattackState = state; }
		};
	}
}

