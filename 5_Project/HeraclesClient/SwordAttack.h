#pragma once
#include "MonoBehaviour.h"

namespace Heracles
{
	namespace Player
	{

		class SwordAttack : public MonoBehaviour
		{
		public:
			float attackPower = 20.f;
			bool attacked = false;

			Vector startPoint;
			Vector direction;
			float speed = 30.f;
			
			bool activated = false;

			float timer = 0.f;
			
		public:
			SwordAttack(std::weak_ptr<GameObject> obj);

			virtual void Start() override;
			virtual void PreUpdate(float tick) override;

			virtual void Update(float tick) override;

			virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
			virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

			void SetStartPoint(Vector pos) { this->startPoint = pos; }
			void SetSpeed(float t) { this->speed = t; }
		};
	}
}

