#pragma once
#include "BehaviourTree.h"
#include <type_traits>
namespace Heracles
{
	namespace Boss
	{
		class BossTree : public ComponentBase
		{
		private:
			std::shared_ptr<Selector> root;

		public:
			bool collisionEnterToWall = false;
			bool getDamage = false;
			int hitCount = 0;
			float hp = 1000.f;
			float speed = 1.5f;

			std::vector<std::shared_ptr<GameObject>> bullets;

			void GetDamage(float damage);
			
			bool onStart = false;

		public:
			BossTree(std::weak_ptr<GameObject> obj);

			void ActivateBullet(int index);

			void DeactivateBulletAll();

			inline void AddBullet(std::shared_ptr<GameObject> obj) { this->bullets.emplace_back(obj); }

			virtual void Awake() override;
			virtual void Start() override;
			virtual void PreUpdate(float tick) override;
			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			template<typename TComposite, typename TCondition, typename TAction >
			static std::shared_ptr<TComposite> MakeSequence()
			{
				static_assert(std::is_base_of<CompositeNode, TComposite>::value, "composite-type-error");
				static_assert(std::is_base_of<Condition, TCondition>::value, "condition-type-error");
				static_assert(std::is_base_of<Action, TAction>::value, "action-type-error");

				std::shared_ptr<TComposite> com = std::make_shared<TComposite>();

				std::shared_ptr<TAction> action = std::make_shared<TAction>();
				com->AddChild(std::make_shared<TCondition>(action));
				com->AddChild(action);
				return com;
			}

			template<typename TComposite, typename TCondition, typename TAction, typename TAction2>
			static std::shared_ptr<TComposite> MakeAttackSequence()
			{
				static_assert(std::is_base_of<CompositeNode, TComposite>::value, "composite-type-error");
				static_assert(std::is_base_of<Condition, TCondition>::value, "condition-type-error");
				static_assert(std::is_base_of<Action, TAction>::value, "action-type-error");
				static_assert(std::is_base_of<Action, TAction2>::value, "action2-type-error");


				std::shared_ptr<TComposite> com = std::make_shared<TComposite>();

				std::shared_ptr<Selector> selector = std::make_shared<Selector>();

				std::shared_ptr<TAction> action = std::make_shared<TAction>();
				std::shared_ptr<TAction2> action2 = std::make_shared<TAction2>();

				selector->AddChild(action);
				selector->AddChild(action2);

				com->AddChild(std::make_shared<TCondition>(selector));
				com->AddChild(selector);
				return com;
			}


			template<typename TComposite, typename TCondition>
			static std::shared_ptr<TComposite> MakeSequenceWithInstance(std::shared_ptr<Node> inst)
			{
				static_assert(std::is_base_of<CompositeNode, TComposite>::value, "composite-type-error");
				static_assert(std::is_base_of<Condition, TCondition>::value, "condition-type-error");

				std::shared_ptr<TComposite> com = std::make_shared<TComposite>();
				com->AddChild(std::make_shared<TCondition>());
				com->AddChild(inst);
				return com;
			}

			template<typename TComposite, typename TCondition, typename TAction1, typename TAction2>
			static std::shared_ptr<TComposite> MakeContinuousAction()
			{
				static_assert(std::is_base_of<CompositeNode, TComposite>::value, "composite-type-error");
				static_assert(std::is_base_of<Condition, TCondition>::value, "condition-type-error");
				static_assert(std::is_base_of<Action, TAction1>::value, "composite-type-error");
				static_assert(std::is_base_of<Action, TAction2>::value, "condition-type-error");

				std::shared_ptr<TComposite> com = std::make_shared<TComposite>();

				std::shared_ptr<Selector> selector = std::make_shared<Selector>();
				selector->AddChild(std::make_shared<TAction1>());
				selector->AddChild(std::make_shared<TAction2>());

				com->AddChild(std::make_shared<TCondition>(selector));
				com->AddChild(selector);
				return com;
			}


		private:
			std::shared_ptr<Node> TestPhase();
			std::shared_ptr<Node> TestPhase1Module();
			std::shared_ptr<Node> TestPhase2Module();
			std::shared_ptr<Node> TestAttack1();
			std::shared_ptr<Node> TestAttack2();

		private:
			std::shared_ptr<Node> PhaseSelector();

		private:

			std::shared_ptr<Node> Phase1Module();
			std::shared_ptr<Node> Phase1AttackSequence();

		private:

			std::shared_ptr<Node> Phase2Module();
			std::shared_ptr<Node> Phase2AttackSequence();

		private:

			std::shared_ptr<Node> Phase3Module();
			std::shared_ptr<Node> Phase3AttackSequence();

		private:

