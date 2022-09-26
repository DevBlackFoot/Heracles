#pragma once
#include "ObstacleState.h"

namespace Heracles
{
	using namespace Obstacle;
	namespace Monster
	{
		class MonsterState;

		enum class State
		{
			IDLE = 1,
			IDLE_AGGRESSIVE,
			MOVE,
			ATTACK,
			DEATH,
			ATTACKED
		};

		enum class Event
		{
			DETECT,
			DEATH,
			ATTACKED,
			NONE
		};

		class StateBase abstract
		{
		protected:
			bool running = false;
			float progressTime = 0.f;
			State tag;

		public:
			float attackTime = 2.0f; // 공격모션 지속시간
			float attackTimer = 0.f;
		public:
			virtual void EventCheck(MonsterState* base, float tick) = 0;
			StateBase(State state)
			{
				this->tag = state;
			}

		private:
			virtual void Enter(MonsterState* monsterState,float tick) = 0;
			virtual void Exit(MonsterState* monsterState, float tick) = 0;
		};

		class Idle final: public StateBase
		{
		public:
			Idle() :StateBase(State::IDLE){}
			virtual void EventCheck(MonsterState* base,float tick) override;
		private:
			virtual void Enter(MonsterState* monsterState, float tick) override;

			virtual void Exit(MonsterState* monsterState, float tick) override;
		};

		class IdleAggressive final : public StateBase
		{
		private:
			float timer;

		public:
			IdleAggressive() :StateBase(State::IDLE_AGGRESSIVE), timer(0.f){}

			virtual void EventCheck(MonsterState* base, float tick) override;

		private:
			virtual void Enter(MonsterState* monsterState , float tick) override;
			virtual void Exit(MonsterState* monsterState, float tick) override;
		};

		class Move final : public StateBase
		{
		public:
			Move() :StateBase(State::MOVE) {}
			Vector rayDirection = Vector::Forward;
			virtual void EventCheck(MonsterState* base,float tick) override;
		private:

			virtual void Enter(MonsterState* monsterState, float tick) override;
			virtual void Exit(MonsterState* monsterState,float tick) override;

			void MoveToPlayer(MonsterState* monsterState);
		};

		class Attack final : public StateBase
		{
		private:
			bool attacked = false;
		public:
			Attack() :StateBase(State::ATTACK) {}

			virtual void EventCheck(MonsterState* base,float tick) override;

		private:

			virtual void Enter(MonsterState* monsterState, float tick) override;
			virtual void Exit(MonsterState* monsterState, float tick) override;
		};

		class Death final : public StateBase
		{
		public:
			Death() :StateBase(State::DEATH) {}

			virtual void EventCheck(MonsterState* base, float tick) override;

		private:

			virtual void Enter(MonsterState* monsterState, float tick) override;
			virtual void Exit(MonsterState* monsterState, float tick) override;
		};

		class Attacked : public StateBase
		{
		public:
			Attacked() : StateBase(State::ATTACKED){}

			virtual void EventCheck(MonsterState* base, float tick) override;

		private:

			virtual void Enter(MonsterState* monsterState, float tick) override;
			virtual void Exit(MonsterState* monsterState, float tick) override;
		};


		class MonsterState : public ComponentBase
		{
		public:

			std::string hitSound = "MonsterAttacked2";

			float monsterHp = 55.f;
			float attackPower = 17.f;

			float attackDelay = 1.f;
			float armor= 0.2f;

			float detectRange = 12.f;
			float attackRange = 2.f;
			float moveSpeed = 1.f;
			float moveSpeedThisFrame = 0.f;

			float attackMotionLength = 2.f;

			bool enterAttackState = false;
			
			float attackTimer = 0.f;

			bool avoidingObstacle = false;
			float changeDirTimer = 0.f;

			bool getAttack = false;

			bool isStartVibe = false;

			//이번 프레임에 이동할 방향
			Vector moveDir = Vector::Zero;
			
			//레이 방향 (플레이어있는곳)
			Vector targetDir = Vector::Forward;

			State currState;
			std::map<State, std::shared_ptr<StateBase>> stateList;

			std::shared_ptr<Transform> transform;
			std::shared_ptr<ObstacleState> obstacle;

			std::shared_ptr<GameObject> wall;

		public:

			Event currEvent = Event::NONE;
			bool isPlayerInAttackRange = false; // collider onExit에서 업데이트
			float dyingAnimationTime = 2.f;

		private:
			GraphicsEngineSpace::Canvas* debugCanvas;

		public:
			MonsterState(std::weak_ptr<GameObject> obj);
			~MonsterState();

			inline void SetCurrentState(State target) { this->currState = target; };
			inline State GetCurrentState() { return this->currState; }
			inline std::shared_ptr<StateBase> GetStatePtr(State state) { return stateList[state]; }

			inline std::shared_ptr<Transform> GetTransform() { return this->transform; }
			
			inline std::shared_ptr<GameObject> GetMonsterAttack() { return this->GetGameObject()->GetChild(TEXT("MonsterAttack"));}


			void SetCurrentObstacle(std::shared_ptr<ObstacleState> obj);
			std::shared_ptr<ObstacleState> GetObstacleState();
			
			float GetMoveSpeed();
			void SetRayDirection(Vector dir);

			void SetAnimation(const std::string & name);
			void UpdateColliderRadius();
			void DestroyMonster();

			void GetDamage(int damage);

			bool CheckObstacle();

			void SetDirectionCollider();
			void ReGenerate(Vector pos);

			virtual void Awake() override;
			virtual void Start() override;

			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

		private:
			void UpdateTimer(float tick);
			void UpdateMoveDirection(float tick);
		};
	}
}

