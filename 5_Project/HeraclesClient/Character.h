#pragma once
#include "GameObject.h"
#include "SceneBase.h"

namespace Heracles
{

	class Character
	{
	private:

		// ü�°� ����
		int HP;
		float armor;

		//����
		int attackPower;
		int attackSpeed;
	
		// �̵�
		int moveSpeed;
	
		// ȸ�Ǳ�
		int flashDistance;
		int flashDelay;

		bool isInvinsable;

		std::shared_ptr<GameObject> gameObject;

	public:
		Character();
		~Character();

		// ���ӽ��� ���� ����
		void Initialize(tstring name, tstring tag, uint32 layer, SceneBase* currentScene);

		// ���ӿ��� �� ������� �� ����
		void Resurrect();

		// ���� ������ ���� ����
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


