#pragma once

#include "ComponentBase.h"


namespace Heracles
{
	//class PlayerState : public ComponentBase
	//{
	//private:
	//	std::shared_ptr<Player::PlayerStateMachine> stateMachine;

	//public:
	//	// 체력과 방어력
	//	int Hp;
	//	float armor;

	//	//공격
	//	int attackPower;
	//	float attackDelay;
	//	float comboDelay; // 이때는 이동키만 먹힘
	//	int comboStack;

	//	// 이동
	//	int moveSpeed;		   //
	//	int rotateSpeed;      // 회전속도
	//	float worldYRotation; // 현재 캐릭터가 바라보는 방향

	//	// 회피기
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

