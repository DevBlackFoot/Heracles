#pragma once


/*
http://icecola89.blogspot.com/2016/05/c.html
https://www.aleksandrhovhannisyan.com/blog/implementing-a-finite-state-machine-in-cpp/

*/

namespace Heracles
{
	class CharacterState;


	namespace Character
	{
		enum class StateTag
		{
			DODGE = 0,
			ATTACK,
			MOVE,
			SKILL,
			INTERACT,
			IDLE,
			DIE,
		};

		class StateBase abstract
		{
		public:

			virtual void Enter(CharacterState* character) = 0;
			virtual void Event(CharacterState* character) = 0;
			virtual void Exit(CharacterState* character) = 0;
			virtual ~StateBase() {};
		};

		class Attack : public StateBase
		{
		private:
			bool isAttacking;
			uint32 comboStack;
					
		public:
			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

		class Idle final: public StateBase
		{
		public:
			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

		class Move final: public StateBase
		{
		public:
			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

		// 이 클래스는 뭐하는거죠 ..?
		class Dodge final : public StateBase
		{
		public:
			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

		class Interact final : public StateBase
		{
		public:

			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

		class Skill final : public StateBase
		{
		public:
			virtual void Enter(CharacterState* character) override;
			virtual void Event(CharacterState* character) override;
			virtual void Exit(CharacterState* character) override;
		};

	}

}

