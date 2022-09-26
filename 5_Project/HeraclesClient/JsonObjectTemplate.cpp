#include "pch.h"
#include "JsonObjectTemplate.h"

// �� ������ �ʿ��� ���� ������Ʈ��.
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "RayCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "JsonDoc.h"

// Ŭ���̾�Ʈ ������Ʈ
#include "PlayerStateMachine.h"
#include "PlayerController.h"
#include "InventoryUI.h"
#include "RayCollider.h"
#include "PlayerCamera.h"
#include "PlayerCollider.h"

//#include "AltarState.h"
#include "AltarMonoBehaviour.h"

#include "DefaultWeapon.h"
#include "WeaponMonoBehaviour.h"

#include "MonsterState.h"
#include "MonsterMonoBehaviour.h"
#include "MonsterAttack.h"

#include "BossTree.h"
#include "BossMonoBehaviour.h"
#include "BossAttackMonoBehaviour.h"

#include "GameState.h"
#include "TestBossController.h"

#include "SceneController.h"

namespace Heracles
{
	namespace JsonObject
	{
		void BuildJsonMap(const std::shared_ptr<SceneBase>& scene, const tstring& mapJsonPath,
			const tstring& objInfoJsonPath, const tstring& eventInfoJsonPath)
		{
			// Json ������ �Ľ����ݽô�.
			std::shared_ptr<JsonDoc> mapJson = std::make_shared<JsonDoc>();

			std::string strJson;
			strJson.assign(mapJsonPath.begin(), mapJsonPath.end());

			if (mapJson->LoadJsonFromFile(strJson) == false)
			{
				OutputDebugString(TEXT("Json Load Fail"));
				return;
			}

			// ������Ʈ json �Ľ�
			std::shared_ptr<JsonDoc> objInfoJson = std::make_shared<JsonDoc>();

			strJson.assign(objInfoJsonPath.begin(), objInfoJsonPath.end());
			if (objInfoJson->LoadJsonFromFile(strJson) == false)
			{
				OutputDebugString(TEXT("Json Load Fail"));
				return;
			}

			// �ڽ��� �ٴ� ������Ʈ���� ĳ���ϱ� ���� ������ �������ݴϴ�.
			std::shared_ptr<GameObject> player = nullptr;
			std::shared_ptr<GameObject> monster = nullptr;
			std::shared_ptr<GameObject> boss = nullptr;

			// map json ��ü�� �����ݴϴ�.
			int mapObjCount = mapJson->GetDocument().Size();

			for (int i = 0; i < mapObjCount; i++)
			{
				// ������Ʈ�� �����մϴ�.
				std::shared_ptr<GameObject> tmpObj = scene->CreateEmpty();

				// ������ ����� id�� �޾ƿɴϴ�.
				int mapObjID = mapJson->GetValue<int>("ID", i, false);

				// �⺻���� �̸��� �±� + Ʈ�������� �޾ƿɴϴ�.
				SetObjBasicInfo(tmpObj, mapJson, mapObjID);
				SetObjTransform(tmpObj, mapJson, mapObjID);

				// Collider�� �̸��� �����ɴϴ�.
				// �ݶ��̴��� �̸��� ����, �ݶ��̴��� �������ݴϴ�.
				std::string collider = mapJson->GetValue<std::string>("Collider", mapObjID);

				if (collider == "BOX")
				{
					SetObjBoxCollider(tmpObj, mapJson, mapObjID);
				}
				else if (collider == "SPHERE")
				{
					SetObjSphereCollider(tmpObj, mapJson, mapObjID);
				}

				// ���� ������Ʈ ID�� ���� Obj Json�� �����ؼ� ������Ʈ�� �������ݴϴ�.
				int objID = mapJson->GetValue<int>("ObjectID", mapObjID);

				// �������� �����ݴϴ�..
					// => �ش� ���� 10�̸�, �̺�Ʈ�� ���� obj �Դϴ�.
					// -> �ش� ���� 20�̸� Ŭ���̾�Ʈ ������Ʈ�� �ٿ�����մϴ�..
				int objValIdx = objID / 1000;

				// ���� ���� ���θ� �Ǵ��մϴ�
				bool renderable = objInfoJson->GetValue<bool>("Renderable", objID);

				// ���� �������� ������ �������� �������� �ʽ��ϴ�.
				if (renderable == true)
				{
					bool hasAnim = objInfoJson->GetValue<bool>("HasAnim", objID);
					// �ִϸ��̼��� ������ -> Mesh ������
					if (hasAnim != true)
					{
						// Map JSon�� �ƴ� �Ϳ� ����!
						SetObjMeshRenderer(tmpObj, objInfoJson, objID);
					}
					// ������ -> Skinned ������ �� �ִϸ����� �߰�
					else
					{
						SetObjSkinnedRenderer(tmpObj, objInfoJson, objID);
						SetObjAnimator(tmpObj, objInfoJson, objID);
					}
				}

				// ������ ��� �������� �������־��⿡.. ���⼭�� ������Ʈ ������ �������ݴϴ�.
				if (objValIdx == 20)
				{
					// �������� �Ǻ����ݴϴ�..
					objValIdx = objID % 1000;

					switch (objValIdx)
					{
						case 101:
						{
							// �÷��̾� -> ���� ������Ʈ�� �÷��̾�� �������ݴϴ�.
							player = tmpObj;

							SetObjPlayer(tmpObj, objInfoJson, objID);
							break;
						}
						case 102:
						{
							if (player == nullptr)
							{
								OutputDebugString(TEXT("Player not Found"));
								break;
							}

							SetObjPlayerWeapon(tmpObj, player);
							break;
						}

						case 201:
						SetObjAltar(tmpObj);
						break;

						case 202:
						case 203:
						SetObjObstacle(tmpObj);
						break;

						// ���Ϳ� ���� ��� ���� ���� ������� ���ݴϴ�.
						case 301:
						{
							// ���� -> ���� ������Ʈ�� ���ͷ� �������ݴϴ�.
							monster = tmpObj;

							SetObjMonster(tmpObj);
							break;
						}
						case 302:
						{
							if (monster == nullptr)
							{
								OutputDebugString(TEXT("Monster not Found"));
								break;
							}

							SetObjMonsterAttack(tmpObj, monster);
							break;
						}

						case 303:
						{
							// ������ ����.
							boss = tmpObj;

							SetObjBoss(tmpObj);
							break;
						}
						case 304:
						{
							if (boss == nullptr)
							{
								OutputDebugString(TEXT("Boss not Found"));
								break;
							}

							SetObjBossAttack(tmpObj, boss);
							break;
						}

						case 401:
						SetObjCamera(scene, tmpObj, player);
						break;

						case 402:
						SetObjSceneController(tmpObj);
						break;

						default:
						break;
					}
				}

			}
		}

