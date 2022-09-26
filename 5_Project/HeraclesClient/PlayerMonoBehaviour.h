#pragma once
#include "Monobehaviour.h"

namespace Heracles
{
	namespace Player
	{
		class PlayerMonoBehaviour : public MonoBehaviour
		{
		private:

		public:
			PlayerMonoBehaviour(std::weak_ptr<GameObject> obj);
			virtual void Awake() override;
			virtual void PreUpdate(float tick) override;
			virtual void LateUpdate(float tick) override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;
		};
	}
}

