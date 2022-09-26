#pragma once

#pragma once
#include "ObstacleState.h"

namespace Heracles
{
	using namespace Obstacle;
	namespace LongRangeMonster
	{
		class LongRangeMonsterState;

		enum class State
		{
			IDLE = 1,
			IDLE_AGGRESSIVE,
			MOVE,
			ATTACK,
			DEATH
		};

		enum class Event
		{
			DETECT,
			DEATH,
			NONE
		};

		class StateBase abstract
		{
		protected:
			bool running = false;
			State tag;

		public:
			float attackTime = 2.f; // ���ݸ�� ���ӽð�
			float attackTimer = 0.f;
		public:
			StateBase(State state)
			{
				this->tag = state;
			}
			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) = 0;

		private:
			virtual void Enter(LongRangeMonsterState* monsterState, float tick) = 0;
			virtual void Exit(LongRangeMonsterState* monsterState, float tick) = 0;
		};

		class Idle final : public StateBase
		{
		public:
			Idle() :StateBase(State::IDLE) {}
			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) override;
		private:
			virtual void Enter(LongRangeMonsterState* monsterState, float tick) override;

			virtual void Exit(LongRangeMonsterState* monsterState, float tick) override;
		};

		class IdleAggressive final : public StateBase
		{
		private:
			float timer;

		public:
			IdleAggressive() :StateBase(State::IDLE_AGGRESSIVE), timer(0.f) {}

			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) override;

		private:
			virtual void Enter(LongRangeMonsterState* monsterState, float tick) override;
			virtual void Exit(LongRangeMonsterState* monsterState, float tick) override;
		};

		class Move final : public StateBase
		{
		public:
			Move() :StateBase(State::MOVE) {}
			Vector rayDirection = Vector::Forward;
			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) override;
		private:

			virtual void Enter(LongRangeMonsterState* monsterState, float tick) override;
			virtual void Exit(LongRangeMonsterState* monsterState, float tick) override;

			void MoveToPlayer(LongRangeMonsterState* monsterState);
		};

		class Attack final : public StateBase
		{
		public:
			Attack() :StateBase(State::ATTACK) {}

			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) override;

		private:

			virtual void Enter(LongRangeMonsterState* monsterState, float tick) override;
			virtual void Exit(LongRangeMonsterState* monsterState, float tick) override;
		};

		class Death final : public StateBase
		{
		public:
			Death() :StateBase(State::DEATH) {}

			virtual void EventCheck(LongRangeMonsterState* monsterState, float tick) override;

		private:

			virtual void Enter(LongRangeMonsterState* monsterState, float tick) override;
			virtual void Exit(LongRangeMonsterState* monsterState, float tick) override;
		};

		class LongRangeMonsterState : public ComponentBase
		{
		public:

			float monsterHp = 7.f;
			float attackPower = 2.f;

			float attackDelay = 1.f;

			float detectRange = 5.f;
			float attackRange = 1.f;
			float moveSpeed = 1.f;
			float moveSpeedThisFrame = 0.f;

			float attackMotionLength = 2.f;

			bool enterAttackState = false;

			float attackTimer = 0.f;

			bool avoidingObstacle = false;
			float changeDirTimer = 0.f;

			//�̹� �����ӿ� �̵��� ����
			Vector moveDir = Vector::Zero;

			//���� ���� (�÷��̾��ִ°�)
			Vector targetDir = Vector::Forward;

			//������ �ִ� źȯ�� 
			std::vector<std::shared_ptr<GameObject>> bullets;

			int bulletIndex = 0;

			State currState;
			std::map<State, std::shared_ptr<StateBase>> stateList;

			std::shared_ptr<Transform> transform;
			std::shared_ptr<ObstacleState> obstacle;

		public:

			Event currEvent = Event::NONE;
			bool isPlayerInAttackRange = false; // collider onExit���� ������Ʈ
			float dyingAnimationTime = 2.f;

		public:
			LongRangeMonsterState(std::weak_ptr<GameObject> obj);
			~LongRangeMonsterState();

			inline void AddBullet(std::shared_ptr<GameObject> obj) { this->bullets.emplace_back(obj); }

			inline void SetCurrentState(State target) { this->currState = target; };
			inline State GetCurrentState() { return this->currState; }
			inline std::shared_ptr<StateBase> GetStatePtr(State state) { return stateList[state]; }

			inline std::shared_ptr<Transform> GetTransform() { return this->transform; }

			inline std::shared_ptr<GameObject> GetMonsterAttack() { return this->GetGameObject()->GetChild(TEXT("MonsterAttack")); }


			void SetCurrentObstacle(std::shared_ptr<ObstacleState> obj);
			std::shared_ptr<ObstacleState> GetObstacleState();

			float GetMoveSpeed();
			void SetRayDirection(Vector dir);

			void SetAnimation(const std::string& name);
			void UpdateColliderRadius();
			void DestroyMonster();

			void GetDamage(int damage);

			bool CheckObstacle();

			void ReGenerate(Vector pos = Vector::Zero);

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


