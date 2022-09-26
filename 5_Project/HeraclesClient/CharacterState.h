#pragma once

#include "ComponentBase.h"
#include "CharacterStates.h"
namespace Heracles
{
	

	using namespace GameEngineSpace;

	class CharacterState : public ComponentBase
	{
	private:
		// ü�°� ����
		int Hp;
		float armor;

		//����
		int attackPower;
		float attackDelay;
		float comboDelay; // �̶��� �̵�Ű�� ����
		int comboStack;

		// �̵�
		int moveSpeed;		   //
		int rotateSpeed;      // ȸ���ӵ�
		float worldYRotation; // ���� ĳ���Ͱ� �ٶ󺸴� ����

		// ȸ�Ǳ�
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


