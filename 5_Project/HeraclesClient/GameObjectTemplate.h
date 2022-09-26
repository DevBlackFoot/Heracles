#pragma once

namespace Heracles
{
	namespace InGameObject
	{
		std::shared_ptr<GameObject> CreatePlayer(SceneBase* scene);

		std::shared_ptr<GameObject> CreateSwordAttackNormal(SceneBase* scene);

		std::shared_ptr<GameObject> CreateCamera(SceneBase* scene);

		std::shared_ptr<GameObject> CreateAltar(SceneBase* scene);
		std::shared_ptr<GameObject> CreateWeaponAltar(SceneBase* scene);

		std::shared_ptr<GameObject> CreateWeapon(SceneBase* scene);

		std::shared_ptr<GameObject> CreateTrap(SceneBase* scene, Vector pos);

		std::shared_ptr<GameObject> CreateRewardBox(SceneBase* scene, Vector pos, int index = 0);

		std::shared_ptr<GameObject> CreateRewardWithIndex(SceneBase* scene, int index);

		// HP ��ȭ
		std::shared_ptr<GameObject> CreateHP(SceneBase* scene, Vector pos = Vector::Zero);
		// ��� ����
		std::shared_ptr<GameObject> CreateMemoryGem(SceneBase* scene, Vector pos = Vector::Zero);
		//���Ⱝȭ�� ���̴� ��ȭ
		std::shared_ptr<GameObject> CreateMoney(SceneBase* scene, Vector pos = Vector::Zero);

		//�׽�Ʈ��
		std::shared_ptr<GameObject> CreateObstacle(SceneBase* scene, Vector pos, float width, float height);

		std::shared_ptr<GameObject> CreateMonster(SceneBase* scene, Vector pos);

		std::shared_ptr<GameObject> CreateBigMonster(SceneBase* scene, Vector pos);

		std::shared_ptr<GameObject> CreateLongRangeMonster(SceneBase* scene, Vector pos);

		std::shared_ptr<GameObject> CreateSceneController(SceneBase* scene);

		std::shared_ptr<GameObject> CreateBoss(SceneBase* scene);

		std::shared_ptr<GameObject> CreateEntrance(SceneBase* scene, tstring name, Vector pos); // ������� ����

		std::shared_ptr<GameObject> CreateEntranceWithIndex(SceneBase* scene, Vector pos, int index);

		// �� or ���� �ݶ��̴� ����
		std::shared_ptr<GameObject> CreateWall(SceneBase* scene, Vector pos, float width, float height);

		std::shared_ptr<GameObject> CreateStaticMesh(SceneBase* scene, std::string name, std::string path);
	}
}

