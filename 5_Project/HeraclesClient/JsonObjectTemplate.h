#pragma once
#include "JsonDoc.h"

/// ���̽� �Ľ��� ���� ���� �Լ����� �����ص� utils �Լ� ���.
/// �⺻������ Ŭ���̾�Ʈ ������Ʈ�� �ڵ����� ���� �ٿ��ִ� �Ϳ� ������ �ִ�.


namespace Heracles
{
	namespace JsonObject
	{
		// �⺻������ ���� ���̽� ������ �����ͼ� ������ ���� ����� �Լ�
			// TODO �� �Լ��� ���������� ���̽� ���ϰ� ������ ���̺��� �ϼ��Ǹ� ���� ����.
			// �ϴ��� ���� ���� �Լ� ���� ���� ����ô�. => CreateEmpty�� ���⼭ / ������ ������ ������ �Ʒ��� �Լ��鿡��.
		void BuildJsonMap(const std::shared_ptr<SceneBase>& scene, const tstring& mapJsonPath, const tstring& objInfoJsonPath, const tstring& eventInfoJsonPath = TEXT(""));

		// �⺻���� �̸�, �±� ���� ������ �ִ� �Լ�.
		void SetObjBasicInfo(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// ������Ʈ Ʈ������ ����
		void SetObjTransform(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// ������Ʈ �Ž� ������ ����
		void SetObjMeshRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// ������Ʈ ��Ų ������ ���� => �ִϸ��̼��� �����ϸ� �ݵ�� ��Ų �������� ����.
		void SetObjSkinnedRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// �ִϸ����� ����
		void SetObjAnimator(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// �� �ݶ��̴� ����
		void SetObjSphereCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// �ڽ� �ݶ��̴� ����.
		void SetObjBoxCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// �÷��̾� ������Ʈ ����
		void SetObjPlayer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);
		// �÷��̾� �ڽ� -> ���� ���� => ��Ե� ������ �÷��̾ ã�Ƽ� �־�����..
		void SetObjPlayerWeapon(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj);

		// ī�޶� ���� -> ��Ե� ������ �÷��̾ ã�Ƽ� �־�����..
		void SetObjCamera(const std::shared_ptr<SceneBase>& scene, const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj);

		// ���� ������Ʈ ���� - ����
		void SetObjAltar(const std::shared_ptr<GameObject>& gameObj);
		// ��ֹ� + �� ���� -> �ش� ������Ʈ�� Init�� ����.
		void SetObjObstacle(const std::shared_ptr<GameObject>& gameObj);

		// ���� ����
		void SetObjMonster(const std::shared_ptr<GameObject>& gameObj);
		// ���� ���� ���� ����
		void SetObjMonsterAttack(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& MonsterObj);

		// Scene controller ����
		void SetObjSceneController(const std::shared_ptr<GameObject>& gameObj);

		// ���� ����
		void SetObjBoss(const std::shared_ptr<GameObject>& gameObj);
		void SetObjBossAttack(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& bossObj);
	}
}
