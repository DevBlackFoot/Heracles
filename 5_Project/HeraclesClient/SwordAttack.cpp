#include "pch.h"
#include "SwordAttack.h"
#include "ColliderBase.h"
#include "GameState.h"
#include "PlayerStateMachine.h"
#include "MonsterState.h"
#include "BossTree.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	namespace Player
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

		SwordAttack::SwordAttack(std::weak_ptr<GameObject> obj) : MonoBehaviour(obj)
		{
		}
		void SwordAttack::Start()
		{

		}
		void SwordAttack::PreUpdate(float tick)
		{
			if (this->timer > 2.f)
			{
				this->activated = false;
				this->timer = 0.f;
			}
		}
		void SwordAttack::Update(float tick)
		{
			if (this->activated)
			{
				this->timer += tick;
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + VecotrMultFloat(this->direction, this->speed * tick));
			}
			else
			{
				this->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f));
				this->GetGameObject()->GetChild(TEXT("SkillEffect"))->GetComponent<SpriteAnimator>()->SetLoop(false);
			}
		}
		void SwordAttack::OnTriggerEnter(std::shared_ptr<ColliderBase> other)
		{
			if (this->activated)
			{
				auto obj = other->GetGameObject();
				if (obj->GetName() == TEXT("Monster") || obj->GetName() == TEXT("BigMonster"))
				{
					obj->GetComponent<Monster::MonsterState>()->GetDamage(this->attackPower);
				}
				else if(obj->GetName() == TEXT("Boss"))
				{
					obj->GetComponent<Boss::BossTree>()->GetDamage(this->attackPower);
				}
			}
		}
		void SwordAttack::OnTriggerStay(std::shared_ptr<ColliderBase> other)
		{

		}
		void SwordAttack::OnTriggerExit(std::shared_ptr<ColliderBase> other)
		{

		}
	}
}