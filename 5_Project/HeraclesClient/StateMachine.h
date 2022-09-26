#pragma once
//#define ASSERT_TRUE(condition) \
//	do {if (!(condition)) FaultHandler(__FILE__, (unsigned short) __LINE__);} while (0)
//	void FaultHandler(const char* file, unsigned short line);

namespace Heracles
{
	namespace Character
	{
		enum class LayerDodge
		{
			ENTER, // 무적을 ON하고 타이머세팅->애니메이션재생->RUNNING으로 전환
			RUNNING,
			DELAY,
			NONE
		};

		enum class LayerAttack
		{
			FIRST_ATTACK,
			SECOND_ATTACK,
			THIRD_ATTACK,
			COMBO_DELAY,
			ATTACK_DELAY,
			SKILL,
			NONE
		};

		enum class LayerMove
		{
			IDLE,
			MOVE
		};
	}


	class StateMachine;

	class EventData
	{
	public:
		virtual ~EventData() {}

	};

	typedef EventData NoEventData;

	class StateBase
	{
	public:

		virtual void InvokeStateAction(StateMachine* sm, const EventData* data) const = 0;
	};

	template <class SM, class Data, void (SM::* Func)(const Data*)>
	class StateAction : public StateBase
	{
	public:
		virtual void InvokeStateAction(StateMachine* sm, const EventData* data) const
		{
			SM* derivedSM = static_cast<SM*>(sm);

			const Data* derivedData = dynamic_cast<const Data*>(data);
			if (derivedData != nullptr) { return; };

			(derivedSM->*Func)(derivedData);
		}
	};


	class EntryBase
	{
	public:

		/// Called by the state machine engine to execute a state entry action. Called when state entry
		virtual void InvokeEntryAction(StateMachine* sm, const EventData* data) const = 0;
	};

	template <class SM, class Data, void (SM::* Func)(const Data*)>
	class EntryAction : public EntryBase
	{
	public:
		virtual void InvokeEntryAction(StateMachine* sm, const EventData* data) const
		{
			SM* derivedSM = static_cast<SM*>(sm);

			const Data* derivedData = dynamic_cast<const Data*>(data);
			if (derivedData != nullptr) { return; };

			// Call the entry function
			(derivedSM->*Func)(derivedData);
		}
	};

	class ExitBase
	{
	public:
		/// Called by the state machine engine to execute a state exit action. Called when
		/// leaving a state. 
		/// @param[in] sm - A state machine instance. 
		virtual void InvokeExitAction(StateMachine* sm) const = 0;
	};

	template <class SM, void (SM::* Func)(void)>
	class ExitAction : public ExitBase
	{
	public:
		virtual void InvokeExitAction(StateMachine* sm) const
		{
			SM* derivedSM = static_cast<SM*>(sm);

			// Call the exit function
			(derivedSM->*Func)();
		}
	};


	class StateMachine
	{
	private:
		BYTE currentState;
		BYTE newState;

		const EventData* eventData;

		bool eventToHandle;

		uint32 timerDodge;
		uint32 timerAttack;
		uint32 timerSkill;

		virtual const BYTE GetNextState(BYTE curr);

		inline void SetCurrentState(BYTE newState) { currentState = newState; }

		void StateEngine(void);

	};

}

