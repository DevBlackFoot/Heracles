#include "pch.h"
#include "CharacterStatus.h"


namespace Heracles
{
	CharacterState::CharacterState(std::weak_ptr<GameObject> gameObject) :
		ComponentBase(gameObject, ComponentType::MONOBEHAVIOR)
		, Hp(100)
		, armor(0)
		, attackPower(1)
		, attackDelay(0.5)
		, comboDelay(0.3)
		, moveSpeed(1)
		, rotateSpeed(1)
		, dodgeDistance(4)
		, dodgeDelay(2)
		, comboStack(0)
		, worldYRotation(0)
		, isInvincable(true)
	{
	}

	CharacterState::~CharacterState()
	{
	}
	int CharacterState::GetHp()
	{
		return this->Hp;
	}
	void CharacterState::UpdateHp(int scale)
	{
		this->Hp += scale;
	}
	void CharacterState::SetHp(int hp)
	{
		this->Hp = hp;
	}

	int CharacterState::GetAttackPower()
	{
		return this->attackPower;
	}
	void CharacterState::UpdateAttackPower(int scale)
	{
		this->attackPower += scale;
	}
	void CharacterState::SetAttackPower(int attackPower)
	{
		this->attackPower = attackPower;
	}

	float CharacterState::GetArmor()
	{
		return this->armor;
	}

	void CharacterState::UpdateArmor(float scale)
	{
		this->armor += scale;
	}

	void CharacterState::SetArmor(float armor)
	{
		this->armor = armor;
	}

	int CharacterState::GetMoveSpeed()
	{
		return this->moveSpeed;
	}
	void CharacterState::UpdateMoveSpeed(int scale)
	{
		this->moveSpeed += scale;
	}
	void CharacterState::SetMoveSpeed(int movespeed)
	{
		this->moveSpeed = movespeed;
	}
	void CharacterState::SetInvincabiliity(bool b)
	{
		this->isInvincable = b;
	}
	float CharacterState::GetAttackDelay()
	{
		return this->attackDelay;
	}
	void CharacterState::UpdateAttackDelay(float scale)
	{
		this->attackDelay += scale;
	}
	void CharacterState::SetAttackDelay(float attackDelay)
	{
		this->attackDelay = attackDelay;
	}
}
