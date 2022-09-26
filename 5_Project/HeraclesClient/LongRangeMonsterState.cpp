#include "pch.h"
#include "LongRangeMonsterState.h"

#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Animator.h"

#include "MonsterAttack.h"

#include "RayCollider.h"
#include "GameState.h"
#include "UIManager.h"
#include "Timer.h"

namespace Heracles
{
	namespace LongRangeMonster
	{
		static const float epsilon = 0.0001f;
		static const float attackRot = 180.f;

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

		LongRangeMonsterState::LongRangeMonsterState(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj,ComponentType::MONOBEHAVIOR)
		{
			currState = State::IDLE;
			this->transform = this->GetGameObject()->GetComponent<Transform>();
		}
		LongRangeMonsterState::~LongRangeMonsterState()
		{
		}
		void LongRangeMonsterState::SetCurrentObstacle(std::shared_ptr<ObstacleState> obj)
		{
			this->obstacle = obj;
		}
		std::shared_ptr<ObstacleState> LongRangeMonsterState::GetObstacleState()
		{
			return this->obstacle;
		}
		float LongRangeMonsterState::GetMoveSpeed()
		{
			return this->moveSpeedThisFrame * 4.f;
		}
		void LongRangeMonsterState::SetRayDirection(Vector dir)
		{
			this->GetGameObject()->GetComponent<RayCollider>()->SetDirection(dir);
		}
		void LongRangeMonsterState::SetAnimation(const std::string& name)
		{
			this->GetGameObject()->GetComponent<Animator>()->Play(name);
		}
		void LongRangeMonsterState::UpdateColliderRadius()
		{
			this->GetGameObject()->GetComponent<SphereCollider>()->SetRadius(this->attackRange);
		}
		void LongRangeMonsterState::DestroyMonster()
		{
			this->GetGameObject()->Destroy();
		}
		void LongRangeMonsterState::GetDamage(int damage)
		{
			this->monsterHp -= damage;
		}
		bool LongRangeMonsterState::CheckObstacle()
		{
			std::shared_ptr<GameObject> obj = this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitGameObject;
			if (obj != nullptr && this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitDist < 1.f)
			{
				if (obj->GetTag() == TEXT("Obstacle"))
				{
					this->SetCurrentObstacle(obj->GetComponent<Obstacle::ObstacleState>());
					return true;
				}
			}
			this->SetCurrentObstacle(nullptr);
			return false;
		}
		void LongRangeMonsterState::ReGenerate(Vector pos)
		{
			if (GameState::GetInstance()->curseMadness)
			{
				this->moveSpeed *= 1.1f;
			}

			GameState::GetInstance()->currMonsterNum += 1;
			GameState::GetInstance()->mapCleared = false;

			this->monsterHp = 10;
			this->currState = State::IDLE;
			this->SetAnimation("Ely_Idle");
			this->GetGameObject()->GetComponent<SphereCollider>()->SetRadius(this->detectRange);
			this->GetGameObject()->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
		}
		void LongRangeMonsterState::Awake()
		{
			this->stateList.emplace(State::IDLE, std::make_shared<Idle>());
			this->stateList.emplace(State::IDLE_AGGRESSIVE, std::make_shared<IdleAggressive>());
			this->stateList.emplace(State::MOVE, std::make_shared<Move>());
			this->stateList.emplace(State::ATTACK, std::make_shared<Attack>());
			this->stateList.emplace(State::DEATH, std::make_shared<Death>());
		}
		void LongRangeMonsterState::Start()
		{
			if (GameState::GetInstance()->curseMadness)
			{
				this->moveSpeed *= 1.1f;
			}
			this->currState = State::IDLE;
			this->SetAnimation("Ely_Idle");

			this->GetGameObject()->GetComponent<SphereCollider>()->SetRadius(this->detectRange);
		}
		void LongRangeMonsterState::PreUpdate(float tick)
		{
			if (this->monsterHp <= 0)
			{
				this->currEvent = Event::DEATH;
				return;
			}
		}
		void LongRangeMonsterState::Update(float tick)
		{
			if (this->currEvent == Event::DEATH)
			{
				this->GetStatePtr(State::DEATH)->EventCheck(this, tick);
				return;
			}

			if (this->enterAttackState && this->attackTimer <= 0.f - epsilon)
			{
				this->currState = State::ATTACK;
				this->attackTimer = this->attackMotionLength;
				this->enterAttackState = false;
			}

			this->GetStatePtr(GetCurrentState())->EventCheck(this, tick);
		}
		void LongRangeMonsterState::LateUpdate(float tick)
		{
			this->obstacle = nullptr;
			this->currEvent = Event::NONE;
			this->UpdateTimer(tick);
			this->SetRayDirection(this->targetDir);
		}
		void LongRangeMonsterState::UpdateTimer(float tick)
		{
			if (this->attackTimer > 0.f)
			{
				float target = this->attackTimer - tick;
				if (target < 0.f)
				{
					this->attackTimer = 0.f;
				}
				else
				{
					this->attackTimer = target;
				}
			}
			if (this->changeDirTimer)
			{
				float target = this->changeDirTimer - tick;
				if (target < 0.f)
				{
					this->changeDirTimer = 0.f;
				}
				else
				{
					this->attackTimer = target;
				}
			}
		}
		void LongRangeMonsterState::UpdateMoveDirection(float tick)
		{
			Obstacle::Direction dir = this->GetObstacleState()->GetDirectionFromTarget(this->GetTransform()->GetLook(), this->GetTransform()->GetWorldPosition());
			this->moveDir = Vector3Normalize(this->obstacle->GetDirectionToTarget(dir));
			this->changeDirTimer = 1.f;
		}
		void Idle::EventCheck(LongRangeMonsterState* monsterState, float tick)
		{
			if (!this->running)
			{
				this->Enter(monsterState, tick);
			}
			if (monsterState->currEvent == Event::DETECT)
			{
				monsterState->UpdateColliderRadius();
				return this->Exit(monsterState, tick);
			}
			else
			{
				return;
			}
		}
		void Idle::Enter(LongRangeMonsterState* monsterState, float tick)
		{
			monsterState->SetAnimation("Ely_Idle");
			this->running = true;
		}
		void Idle::Exit(LongRangeMonsterState* monsterState, float tick)
		{
			this->running = false;
			monsterState->GetStatePtr(State::IDLE_AGGRESSIVE)->EventCheck(monsterState, tick);
		}
		void IdleAggressive::EventCheck(LongRangeMonsterState* monsterState, float tick)
		{
			if (this->running)
			{
				this->timer -= tick;
				if (this->timer <= 0.f)
				{
					this->timer = 0.f;
					return this->Exit(monsterState, tick);
				}
			}
			else
			{
				this->timer = 1.f;
				this->running = true;
				return this->Enter(monsterState, tick);
			}
		}
		void IdleAggressive::Enter(LongRangeMonsterState* monsterState, float tick)
		{
			monsterState->GetGameObject()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
			monsterState->SetCurrentState(this->tag);
			monsterState->SetAnimation("Aggressive");
		}
		void IdleAggressive::Exit(LongRangeMonsterState* monsterState, float tick)
		{
			monsterState->GetStatePtr(State::MOVE)->EventCheck(monsterState, tick);
		}
		void Move::EventCheck(LongRangeMonsterState* monsterState, float tick)
		{
			Vector monster = monsterState->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			if (Vector3Length(player-monster)<5.f)
			{
				return this->Exit(monsterState, tick);
			}
			else
			{
				if (this->running)
				{
					return this->MoveToPlayer(monsterState);
				}
				else
				{
					return this->Enter(monsterState, tick);
				}
			}
		}
		void Move::Enter(LongRangeMonsterState* monsterState, float tick)
		{
			this->running = true;
			monsterState->SetCurrentState(this->tag);
			monsterState->SetAnimation("Ely_Run");
			return;
		}
		void Move::Exit(LongRangeMonsterState* monsterState, float tick)
		{
			this->running = false;
			monsterState->GetStatePtr(State::ATTACK)->EventCheck(monsterState, tick);
		}
		void Move::MoveToPlayer(LongRangeMonsterState* monsterState)
		{
			monsterState->GetTransform()->LookAt(monsterState->GetTransform()->GetWorldPosition() + monsterState->moveDir);
			monsterState->GetTransform()->SetPosition(monsterState->GetTransform()->GetWorldPosition() + VecotrMultFloat(monsterState->moveDir, monsterState->GetMoveSpeed()), Space::WORLD);
		}
		void Attack::EventCheck(LongRangeMonsterState* monsterState, float tick)
		{
			if (this->running)
			{
				if (this->attackTimer <= 0.001f)
				{
					return this->Exit(monsterState, tick);
				}
				else
				{
					this->attackTimer -= tick;
				}
			}
			else
			{
				if (monsterState->enterAttackState)
				{
					return this->Enter(monsterState, tick);
				}
			}
		}
		void Attack::Enter(LongRangeMonsterState* monsterState, float tick)
		{
			this->running = true;
			monsterState->enterAttackState = false;
			monsterState->SetCurrentState(this->tag);
			monsterState->SetAnimation("Ely_Attack");
			//탄환 시작위치 + 방향지정
		}
		void Attack::Exit(LongRangeMonsterState* monsterState, float tick)
		{
			this->attackTimer = this->attackTime;
			this->running = false;
			// 탄환 반환

			return monsterState->GetStatePtr(State::MOVE)->EventCheck(monsterState, tick);
		}
		void Death::EventCheck(LongRangeMonsterState* monsterState, float tick)
		{
			if (this->running)
			{
				if (monsterState->dyingAnimationTime <= 0)
				{
					return this->Exit(monsterState, tick);
				}
				else
				{
					monsterState->dyingAnimationTime -= tick;
				}
			}
			else
			{
				this->Enter(monsterState, tick);
			}
		}
		void Death::Enter(LongRangeMonsterState* monsterState, float tick)
		{
			this->running = true;
			GameState::GetInstance()->currMonsterNum -= 1;
			GameState::GetInstance()->totalMonsterNumInMap -= 1;

			monsterState->SetCurrentState(this->tag);
			monsterState->SetAnimation("Death");
		}
		void Death::Exit(LongRangeMonsterState* monsterState, float tick)
		{
			monsterState->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f), Space::WORLD);
		}
}

}