			std::shared_ptr<Node> StunSequence();
			std::shared_ptr<Node> MoveSequence();

		};


		class BossStart final : public Action
		{
		private:
			float cameraFocusTime = 10.f;
		public:
			virtual bool Run() override;
		};

		class Phase1 : public Condition
		{
		public:
			virtual bool Run() override;
		};

		class Phase2 : public Condition
		{
		public:
			virtual bool Run() override;
		};

		class Phase3 : public Condition
		{
		public:
			virtual bool Run() override;
		};

		class CheckIdle : public Condition
		{
		public:
			CheckIdle(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;

		};

		class CheckDeath : public Condition
		{
		public:
			CheckDeath() {};
			CheckDeath(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;
		};

		class CheckHitCount : public Condition
		{
		private:
			int threshold = 6;
			int curr = 0;
		public:
			CheckHitCount(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;
		};



		class CloseRange : public Condition
		{
		private:
			float closeAttackDist = 10.f;
		public:
			CloseRange() {}
			CloseRange(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;
		};

		class StunCondition : public Condition
		{
		public:
			StunCondition() {}
			StunCondition(std::shared_ptr<Node> action) : Condition(action) {}
			virtual bool Run() override;
		};

		class JumpRange : public Condition
		{
		private:
			float closeAttackDist = 10.f;
			float moveDist = 20.f;
			float jumpAttackDist = 30.f;
		public:
			JumpRange() {}
			JumpRange(std::shared_ptr<Node> action) : Condition(action) {}
			virtual bool Run() override;
		};

		class DashRange : public Condition
		{
		private:
			float mapLeft;
			float mapRight;
			float mapTop;
			float mapDown;

			float dashAttackDist = 50.f;

		public:
			DashRange() {}
			DashRange(std::shared_ptr<Node> action) : Condition(action) {}
			virtual bool Run() override;
		};

		class MoveRange : public Condition
		{
		public:
			MoveRange() {}
			MoveRange(std::shared_ptr<Node> action) : Condition(action) {}
			virtual bool Run() override;
		};

		class Stun : public Action
		{
		private:
			float stunTime = 5.f;
		public:
			virtual bool Run() override;
		};

		class CounterAttack : public Action
		{
		private:
			float tempTime = 2.f;
			bool attacked = false;
		public:
			virtual bool Run() override;
		};

		class Move : public Action
		{
		private:
			float speed = 8.f;
		public:
			virtual bool Run() override;
		};

		class Idle : public Action
		{
		public:
			static bool afterAttack;
		private:
			float idleTime = 1.5f;
		public:
			virtual bool Run() override;
		};


		class CloseAttack : public Action
		{
		private:
			float attackTime = 3.f;
			bool attacked = false;
		public:
			virtual bool Run() override;
		};

		class CloseJumpAttack : public Action
		{
		private:
			float attackTime = 1.5f;
			bool attacked = false;

			Vector initialPos;
			Vector targetPos;
			
			float dx;
			float dz;
		public:
			virtual bool Run() override;

		private:
			void ParabolicMove();
		};

		class LongJumpAttack : public Action
		{
		private:
			float attackTime = 1.7f;
			float additionalDistance = 0.f;

			Vector initialPos;
			Vector targetPos;

			float dx = 0.f;
			float dz = 0.f;

		public:
			virtual bool Run() override;

		private:
			void ParabolicMove();
		};

		class DashAttackStart : public Action
		{
		private:
			static bool dashAttackRunning;
			friend class DashAttack;
		public:
			virtual bool Run() override;
		private:
			void DashStartMove();
		};


		class DashAttack : public Action
		{
		private:
			float dashSpeed = 30.f;
		public:
			virtual bool Run() override;

		private:
			void DashMove();
		};

		class Death : public Action
		{
		private:
			float deathDelay = 2.f;
		public:
			virtual bool Run() override;
		};

		class ShockWave : public Action
		{
		private:

			bool attacked = false;

		public:
			virtual bool Run() override;
		};

		class EnterPhase2 : public Condition
		{
		public:
			EnterPhase2(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;
		};


		class Phase2Roar : public Action
		{
		private:
			float actionTime = 1.f;
		public:
			virtual bool Run() override;
		};

		class EnterPhase3 : public Condition
		{
		public:
			EnterPhase3(std::shared_ptr<Node> node) : Condition(node) {}
			virtual bool Run() override;
		};

		class Phase3Roar : public Action
		{
		private:
			float actionTime = 2.f;
		public:
			virtual bool Run() override;
		};

		class Barrage : public Action
		{
		private:
			float nextFireTime = 2.0f;
			float increment = 0.3f;

			int currIndex = 0;
			int maxIndex = 10;

		public:
			virtual bool Run() override;
		};

	}
}