		/**
		 * \brief �̸��� �±� ������ �����ͼ� ���� ������Ʈ�� �־��ִ� �Լ�.
		 * \param gameObj �������� ���� ������Ʈ
		 * \param json �ش� ������Ʈ�� ������ ��� json ���� => depth ����.
		 * \param objID ��� ������Ʈ�� �ִ� json ���� ���� id.
		 */
		void SetObjBasicInfo(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			// �̸� �߰�
			std::string objStr = json->GetValue<std::string>("GameObjectName", objID);
			tstring objTStr;
			objTStr.assign(objStr.begin(), objStr.end());

			gameObj->SetName(objTStr);

			// �±� �߰�
			objStr = json->GetValue<std::string>("GameObjectTag", objID);
			objTStr.assign(objStr.begin(), objStr.end());

			gameObj->SetTag(objTStr);
		}

		/**
		* \brief Json���� ���� �Ľ��ؼ� ���ӿ�����Ʈ�� �־��ݴϴ�.
		* \param gameObj �������� ���� ������Ʈ
		* \param json ������ ���� �޾ƿ� jsonDoc
		*/
		void SetObjTransform(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto transfromVec = json->GetValueArray<float>("Transform", objID);

			Vector pos = { transfromVec[0], transfromVec[1], transfromVec[2] };
			Vector rot = { transfromVec[3], transfromVec[4], transfromVec[5] };
			Vector scale = { transfromVec[6], transfromVec[7], transfromVec[8] };

			gameObj->GetComponent<Transform>()->SetPosition(pos, Space::WORLD);
			gameObj->GetComponent<Transform>()->SetRotation(rot, Space::WORLD);
			gameObj->GetComponent<Transform>()->SetScale(scale);
		}

		/**
		* \brief json���� �Ľ��� �޽� ������ ���� ���ӿ�����Ʈ�� �������ݴϴ�.
		* \param gameObj �������� ���� ������Ʈ
		* \param json ������ ���� �޾ƿ� JsonDoc
		* \param objID ������Ʈ IDX => Json ���� ������Ʈ ����
		*/
		void SetObjMeshRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto meshRendererVec = json->GetValueArray<std::string>("MeshRenderer", objID);

