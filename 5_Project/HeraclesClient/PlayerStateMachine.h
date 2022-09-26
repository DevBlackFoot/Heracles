#pragma once

namespace Heracles
{
	namespace Player
	{
		class PlayerStateMachine;

		enum class PlayerState
		{
			DODGE = 0,
			ATTACKED,

			FISRST_ATTACK,
			SECOND_ATTACK,
			THIRD_ATTACK,
			SKILL,

			IDLE,
			MOVE,

			DEATH,

			NONE,
		};

		enum class StateLayer
		{
			DODGE_LAYER = 0,
			ATTACK_LAYER,
			MOVE_LAYER,
			NONE
		};

		enum class ExternalEvent
		{
			ATTACKED,
			DODGE,
			ATTACK,
			MOVE,
			SKILL,
			NONE
		};

		enum class InternalEvent
		{
			TRANSITION,
			DELAY,
			IGNORE_EVENT,
			NONE,
		};


		class EventBase abstract
		{
		public:
			ExternalEvent externalEventType = ExternalEvent::NONE;
			InternalEvent internalEventTye = InternalEvent::NONE;

			PlayerState prevState = PlayerState::NONE;
			PlayerState nextState = PlayerState::NONE;
			PlayerState DelayedState = PlayerState::NONE;

			bool handled = false;
			bool fromPrevState = true;

		public:
			EventBase() {};
			virtual ~EventBase() {};

			bool ToBeHandled();
			inline void ExEventHandled() { this->externalEventType = ExternalEvent::NONE; }
			inline void InEventHandled() { this->internalEventTye = InternalEvent::NONE; }
			void Initialize();

		};

		class InputEvent final : public EventBase
		{
		public:

		public:

		};

		class SomeEvent final : public EventBase
		{
		private:

		public:

		};

		class StateBase abstract
		{
		protected:
			bool running = false;

			float progressTime = 0.f;

			StateLayer layer = StateLayer::NONE;
			PlayerState tag = PlayerState::NONE;

		public:
			float dodgeTime = 0.f;
			bool attacked = false;
		public:
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) = 0;
			StateBase(PlayerState state)
			{
				this->tag = state;
			}

		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) = 0;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) = 0;

		};

		class Death final : public StateBase
		{
		private:
			float deathAnimTime = 4.2f;
			float deathTimer = 0.f;
		public:
			Death() :StateBase(PlayerState::DEATH)
			{
				this->tag = PlayerState::DEATH;
			}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;
			
		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};


		class DodgeState final : public StateBase
		{
		private:
		public:
			DodgeState() : StateBase(PlayerState::DODGE)
			{
				this->layer = StateLayer::DODGE_LAYER;
			}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;

		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;

			void DodgeMove(PlayerStateMachine* stateMachine);
		};

		class AttackState : public StateBase
		{
		private:

		public:
			AttackState(PlayerState state) : StateBase(state)
			{
				this->layer = StateLayer::ATTACK_LAYER;
			}
		};

		class FirstAttack : public AttackState
		{
		private:

		public:
			FirstAttack() : AttackState(PlayerState::FISRST_ATTACK)
			{
			}

			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;

		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;

			void Attack(PlayerStateMachine* stateMachine);

		};


		class SecondAttack : public AttackState
		{
		private:

		public:
			SecondAttack() : AttackState(PlayerState::FISRST_ATTACK) {}

			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;

		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};

		class ThirdAttack : public AttackState
		{
		private:

		public:
			ThirdAttack() : AttackState(PlayerState::FISRST_ATTACK) {}

			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;

		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};

		class Idle : public StateBase
		{
		private:
		public:
			Idle() : StateBase(PlayerState::IDLE) {}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;
		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};

		class Move : public StateBase
		{
		private:
		public:
			Move() : StateBase(PlayerState::MOVE) {}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;
		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};

		class Attacked : public StateBase
		{
		public:
			Attacked() : StateBase(PlayerState::ATTACKED) {}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;
		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};

		class Skill : public StateBase
		{
		public:
			Skill() : StateBase(PlayerState::SKILL){}
			virtual void EventCheck(PlayerStateMachine* stateMachine, EventBase* e) override;
		private:
			virtual void Enter(PlayerStateMachine* stateMachine, EventBase* e) override;
			virtual void Exit(PlayerStateMachine* stateMachine, EventBase* e) override;
		};



		class PlayerStateMachine : public ComponentBase
		{
		private:
			StateLayer eventLayer;
			PlayerState currState;

			PlayerState delayedState;

			EventBase* eventData;

			std::map<PlayerState, std::shared_ptr<StateBase>> stateList;

			Vector prevPosition;

		public:

			std::shared_ptr<GameObject> skill;


			float currHp = 100;
			float maxHp = 100;
			float armor = 0.f;

			//공격
			float attackPower = 10.f;

			float attackDamage = 10.f;

			float skillDamage = 20.f;

			float attackDelay = 1.f;
			float skillDelay = 2.f;

			// 이동
			float moveSpeed = 10.f;		   //
			float rotateSpeed = 3.f;      // 회전속도
			float worldYRotation; // 현재 캐릭터가 바라보는 방향

			// 회피기
			float dodgeSpeed = 80.f;
			float dodgeDelay = 1.f;       //sec

			bool isInvincable = false;
			float invincableTime = 2.5f;

			bool canMove = true;

			float attackTimer = 0;
			float dodgeTimer = 0;
			float beAttackedTimer = 0.f;
			float invincableTimer = 0.f;
			float skillTimer = 0.f;

		public:
			int memoryPieceNum = 0; // 기억 조각

			int money = 0; // 무기 재화
		public:
			PlayerStateMachine(std::weak_ptr<GameObject> obj);
			~PlayerStateMachine();

			inline void SetCurrentState(PlayerState target) { this->currState = target; };
			inline PlayerState GetCurrentState() { return currState; }

			inline PlayerState GetDelayedState() { return this->delayedState; }
			inline void SetDelayedState(PlayerState toDelay) { this->delayedState = toDelay; }

			inline std::shared_ptr<StateBase> GetStatePtr(PlayerState state) { return stateList[state]; }
			inline EventBase* GetEventData() { return eventData; }

			inline std::shared_ptr<GameObject> GetWeapon() { return this->GetGameObject()->GetChild(TEXT("Weapon")); }

			void SetStopTimer(float t);

			void GetDamage(float damage);

		public:

			virtual void Start() override;

			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			// 플레이어 정보 저장
			void InitializePlayerFromGameState();
			void SavePlayerDataToGameState();

		private:
			void Run(float);
			void UpdateTimer(float tick);
		};

	};
}

