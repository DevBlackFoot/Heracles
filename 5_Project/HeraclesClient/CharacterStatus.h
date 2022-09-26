#pragma once

#include "ComponentBase.h"

namespace Heracles
{
	namespace Character
	{
		enum class Message
		{
			COMPLETE = 0,
			CONTINUE
		};

		enum class Event
		{

		};

		enum class State
		{
			DODGE = 0,
			ATTACK,
			ROTATE,
			MOVE,
			SKILL,
			INTERACT,
			IDLE ,
			DIE,
		};

	}


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

	public:
		CharacterState(std::weak_ptr<GameObject> _gameObject);
		virtual ~CharacterState();

		int GetHp();
		void UpdateHp(int scale);
		void SetHp(int hp);

		float GetArmor();
		void UpdateArmor(float scale);
		void SetArmor(float armor);

		int GetAttackPower();
		void UpdateAttackPower(int scale);
		void SetAttackPower(int attackPower);

		float GetAttackDelay();
		void UpdateAttackDelay(float scale);
		void SetAttackDelay(float attackDelay);

		int GetMoveSpeed();
		void UpdateMoveSpeed(int scale);
		void SetMoveSpeed(int movespeed);

		void SetInvincabiliity(bool b);
	};
}


