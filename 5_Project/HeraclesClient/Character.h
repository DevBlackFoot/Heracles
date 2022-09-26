#pragma once
#include "GameObject.h"
#include "SceneBase.h"

namespace Heracles
{

	class Character
	{
	private:

		// 체력과 방어력
		int HP;
		float armor;

		//공격
		int attackPower;
		int attackSpeed;
	
		// 이동
		int moveSpeed;
	
		// 회피기
		int flashDistance;
		int flashDelay;

		bool isInvinsable;

		std::shared_ptr<GameObject> gameObject;

	public:
		Character();
		~Character();

		// 게임시작 최초 실행
		void Initialize(tstring name, tstring tag, uint32 layer, SceneBase* currentScene);

		// 게임오버 후 재시작할 떄 실행
		void Resurrect();

		// 다음 맵으로 갈때 실행
		void NextMap();

		void SetToScene(SceneBase* scene); // get curr scene-> add gameobj


	private:

		void Move(float scale);

		void Attack();

		void OnAttacked(float Damage);

		void Die();


	private:

	};

}


