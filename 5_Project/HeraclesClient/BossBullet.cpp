#include "pch.h"
#include "BossBullet.h"

#include "ColliderBase.h"
#include "PlayerStateMachine.h"


namespace Heracles
{
	namespace Boss
	{
		static Vector VecotrMultFloat(Vector v, float f)
		{
			return Vector
			{
				v.x * f,
				v.y * f,
				v.z * f,
				v.w * f
			};
		}


		BossBullet::BossBullet(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
		{
		}

		void BossBullet::Start()
		{
			transform = this->GetGameObject()->GetComponent<Transform>();
		}

		void BossBullet::PreUpdate(float tick)
		{
			if (this->activatedThisFrame == true)
			{
				this->activatedThisFrame = false;
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(this->initialPos, Space::WORLD);
				this->GetGameObject()->GetComponent<Transform>()->LookAt(this->initialPos + this->direction);
			}
		}

		void BossBullet::Update(float tick)
		{
			this->transform->SetPosition(this->transform->GetWorldPosition() + VecotrMultFloat(this->transform->GetLook(), this->speed * tick),Space::WORLD);
		}

		void BossBullet::LateUpdate(float tick)
		{
		}

		void BossBullet::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetName() == TEXT("Player"))
			{
				other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->damage);
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f), Space::WORLD);
				this->SetSpeed(0.f);
			}
		}
		void BossBullet::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetName() == TEXT("Player"))
			{
				other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->damage);
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f), Space::WORLD);
				this->SetSpeed(0.f);
			}
		}
		void BossBullet::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{
			if (other->GetGameObject()->GetName() == TEXT("Player"))
			{
				other->GetGameObject()->GetComponent<Player::PlayerStateMachine>()->GetDamage(this->damage);
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f), Space::WORLD);
				this->SetSpeed(0.f);

			}
		}
	}
}