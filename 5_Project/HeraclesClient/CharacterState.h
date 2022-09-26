#pragma once

#include "ComponentBase.h"
#include "CharacterStates.h"
namespace Heracles
{
	

	using namespace GameEngineSpace;

	class CharacterState : public ComponentBase
	{
	private:
		// 체력과 방어력
		int Hp;
		float armor;

		//공격
		int attackPower;
		float attackDelay;
		float comboDelay; // 이때는 이동키만 먹힘
		int comboStack;

		// 이동
		int moveSpeed;		   //
		int rotateSpeed;      // 회전속도
		float worldYRotation; // 현재 캐릭터가 바라보는 방향

		// 회피기
		int dodgeDistance;
		int dodgeDelay;       //sec

		bool isInvincable;

	private:
		std::map<Character::StateTag,Character::StateBase*> stateList;
		Character::StateBase* currentState;


	public:
		CharacterState(std::weak_ptr<GameObject> _gameObject);
		virtual ~CharacterState();

		void SetState(Character::StateTag tag);

		virtual void Awake() override;
		virtual void Update(float tick) override;

	private:
		void CheckState(); // invoke in update

	public:


		int GetHp();
		void UpdateHp(int scale);
		void SetHp(int hp);

		float GetArmor();
		void UpdateArmor(float scale);
		void SetArmor(float armor);

		int GetAttackPower();
		void UpdateAttackPower(int scale);
		void SetAttackPower(float attackPower);

		float GetAttackDelay();
		void UpdateAttackDelay(float scale);
		void SetAttackDelay(float attackDelay);

		int GetMoveSpeed();
		void UpdateMoveSpeed(int scale);
		void SetMoveSpeed(int movespeed);

		void SetInvincabiliity(bool b);
	};
}


