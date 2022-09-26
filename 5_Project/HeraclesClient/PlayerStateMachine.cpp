#include "pch.h"
#include "PlayerStateMachine.h"
#include "WeaponMonoBehaviour.h"
#include "Animator.h"

#include "BoxCollider.h"
#include "RayCollider.h"
#include "GameState.h"
#include "UIManager.h"
#include "StringHelper.h"
#include "Timer.h"

#include "SoundManager.h"
#include "SpriteAnimator.h"
#include "SwordAttack.h"
namespace Heracles
{
	namespace Player
	{
		static const float attackTiming = 0.5f;
		static const float tempDodgeTime = 0.25f;


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

		bool EventBase::ToBeHandled()
		{
			if (externalEventType == ExternalEvent::NONE && internalEventTye == InternalEvent::NONE)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		void EventBase::Initialize()
		{
			this->handled = false;
			this->fromPrevState = false;
			this->prevState = PlayerState::NONE;
			this->nextState = PlayerState::NONE;
			this->externalEventType = ExternalEvent::NONE;
			this->internalEventTye = InternalEvent::NONE;
			//this->DelayedState = PlayerState::NONE;
		}

		void Idle::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->externalEventType == ExternalEvent::DODGE)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;
				return this->Exit(stateMachine, e);
			}
			else if (e->externalEventType == ExternalEvent::SKILL)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::SKILL;
				return this->Exit(stateMachine, e);
			}

			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (!e->ToBeHandled()) { return e->Initialize(); }

			if (e->externalEventType == ExternalEvent::MOVE)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::MOVE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}
			else if (e->externalEventType == ExternalEvent::ATTACK)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::FISRST_ATTACK;
				e->internalEventTye = InternalEvent::TRANSITION;
				return this->Exit(stateMachine, e);
			}
			else
			{
				e->Initialize();
				return;
			}

		}
		void Idle::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			stateMachine->SetCurrentState(PlayerState::IDLE);
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Idle");

			return e->Initialize();
		}

		void Idle::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			//stateMachine->GetGameObject()->GetComponent<Animator>();

			e->fromPrevState = true;
			e->prevState = this->tag;

			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void Move::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (!e->ToBeHandled())
			{
				e->nextState = PlayerState::IDLE;
				return this->Exit(stateMachine, e);
			}

			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (e->externalEventType == ExternalEvent::DODGE)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}
			else if (e->externalEventType == ExternalEvent::SKILL)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::SKILL;
				return this->Exit(stateMachine, e);
			}
			else if (e->externalEventType == ExternalEvent::MOVE)
			{
				SoundManager::GetInstance()->PlayWalkSound("PlayerRun",false);
				return  e->Initialize();
			}
			else if (e->externalEventType == ExternalEvent::ATTACK)
			{
				e->ExEventHandled();
				e->nextState = PlayerState::FISRST_ATTACK;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}
			else
			{
				e->Initialize();
				e->nextState = PlayerState::IDLE;
				e->internalEventTye = InternalEvent::TRANSITION;
				return this->Exit(stateMachine, e);
			}
		}

		void Move::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			stateMachine->SetCurrentState(PlayerState::MOVE);
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Run");

			return e->Initialize();
		}

		void Move::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			e->fromPrevState = true;
			e->prevState = this->tag;
			SoundManager::GetInstance()->PlayWalkSound("PlayerRun", true);

			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void FirstAttack::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->externalEventType == ExternalEvent::DODGE)
			{
				if (this->running)
				{
					stateMachine->attackTimer = stateMachine->attackDelay;
				}
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}

			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (e->externalEventType == ExternalEvent::MOVE)
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying() == false)
				{
					if (e->DelayedState == PlayerState::SECOND_ATTACK)
					{
						e->DelayedState = PlayerState::NONE;
						e->nextState = PlayerState::SECOND_ATTACK;
						return this->Exit(stateMachine, e);
					}
				}
				else
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > attackTiming && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
			}
			else if (e->externalEventType == ExternalEvent::ATTACK)
			{
				e->ExEventHandled();
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying())
				{
					e->DelayedState = PlayerState::SECOND_ATTACK;
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > attackTiming && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
				else
				{
					e->nextState = PlayerState::SECOND_ATTACK;
					e->internalEventTye = InternalEvent::TRANSITION;
					return this->Exit(stateMachine, e);
				}
			}
			else
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > attackTiming && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
				else
				{
					if (e->DelayedState != PlayerState::NONE)
					{
						e->nextState = e->DelayedState;
						e->DelayedState = PlayerState::NONE;
						return this->Exit(stateMachine, e);
					}
					else
					{
						e->nextState = PlayerState::IDLE;
						e->internalEventTye = InternalEvent::TRANSITION;
						return this->Exit(stateMachine, e);
					}
				}
			}
		}

		void FirstAttack::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			this->progressTime = 0.f;
			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Attack1");
			SoundManager::GetInstance()->PlayEffect("playerAttack1");

			stateMachine->canMove = false;

			stateMachine->SetCurrentState(PlayerState::FISRST_ATTACK);

			stateMachine->attackDamage = stateMachine->attackPower;

			//Debug
			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

				UIManager::GetInstance()->SetTextUIText("State", "FirstAttack", debugCanvas);
			}

			return e->Initialize();
		}

		void FirstAttack::Exit(PlayerStateMachine* stateMachine, EventBase* e) // 여기서 delayed를 체크하는게 나을까?
		{
			this->running = false;
			this->progressTime = 0.f;
			this->attacked = false;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			stateMachine->canMove = true;

			e->fromPrevState = true;
			e->prevState = this->tag;

			if (e->nextState != PlayerState::SECOND_ATTACK)
			{
				stateMachine->attackTimer = stateMachine->attackDelay;
				stateMachine->attackDamage = stateMachine->attackPower;
			}
			else
			{
				//stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Attack2");
			}

			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void SecondAttack::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->externalEventType == ExternalEvent::DODGE)
			{
				if (this->running)
				{
					stateMachine->attackTimer = stateMachine->attackDelay;
				}
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}

			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (e->externalEventType == ExternalEvent::MOVE)
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying() == false)
				{
					if (e->DelayedState == PlayerState::THIRD_ATTACK)
					{
						e->DelayedState = PlayerState::NONE;
						e->nextState = PlayerState::THIRD_ATTACK;
						return this->Exit(stateMachine, e);
					}
				}
				else
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.2f && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}
					return e->Initialize();
				}
			}
			else if (e->externalEventType == ExternalEvent::ATTACK)
			{
				e->ExEventHandled();
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying())
				{
					e->DelayedState = PlayerState::THIRD_ATTACK;
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.2f && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
				else
				{
					e->nextState = PlayerState::THIRD_ATTACK;
					e->internalEventTye = InternalEvent::TRANSITION;
					return this->Exit(stateMachine, e);
				}
			}
			else
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.2f && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
				else
				{
					if (e->DelayedState != PlayerState::NONE)
					{
						e->nextState = e->DelayedState;
						e->DelayedState = PlayerState::NONE;
						return this->Exit(stateMachine, e);
					}
					else
					{
						e->nextState = PlayerState::IDLE;
						e->internalEventTye = InternalEvent::TRANSITION;
						return this->Exit(stateMachine, e);
					}
				}
			}
		}

		void SecondAttack::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

				UIManager::GetInstance()->SetTextUIText("State", "SecondAttack", debugCanvas);
			}

			this->running = true;
			this->progressTime = 0.f;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			stateMachine->canMove = false;
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Attack2");
			SoundManager::GetInstance()->PlayEffect("playerAttack2");

			stateMachine->SetCurrentState(PlayerState::SECOND_ATTACK);
			stateMachine->attackDamage = (stateMachine->attackPower * 2.f);

			return e->Initialize();
		}

		void SecondAttack::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			this->attacked = false;
			this->progressTime = 0.f;
			stateMachine->canMove = true;
			stateMachine->attackDamage = stateMachine->attackPower;

			e->fromPrevState = true;
			e->prevState = this->tag;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			if (e->nextState != PlayerState::THIRD_ATTACK)
			{
				stateMachine->attackTimer = stateMachine->attackDelay;
			}

			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void ThirdAttack::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->externalEventType == ExternalEvent::DODGE)
			{
				if (this->running)
				{
					stateMachine->attackTimer = stateMachine->attackDelay;
				}
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}

			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (e->externalEventType == ExternalEvent::MOVE)
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying() == false)
				{
					e->nextState = PlayerState::MOVE;
					e->ExEventHandled();
					return this->Exit(stateMachine, e);
				}
				else
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.35f && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
			}
			else
			{
				if (stateMachine->GetGameObject()->GetComponent<Animator>()->IsPlaying())
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					if (this->progressTime > 0.35f && this->attacked == false)
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(true);
						this->attacked = true;
					}
					else
					{
						stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
					}

					return e->Initialize();
				}
				else
				{
					e->nextState = PlayerState::IDLE;
					e->DelayedState = PlayerState::NONE;
					e->internalEventTye = InternalEvent::TRANSITION;
					return this->Exit(stateMachine, e);
				}
			}
		}

		void ThirdAttack::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			this->progressTime = 0.f;
			stateMachine->canMove = false;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
			stateMachine->attackDamage = (stateMachine->attackPower * 2.5f);

			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Attack3");
			SoundManager::GetInstance()->PlayEffect("playerAttack3");

			stateMachine->SetCurrentState(PlayerState::THIRD_ATTACK);
			return e->Initialize();
		}

		void ThirdAttack::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			this->progressTime = 0.f;
			this->attacked = false;

			stateMachine->canMove = true;
			stateMachine->attackTimer = stateMachine->attackDelay;
			stateMachine->attackDamage = stateMachine->attackPower;

			e->fromPrevState = true;
			e->prevState = this->tag;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void DodgeState::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->fromPrevState)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (this->dodgeTime <= 0.f)
			{
				e->nextState = PlayerState::IDLE;
				e->ExEventHandled();
				e->internalEventTye = InternalEvent::TRANSITION;
				return this->Exit(stateMachine, e);
			}
			else
			{
				this->DodgeMove(stateMachine);
				this->dodgeTime -= Timer::GetInstance()->DeltaTime();
				return e->Initialize();
			}
		}

		void DodgeState::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			this->dodgeTime = tempDodgeTime;
			stateMachine->canMove = false;
			stateMachine->isInvincable = true;

			//회피 애니메이션 실행
			SoundManager::GetInstance()->PlayEffect("PlayerDash");

			stateMachine->SetCurrentState(PlayerState::DODGE);
			e->Initialize();
			e->DelayedState = PlayerState::NONE;

			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

				UIManager::GetInstance()->SetTextUIText("State", "DodgeState", debugCanvas);
			}

			return;
		}

		void DodgeState::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			this->dodgeTime = 0.f;
			stateMachine->canMove = true;

			e->fromPrevState = true;
			e->prevState = this->tag;
			stateMachine->dodgeTimer = stateMachine->dodgeDelay;
			stateMachine->isInvincable = false;
			return stateMachine->GetStatePtr(e->nextState)->EventCheck(stateMachine, e);
		}

		void DodgeState::DodgeMove(PlayerStateMachine* stateMachine)
		{
			auto ray = stateMachine->GetGameObject()->GetComponent<RayCollider>();
			if (ray->GetHit().hitGameObject != nullptr)
			{
				if (ray->GetHit().hitDist < 1.5f && ray->GetHit().hitGameObject->GetTag() == TEXT("Obstacle"))
				{
					return;
				}
			}
			auto tran = stateMachine->GetGameObject()->GetComponent<Transform>();
			tran->SetPosition(tran->GetWorldPosition() + VecotrMultFloat(tran->GetLook(), stateMachine->dodgeSpeed * Timer::GetInstance()->DeltaTime()));
		}

		void Death::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			stateMachine->currHp = 0.f;
			if (!this->running)
			{
				e->InEventHandled();
				return this->Enter(stateMachine, e);
			}

			if (this->deathTimer < 0.001f)
			{
				return this->Exit(stateMachine, e);
			}
			else
			{
				this->deathTimer -= Timer::GetInstance()->DeltaTime();
				return e->Initialize();
			}
		}

		void Death::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = true;
			this->deathTimer = this->deathAnimTime;

			if (GameState::GetInstance()->clearedMapNum <= 3)
			{
				GameState::GetInstance()->dieStage = GameState::GetInstance()->clearedMapNum;
			}
			else
			{
				GameState::GetInstance()->dieStage = GameState::GetInstance()->clearedMapNum - 1;
			}
			stateMachine->canMove = false;
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Death");
			stateMachine->GetGameObject()->GetChild(TEXT("PlayerDeath"))->GetComponent<SpriteAnimator>()->Play();

			SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(500.0f);


			stateMachine->SetCurrentState(PlayerState::DEATH);
			e->Initialize();
			e->DelayedState = PlayerState::NONE;
			return;
		}

		void Death::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;
			this->deathTimer = 0.f;

			GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>()->SavePlayerDataToGameState();
			GameState::GetInstance()->playerInfo->currHp = GameState::GetInstance()->playerInfo->maxHp;
			GameState::GetInstance()->mapCleared = false;
			GameState::GetInstance()->playNum += 1;
			GameState::GetInstance()->clearedMapNum = 0;
			GameState::GetInstance()->ResetMapVector();
			SceneManager::GetInstance()->LoadScene(TEXT("StartCurseMap"));

			// 게임 재시작
		}


		PlayerStateMachine::PlayerStateMachine(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj)
		{
			this->currHp = GameState::GetInstance()->playerInfo->currHp;

			this->eventData = new SomeEvent;
			this->stateList.emplace(PlayerState::IDLE, std::make_shared<Idle>());
			this->stateList.emplace(PlayerState::MOVE, std::make_shared<Move>());
			this->stateList.emplace(PlayerState::DODGE, std::make_shared<DodgeState>());
			this->stateList.emplace(PlayerState::FISRST_ATTACK, std::make_shared<FirstAttack>());
			this->stateList.emplace(PlayerState::SECOND_ATTACK, std::make_shared<SecondAttack>());
			this->stateList.emplace(PlayerState::THIRD_ATTACK, std::make_shared<ThirdAttack>());
			this->stateList.emplace(PlayerState::DEATH, std::make_shared<Death>());
			this->stateList.emplace(PlayerState::ATTACKED, std::make_shared<Attacked>());
			this->stateList.emplace(PlayerState::SKILL, std::make_shared<Skill>());

			this->currState = PlayerState::IDLE;
			this->delayedState = PlayerState::NONE;
			this->eventLayer = StateLayer::NONE;
		}

		PlayerStateMachine::~PlayerStateMachine()
		{
			delete this->eventData;
		}

		void PlayerStateMachine::SetStopTimer(float t)
		{
			if (!this->isInvincable)
			{
				this->beAttackedTimer = t;
				this->isInvincable = true;
				this->invincableTimer = this->invincableTime;
			}
		}

		void PlayerStateMachine::GetDamage(float damage)
		{
			if (this->isInvincable)
			{
				return;
			}
			else
			{
				float finalDamage = damage - this->armor;
				if (finalDamage <= 0.f)
				{
					finalDamage = 1.f;
				}
				this->currHp -= finalDamage;
				this->isInvincable = true;
				this->invincableTimer = this->invincableTime;
				InputManager::GetInstance()->GetGamePad()->StartVibration(500, 500);
				SoundManager::GetInstance()->PlayEffect("playerHit");
				this->SetCurrentState(PlayerState::ATTACKED);
				this->GetGameObject()->GetChild(TEXT("HitEffect"))->GetComponent<SpriteAnimator>()->Play();
				InputManager::GetInstance()->GetGamePad()->StopVibration();

				SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(100.0f);
			}
		}

		void PlayerStateMachine::Start()
		{

			this->InitializePlayerFromGameState();
		}

		void PlayerStateMachine::PreUpdate(float tick)
		{
			if (this->currHp <= 0.f)
			{
				this->SetCurrentState(PlayerState::DEATH);
			}

			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

				std::string p = "PlayerCurrentHp : ";
				p.append((std::to_string(this->currHp)));
				UIManager::GetInstance()->SetTextUIText("PlayerCurrentHp", p, debugCanvas);

				auto obj = this->GetGameObject()->GetComponent<RayCollider>()->GetHit().hitGameObject;
				if (obj != nullptr)
				{
					std::string temp = "PlayerRay : ";
					std::string t = StringHelper::WideToString(obj->GetName());
					UIManager::GetInstance()->SetTextUIText("PlayerRayCast", temp.append(t), debugCanvas);
					return;
				}
				UIManager::GetInstance()->SetTextUIText("PlayerRayCast", "PlayerRay : Empty", debugCanvas);
			}

		}

		void PlayerStateMachine::Update(float tick)
		{
			GameState::GetInstance()->playerInfo->currHp = this->currHp;
			GameState::GetInstance()->playerInfo->memoryPieceNum = this->memoryPieceNum;
			GameState::GetInstance()->playerInfo->maxHp = this->maxHp;
			GameState::GetInstance()->playerInfo->money = this->money;
			this->Run(tick);

			{
				Canvas* debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

				Vector p = this->GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
				std::string pos;
				pos.append("PlayerPos : ");
				pos.append(std::to_string(p.x)); pos.append(", ");
				pos.append(std::to_string(p.y)); pos.append(", ");
				pos.append(std::to_string(p.z)); pos.append(", ");

				UIManager::GetInstance()->SetTextUIText("PlayerPos", pos, debugCanvas);
			}

		}

		void PlayerStateMachine::LateUpdate(float tick)
		{
			auto ray = this->GetGameObject()->GetComponent<RayCollider>();
			ray->SetDirection(this->GetGameObject()->GetComponent<Transform>()->GetLook());

			{
				std::string money = "Money : ";
				money.append(std::to_string(this->money));
				std::string armor = "Armor : ";
				armor.append(std::to_string(this->armor));
				std::string playnum = "PlayeNum : ";
				playnum.append(std::to_string(GameState::GetInstance()->playNum));
				UIManager::GetInstance()->SetTextUIText("Money", money);
				UIManager::GetInstance()->SetTextUIText("Armor", armor);
				UIManager::GetInstance()->SetTextUIText("PlayNum", playnum);
			}

		}

		void PlayerStateMachine::InitializePlayerFromGameState()
		{
			auto info = GameState::GetInstance()->playerInfo;

			if (GameState::GetInstance()->cursePain)
			{
				info->armor *= 0.7f;
			}
			else
			{
				this->armor = info->armor;
			}

			if (GameState::GetInstance()->curseSight)
			{
				// 시야 차단 Blur
			}

			this->currHp = info->currHp;
			this->maxHp = info->maxHp;

			this->dodgeSpeed = info->dodgeSpeed;
			this->dodgeDelay = info->dodgeDelay;

			this->memoryPieceNum = info->memoryPieceNum;
			this->money = info->money;
		}

		void PlayerStateMachine::SavePlayerDataToGameState()
		{
			auto info = GameState::GetInstance()->playerInfo;
			info->currHp = this->currHp;
			info->maxHp = this->maxHp;
			info->armor = this->armor;

			info->dodgeSpeed = this->dodgeSpeed;
			info->dodgeDelay = this->dodgeDelay;

			info->memoryPieceNum = this->memoryPieceNum;
			info->money = this->money;

			//이거 외에는 초기값 사용
		}

		void PlayerStateMachine::Run(float tick)
		{
			auto t = this->GetStatePtr(GetCurrentState());
			t->EventCheck(this, this->eventData);
			this->UpdateTimer(tick);

		}

		void PlayerStateMachine::UpdateTimer(float tick)
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
			if (this->dodgeTimer > 0.f)
			{
				float target = this->dodgeTimer - tick;
				if (target < 0.f)
				{
					this->dodgeTimer = 0.f;
				}
				else
				{
					this->dodgeTimer = target;
				}
			}
			if (this->beAttackedTimer > 0.f)
			{
				float target = this->beAttackedTimer - tick;
				if (target < 0.f)
				{
					this->beAttackedTimer = 0.f;
				}
				else
				{
					this->beAttackedTimer = target;
				}
			}
			if (this->invincableTimer > 0.f)
			{
				float target = this->invincableTimer - tick;
				if (target < 0.f)
				{
					this->invincableTimer = 0.f;
					this->isInvincable = false;
				}
				else
				{
					this->invincableTimer = target;
				}
			}
			if (this->skillTimer > 0.f)
			{
				float target = this->skillTimer - tick;
				if (target < 0.f)
				{
					this->skillTimer = 0.f;
				}
				else
				{
					this->skillTimer = target;
				}
			}

		}

		void Attacked::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (this->running)
			{
				if (this->progressTime < 1.f)
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					return;
				}
				else
				{
					return this->Exit(stateMachine, e);
				}
			}
			else
			{
				return this->Enter(stateMachine, e);
			}
		}

		void Attacked::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			e->Initialize();
			this->running = true;
			this->progressTime = 0.f;
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Hit");;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);
			stateMachine->SetCurrentState(PlayerState::ATTACKED);

			SceneManager::GetInstance()->GetCurrentScene()->GetMainCamera()->SetShaking(100.0f);

			return;
		}

		void Attacked::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;

			stateMachine->GetWeapon()->GetComponent<Weapon::WeaponMonoBehaviour>()->SetAttackState(false);

			e->nextState = PlayerState::IDLE;
			e->fromPrevState = true;
			e->prevState = PlayerState::ATTACKED;
			return stateMachine->GetStatePtr(PlayerState::IDLE)->EventCheck(stateMachine, e);
		}

		void Skill::EventCheck(PlayerStateMachine* stateMachine, EventBase* e)
		{
			if (e->externalEventType == ExternalEvent::DODGE)
			{
				if (this->running)
				{
					stateMachine->skillTimer = stateMachine->skillDelay;
				}
				e->ExEventHandled();
				e->nextState = PlayerState::DODGE;
				e->internalEventTye = InternalEvent::TRANSITION;

				return this->Exit(stateMachine, e);
			}

			if (this->running)
			{
				if (this->progressTime < 1.f)
				{
					this->progressTime += Timer::GetInstance()->DeltaTime();

					return;
				}
				else
				{
					return this->Exit(stateMachine, e);
				}
			}
			else
			{
				return this->Enter(stateMachine, e);
			}
		}

		void Skill::Enter(PlayerStateMachine* stateMachine, EventBase* e)
		{
			e->Initialize();
			this->running = true;
			this->progressTime = 0.f;
			stateMachine->GetGameObject()->GetComponent<Animator>()->Play("Skill");

			stateMachine->SetCurrentState(PlayerState::SKILL);

			stateMachine->skill->GetComponent<Transform>()->SetPosition(stateMachine->GetGameObject()->GetComponent<Transform>()->GetWorldPosition(), Space::WORLD);
			stateMachine->skill->GetComponent<SwordAttack>()->activated = true;
			stateMachine->skill->GetChild(TEXT("SkillEffect"))->GetComponent<SpriteAnimator>()->SetLoop(true);
		}

		void Skill::Exit(PlayerStateMachine* stateMachine, EventBase* e)
		{
			this->running = false;


			e->nextState = PlayerState::IDLE;
			e->fromPrevState = true;
			e->prevState = PlayerState::SKILL;
			return stateMachine->GetStatePtr(PlayerState::IDLE)->EventCheck(stateMachine, e);
		}

}
}

