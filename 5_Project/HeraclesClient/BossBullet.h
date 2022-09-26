#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Boss
	{
		class BossBullet : public MonoBehaviour
		{
		private:
			float damage = 5.f;
			float speed = 25.f;

			Vector initialPos;
			Vector direction;

			bool activatedThisFrame = false;

			std::shared_ptr<Transform> transform;

		public:
			BossBullet(std::weak_ptr<GameObject> obj);

			virtual void Start() override;

			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

			inline void Activate() { this->activatedThisFrame = true; }
			inline void SetDirection(Vector dir) { this->direction = dir; }
			inline void SetInitialPos(Vector pos) { this->initialPos = pos; }
			inline void SetSpeed(float speed) { this->speed = speed; }
		};
	}
}

