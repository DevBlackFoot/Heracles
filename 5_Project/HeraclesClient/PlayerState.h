#pragma once

#include "ComponentBase.h"


namespace Heracles
{
	//class PlayerState : public ComponentBase
	//{
	//private:
	//	std::shared_ptr<Player::PlayerStateMachine> stateMachine;

	//public:
	//	// ü�°� ����
	//	int Hp;
	//	float armor;

	//	//����
	//	int attackPower;
	//	float attackDelay;
	//	float comboDelay; // �̶��� �̵�Ű�� ����
	//	int comboStack;

	//	// �̵�
	//	int moveSpeed;		   //
	//	int rotateSpeed;      // ȸ���ӵ�
	//	float worldYRotation; // ���� ĳ���Ͱ� �ٶ󺸴� ����

	//	// ȸ�Ǳ�
	//	int dodgeDistance;
	//	int dodgeDelay;       //sec

	//	bool isInvincable;

	//public:
	//	PlayerState(std::weak_ptr<GameObject> obj);
	//	~PlayerState();

	//	virtual void Awake() override;
	//	virtual void Update(float tick) override;

	//public:
	//	inline const std::shared_ptr<Player::PlayerStateMachine> GetStateMachine() { return this->stateMachine; }
	//	
	//	//inline std::shared_ptr<Animator> GetAnimator() { return this->GetGameObject()->GetComponent<Animator>(); }

	//	};

}