			// 0���� �� �̸� 1���� �� Path ����.
			gameObj->AddComponent<MeshRenderer>()->Init(meshRendererVec[0], meshRendererVec[1], RendererBase::ModelType::NONE_AO);
		}

		/**
		 * \brief json���� �Ľ��� ��Ų ������ ���� ���ӿ�����Ʈ�� �������ݴϴ�.
		 * \param gameObj �������� ���� ������Ʈ
		 * \param json ������ ���� �޾ƿ� JsonDoc
		 * \param objID ������Ʈ IDX => Json ���� ������Ʈ ����
		 */
		void SetObjSkinnedRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto skinRendererVec = json->GetValueArray<std::string>("SkinnedRenderer", objID);

			// 0���� �� �̸� 1���� �� Path ����.
			gameObj->AddComponent<SkinnedMeshRenderer>()->Init(skinRendererVec[0], skinRendererVec[1], RendererBase::ModelType::ALL);
		}

		/**
		 * \brief json���� �Ľ��� �ִϸ��̼� ���� ���ӿ�����Ʈ�� �������ݴϴ�.
		 * \param gameObj �������� ���� ������Ʈ
		 * \param json ������ ���� �޾ƿ� JsonDoc
		 * \param objID ������Ʈ IDX => Json ���� ������Ʈ ����
		 */
		void SetObjAnimator(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID)
		{
			// �ִϸ����͸� ���� �߰����ݴϴ�..
			gameObj->AddComponent<Animator>();

			// ���� ������ �Ľ��մϴ�.
			auto animatorVec = json->GetValue2DArray<std::string>("Animation", objID);

			// for���� ���鼭 �ִϸ��̼��� �־��ݴϴ�.
			for (int i = 0; i < animatorVec.size(); i++)
			{
				// path�� ��.. �ִϸ��̼� �̸��� ��.. => json������ �ݴ�.
				gameObj->GetComponent<Animator>()->AddAnimation(animatorVec[i][1], animatorVec[i][0]);
			}

			// �׽�Ʈ ������ ��â�� �ִϸ��̼��� Set���ݴϴ�.
			// gameObj->GetComponent<Animator>()->SetAnimation(animatorVec[0]);
		}

		/**
		* \brief �������� �޾Ƽ� �� �ݶ��̴��� �������ݴϴ�
		* \param gameObj �������� ���� ������Ʈ
		* \param json ������ ���� �޾ƿ� jsonDoc
		* \param objID ������Ʈ IDX
		*/
		void SetObjSphereCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			// �⺻������ float �ϳ��� �޾ƿɴϴ�..
			float radius = json->GetValue<float>("SphereCollider", objID);

			gameObj->AddComponent<SphereCollider>()->SetRadius(radius);
		}

		/**
		* \brief ����� �޾Ƽ� �ڽ� �ݶ��̴��� �������ݴϴ�
		* \param gameObj �������� ���� ������Ʈ
		* \param json ������ ���� �޾ƿ� jsonDoc
		* \param objID ������Ʈ ID
		*/
		void SetObjBoxCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto boxVec = json->GetValueArray<float>("BoxCollider", objID);

			Vector size = { boxVec[0], boxVec[1], boxVec[2] };

			// �ݶ��̴� �߰� �� ������ ����
			gameObj->AddComponent<BoxCollider>()->SetSize(size);
		}

		/**
		 * \brief Ŭ���̾�Ʈ���� ���� �÷��̾� ������Ʈ�� ���ӿ�����Ʈ�� �޾��ݴϴ�.
		 * \param gameObj �÷��̾� ������Ʈ ���� �޸� ������Ʈ
		 * \param json �÷��̾� ������ ��� json
		 * \param objID ��ü ������Ʈ���� �÷��̾� ������ ��� ID
		 */
		void SetObjPlayer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID)
		{
			gameObj->AddComponent<Player::PlayerStateMachine>();
			gameObj->AddComponent<Player::PlayerController>();

			auto playerColVec = json->GetValueArray<float>("PlayerCollider", objID);

			Vector size = { playerColVec[0], playerColVec[1], playerColVec[2] };

			// �ݶ��̴� �߰� �� ������ ����
			gameObj->AddComponent<Player::PlayerCollider>()->SetSize(size);

			// ���� �ݶ��̴� ����
			gameObj->AddComponent<RayCollider>();
			// �κ��丮 ����.
			gameObj->AddComponent<InventoryUI>();

			// ��Ÿ �޽õ��� �̹� ������� ���� �Ŷ� ����.
			GameState::GetInstance()->SetPlayer(gameObj);
		}

		/**
		 * \brief ���� ������ �������ְ� �÷��̾��� �ڽ����� ������ִ� �Լ� => �⺻���� �̸�, �ݶ��̴� ���� �߰� �Ǿ��ٴ� ���� �Ͽ�..
		 * \param gameObj �������� �������� ���� ������Ʈ
		 * \param playerObj �ش� ������ child�� �������� �÷��̾� ���� ������Ʈ
		 */
		void SetObjPlayerWeapon(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj)
		{
			playerObj->SetChild(gameObj);

			gameObj->AddComponent<Weapon::DefaultWeapon>();
			gameObj->AddComponent<Weapon::WeaponMonoBehaviour>();
		}

		/**
		 * \brief ī�޶� �������ݴϴ�.
		 * \param scene ���� ī�޶�� �������� ��
		 * \param gameObj ī�޶�� �������� ���� ������Ʈ
		 * \param playerObj �÷��̾ ���󰡾� �ϱ� ������ �ش� Ÿ�� ������ �ʿ��� �÷��̾�
		 */
		void SetObjCamera(const std::shared_ptr<SceneBase>& scene, const std::shared_ptr<GameObject>& gameObj,
			const std::shared_ptr<GameObject>& playerObj)
		{
			// ī�޶� ������Ʈ�� �޾ƿͼ� ����.
			scene->SetMainCamera(gameObj->AddComponent<Camera>());
			gameObj->AddComponent<Player::PlayerCamera>();

			gameObj->GetComponent<Player::PlayerCamera>()->SetTarget(playerObj->GetComponent<Transform>());
		}

		// ��� ���� ���� ������Ʈ �Է� �ܿ��� ���� ���̽� Doc�� ���� �ʴ� �� �ϴ�..
		void SetObjAltar(const std::shared_ptr<GameObject>& gameObj)
		{
			//gameObj->AddComponent<Altar::AltarState>();
			gameObj->AddComponent<Altar::AltarMonoBehaviour>();
		}

		// Ŭ���̾�Ʈ ������Ʈ ���������� �����͸� ���� �ʿ䰡 ���� ��..! ���̽��� �޴´�.
		void SetObjObstacle(const std::shared_ptr<GameObject>& gameObj)
		{
			// ������ �������� ������ �̹� ä������ �´ٰ� ���� => �̸�, �±�, Ʈ������, ������ ���.
			auto box = gameObj->GetComponent<BoxCollider>();

			// ���� Ʈ���� ���� �ݶ��̴� ������ init
			gameObj->AddComponent<Obstacle::ObstacleState>()->Init(gameObj->GetComponent<Transform>()->GetWorldPosition(), box->GetSize().x, box->GetSize().z);

		}

		// ���� ������Ʈ �߰�.
		void SetObjMonster(const std::shared_ptr<GameObject>& gameObj)
		{
			gameObj->AddComponent<Monster::MonsterState>();
			gameObj->AddComponent<Monster::MonsterMonoBehaviour>();
			// ������ ����� �ݶ��̴��� �ƴϱ� ������ ���⼭ �߰����ݴϴ�. => json���� �߰�.
			// gameObj->AddComponent<SphereCollider>();
			gameObj->AddComponent<RayCollider>();
		}

		void SetObjMonsterAttack(const std::shared_ptr<GameObject>& gameObj,
			const std::shared_ptr<GameObject>& MonsterObj)
		{
			MonsterObj->SetChild(gameObj);

			gameObj->AddComponent<Monster::MonsterAttack>();
		}

		void SetObjSceneController(const std::shared_ptr<GameObject>& gameObj)
		{
			gameObj->AddComponent<SceneController>();
		}

		void SetObjBoss(const std::shared_ptr<GameObject>& gameObj)
		{
			gameObj->AddComponent<Boss::BossTree>();
			gameObj->AddComponent<Boss::BossMonoBehaviour>();
			gameObj->AddComponent<Boss::TestBossController>();

			GameState::GetInstance()->SetBoss(gameObj);
		}

		void SetObjBossAttack(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& bossObj)
		{
			bossObj->SetChild(gameObj);

			gameObj->AddComponent<Boss::BossAttackMonoBehaviour>();
		}
	}
}
