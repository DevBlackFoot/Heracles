#include "pch.h"
#include "MonsterState.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "Animator.h"

#include "MonsterAttack.h"

#include "RayCollider.h"
#include "GameState.h"
#include "UIManager.h"
#include "Timer.h"

#include "SoundManager.h"

#include "InputManager.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	using namespace Obstacle;
	namespace Monster
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

		MonsterState::MonsterState(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj, ComponentType::MONOBEHAVIOR)
		{
			currState = State::IDLE;
			this->transform = this->GetGameObject()->GetComponent<Transform>();
		}
		MonsterState::~MonsterState()
		{
		}
		void MonsterState::SetCurrentObstacle(std::shared_ptr<ObstacleState> obj)
		{
			this->obstacle = obj;
		}
		std::shared_ptr<ObstacleState> MonsterState::GetObstacleState()
		{
			return this->obstacle;
		}
		float MonsterState::GetMoveSpeed()
		{
			return this->moveSpeedThisFrame * 4.f;
		}
		void MonsterState::SetRayDirection(Vector dir)
		{
			this->GetGameObject()->GetComponent<RayCollider>()->SetDirection(dir);
		}
		void MonsterState::SetAnimation(const std::string& name)
		{
			this->GetGameObject()->GetComponent<Animator>()->Play(name);
		}
		void MonsterState::UpdateColliderRadius()
		{
			this->GetGameObject()->GetComponent<SphereCollider>()->SetRadius(this->attackRange);
		}
		void MonsterState::DestroyMonster()
		{
			this->GetGameObject()->Destroy();
		}

		void MonsterState::GetDamage(int damage)
		{
			this->monsterHp -= damage;

			// 이 때 이펙트 소환.
			/*if(GetGameObject()->GetChild(TEXT("MonsterHitEffect"))->GetComponent<SpriteAnimator>() != nullptr)
				GetGameObject()->GetChild(TEXT("MonsterHitEffect"))->GetComponent<SpriteAnimator>()->SetIsEnable(true);
				*/

			if(isStartVibe == false)
				isStartVibe = true;
			// 이때 진동 시작
			InputManager::GetInstance()->GetGamePad()->StartVibration(500, 500);
			
			SoundManager::GetInstance()->PlayEffect(this->hitSound);

			SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(50.0f);
		}

		bool MonsterState::CheckObstacle()
		{
			std::shared_ptr<GameObject> obj = this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitGameObject;
			float t = this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitDist;
			if (obj != nullptr && obj->GetTag() == TEXT("Obstacle")) //  && 
			{
				if (this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitDist < 1.f)
				{
					this->SetCurrentObstacle(obj->GetComponent<Obstacle::ObstacleState>());
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				this->SetCurrentObstacle(nullptr);
				return false;
			}
		}

		void MonsterState::SetDirectionCollider()
		{
			auto child = this->GetGameObject()->GetChild(TEXT("Direction"))->GetComponent<Transform>();
			child->SetPosition(this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + VecotrMultFloat(this->targetDir, 1.f));
			this->wall = nullptr;
		}

		void MonsterState::Awake()
		{
			this->stateList.emplace(State::IDLE, std::make_shared<Idle>());
			this->stateList.emplace(State::IDLE_AGGRESSIVE, std::make_shared<IdleAggressive>());
			this->stateList.emplace(State::MOVE, std::make_shared<Move>());
			this->stateList.emplace(State::ATTACK, std::make_shared<Attack>());
			this->stateList.emplace(State::DEATH, std::make_shared<Death>());

			debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");
		}

		void MonsterState::Start()
		{
			if (GameState::GetInstance()->curseMadness)
			{
				this->moveSpeed *= 1.1f;
			}
			this->currState = State::IDLE;
			this->SetAnimation("MonsterIdle");


			this->GetGameObject()->GetComponent<SphereCollider>()->SetRadius(this->detectRange);
		}

		void MonsterState::PreUpdate(float tick)
		{

			if (this->monsterHp <= 0.01f)
			{
				this->currEvent = Event::DEATH;
				return;
			}

			if (this->getAttack == true)
			{
				this->SetCurrentState(State::ATTACKED);
				this->currEvent = Event::ATTACKED;
			}

			Vector player = GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition();
			Vector monster = this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
			Vector look = Vector3Normalize(player - monster);

			if (this->CheckObstacle())
			{
				//this->UpdateMoveDirection(tick);
				this->moveDir = Vector::Zero;
				this->targetDir = look;
			}
			else
			{
				this->moveDir = look;
				this->targetDir = look;
			}

			this->moveSpeedThisFrame = this->moveSpeed * tick;

		}


		void MonsterState::Update(float tick)
		{
			static float tickCount = 0.0f;

			if(tickCount > 2.0f)
			{
				isStartVibe = false;
				tickCount = 0.0f;
				InputManager::GetInstance()->GetGamePad()->StopVibration();
			}

			if (this->currEvent == Event::DEATH)
			{
				this->GetStatePtr(State::DEATH)->EventCheck(this, tick);
				return;
			}
			else if(this->currState == State::ATTACKED)
			{
				this->GetStatePtr(GetCurrentState())->EventCheck(this, tick);
				return;
			}

			if (this->enterAttackState && this->attackTimer <= 0.f - epsilon)
			{
				this->currState = State::ATTACK;
				this->attackTimer = this->attackMotionLength;
				this->enterAttackState = false;
			}

			if(isStartVibe)
				tickCount += tick;

			this->GetStatePtr(GetCurrentState())->EventCheck(this, tick);

		}
		void MonsterState::LateUpdate(float tick)
		{
			this->obstacle = nullptr;
			this->currEvent = Event::NONE;
			this->UpdateTimer(tick);
			this->SetRayDirection(this->targetDir);
			
			this->SetDirectionCollider();
		}

		void MonsterState::UpdateTimer(float tick)
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

		void MonsterState::UpdateMoveDirection(float tick)
		{
			Obstacle::Direction dir = this->GetObstacleState()->GetDirectionFromTarget(this->GetTransform()->GetLook(), this->GetTransform()->GetWorldPosition());
			this->moveDir = Vector3Normalize(this->obstacle->GetDirectionToTarget(dir));
			this->changeDirTimer = 1.f;			
		}

		void Idle::EventCheck(MonsterState* monsterState, float tick)
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

		void Idle::Enter(MonsterState* monsterState, float tick)
		{
			monsterState->SetAnimation("MonsterIdle");
			this->running = true;
		}

		void Idle::Exit(MonsterState* monsterState, float tick)
		{
			this->running = false;
			monsterState->GetStatePtr(State::IDLE_AGGRESSIVE)->EventCheck(monsterState, tick);
		}

		void IdleAggressive::EventCheck(MonsterState* monsterState, float tick)
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

		void IdleAggressive::Enter(MonsterState* monsterState, float tick)
		{
			monsterState->GetGameObject()->GetComponent<Transform>()->LookAt(GameState::GetInstance()->GetPlayer()->GetComponent<Transform>()->GetWorldPosition());
			monsterState->SetCurrentState(State::IDLE_AGGRESSIVE);
			monsterState->SetAnimation("MonsterIdle");
		}

		void IdleAggressive::Exit(MonsterState* monsterState, float tick)
		{
			monsterState->GetStatePtr(State::MOVE)->EventCheck(monsterState, tick);
		}

		void Move::EventCheck(MonsterState* monsterState, float tick)
		{
			if (monsterState->enterAttackState)
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

		void Move::Enter(MonsterState* monsterState, float tick)
		{
			this->running = true;
			monsterState->SetCurrentState(State::MOVE);
			monsterState->SetAnimation("MonsterRun");
			return;
		}

		void Move::Exit(MonsterState* monsterState, float tick)
		{
			this->running = false;
			monsterState->GetStatePtr(State::ATTACK)->EventCheck(monsterState, tick);
		}

		void Move::MoveToPlayer(MonsterState* monsterState)
		{
			auto obj = monsterState->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitGameObject;
			if (obj != nullptr)
			{
				if (obj->GetTag() == TEXT("Obstacle") && monsterState->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitDist <= 1.f)
				{
					return;
				}
			}

			monsterState->GetTransform()->LookAt(monsterState->GetTransform()->GetWorldPosition() + monsterState->moveDir);
			monsterState->GetTransform()->SetPosition(monsterState->GetTransform()->GetWorldPosition() + VecotrMultFloat(monsterState->moveDir, monsterState->GetMoveSpeed()), Space::WORLD);
		}

		void Attack::EventCheck(MonsterState* monsterState, float tick)
		{
			if (this->running)
			{
				if (this->attackTimer <= 0.001f)
				{
					return this->Exit(monsterState, tick);
				}
				else
				{
					if (this->progressTime > 1.f && !this->attacked)
					{
						monsterState->GetMonsterAttack()->GetComponent<MonsterAttack>()->attacking = true;
						this->attacked = true;
					}
					else
					{
						monsterState->GetMonsterAttack()->GetComponent<MonsterAttack>()->attacking = false;
					}
					this->progressTime += tick;
					this->attackTimer -= tick;
				}
			}
			else
			{
				return this->Enter(monsterState, tick);
			}
		}

		// 공격속도는 어택타임을 길게하고 애니메이션 뒤에 약간의 텀을 추가하면됨
		void Attack::Enter(MonsterState* monsterState, float tick)
		{
			this->running = true;
			this->attacked = false;
			this->progressTime = 0.f;
			monsterState->enterAttackState = false;
			monsterState->SetCurrentState(State::ATTACK);
			monsterState->SetAnimation("MonsterAttack");
		}

		void Attack::Exit(MonsterState* monsterState, float tick)
		{
			this->attackTimer = this->attackTime;
			this->attacked = false;
			this->running = false;

			return monsterState->GetStatePtr(State::MOVE)->EventCheck(monsterState, tick);
		}

		void Death::EventCheck(MonsterState* monsterState, float tick)
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

		void Death::Enter(MonsterState* monsterState, float tick)
		{
			this->running = true;
			GameState::GetInstance()->currMonsterNum -= 1;
			GameState::GetInstance()->totalMonsterNumInMap -= 1;
			 
			monsterState->SetCurrentState(this->tag);
			monsterState->SetAnimation("MonsterDeath");
		}

		void Death::Exit(MonsterState* monsterState, float tick)
		{
			//monsterState->DestroyMonster(); 이거 외 안되지
			
			monsterState->GetGameObject()->GetComponent<Transform>()->SetPosition(Vector(0.f, 100.f, 0.f), Space::WORLD);
		}

		void Attacked::EventCheck(MonsterState* base, float tick)
		{
			if (this->running)
			{
				if (this->progressTime < 1.f)
				{
					this->progressTime += tick;

					return;
				}
				else
				{
					return this->Exit(base, tick);
				}
			}
			else
			{
				return this->Enter(base, tick);
			}
		}

		void Attacked::Enter(MonsterState* monsterState, float tick)
		{
			this->running = true;
			this->progressTime = 0.f;
			monsterState->SetAnimation("MonsterHit");
		}

		void Attacked::Exit(MonsterState* monsterState, float tick)
		{
			this->running = false;
			monsterState->GetStatePtr(State::MOVE)->EventCheck(monsterState, tick);
		}

}
}
