#include "pch.h"
#include "JsonObjectTemplate.h"

// 맵 빌더에 필요한 각종 컴포넌트들.
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "RayCollider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "JsonDoc.h"

// 클라이언트 컴포넌트
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
			// Json 파일을 파싱해줍시다.
			std::shared_ptr<JsonDoc> mapJson = std::make_shared<JsonDoc>();

			std::string strJson;
			strJson.assign(mapJsonPath.begin(), mapJsonPath.end());

			if (mapJson->LoadJsonFromFile(strJson) == false)
			{
				OutputDebugString(TEXT("Json Load Fail"));
				return;
			}

			// 오브젝트 json 파싱
			std::shared_ptr<JsonDoc> objInfoJson = std::make_shared<JsonDoc>();

			strJson.assign(objInfoJsonPath.begin(), objInfoJsonPath.end());
			if (objInfoJson->LoadJsonFromFile(strJson) == false)
			{
				OutputDebugString(TEXT("Json Load Fail"));
				return;
			}

			// 자식이 붙는 컴포넌트들을 캐싱하기 위해 변수를 선언해줍니다.
			std::shared_ptr<GameObject> player = nullptr;
			std::shared_ptr<GameObject> monster = nullptr;
			std::shared_ptr<GameObject> boss = nullptr;

			// map json 전체를 돌아줍니다.
			int mapObjCount = mapJson->GetDocument().Size();

			for (int i = 0; i < mapObjCount; i++)
			{
				// 오브젝트를 생성합니다.
				std::shared_ptr<GameObject> tmpObj = scene->CreateEmpty();

				// 앞으로 사용할 id를 받아옵니다.
				int mapObjID = mapJson->GetValue<int>("ID", i, false);

				// 기본적인 이름과 태그 + 트랜스폼을 받아옵니다.
				SetObjBasicInfo(tmpObj, mapJson, mapObjID);
				SetObjTransform(tmpObj, mapJson, mapObjID);

				// Collider의 이름을 가져옵니다.
				// 콜라이더의 이름에 따라, 콜라이더를 설정해줍니다.
				std::string collider = mapJson->GetValue<std::string>("Collider", mapObjID);

				if (collider == "BOX")
				{
					SetObjBoxCollider(tmpObj, mapJson, mapObjID);
				}
				else if (collider == "SPHERE")
				{
					SetObjSphereCollider(tmpObj, mapJson, mapObjID);
				}

				// 이제 오브젝트 ID에 따라서 Obj Json을 참고해서 오브젝트를 세팅해줍니다.
				int objID = mapJson->GetValue<int>("ObjectID", mapObjID);

				// 정보가를 나눠줍니다..
					// => 해당 값이 10이면, 이벤트가 없는 obj 입니다.
					// -> 해당 값이 20이면 클라이언트 컴포넌트를 붙여줘야합니다..
				int objValIdx = objID / 1000;

				// 렌더 가능 여부를 판단합니다
				bool renderable = objInfoJson->GetValue<bool>("Renderable", objID);

				// 렌더 가능하지 않으면 렌더러를 세팅하지 않습니다.
				if (renderable == true)
				{
					bool hasAnim = objInfoJson->GetValue<bool>("HasAnim", objID);
					// 애니메이션이 없으면 -> Mesh 렌더러
					if (hasAnim != true)
					{
						// Map JSon이 아닌 것에 주의!
						SetObjMeshRenderer(tmpObj, objInfoJson, objID);
					}
					// 있으면 -> Skinned 렌더러 및 애니메이터 추가
					else
					{
						SetObjSkinnedRenderer(tmpObj, objInfoJson, objID);
						SetObjAnimator(tmpObj, objInfoJson, objID);
					}
				}

				// 위에서 모든 렌더러를 세팅해주었기에.. 여기서는 오브젝트 설정만 변경해줍니다.
				if (objValIdx == 20)
				{
					// 나머지로 판별해줍니다..
					objValIdx = objID % 1000;

					switch (objValIdx)
					{
						case 101:
						{
							// 플레이어 -> 현재 오브젝트를 플레이어로 설정해줍니다.
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

						// 몬스터와 보스 모두 위와 같은 방식으로 해줍니다.
						case 301:
						{
							// 몬스터 -> 현재 오브젝트를 몬스터로 설정해줍니다.
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
							// 보스로 설정.
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
		 * \brief 이름과 태그 정보를 가져와서 게임 오브젝트에 넣어주는 함수.
		 * \param gameObj 세팅해줄 게임 오브젝트
		 * \param json 해당 오브젝트의 정보가 담긴 json 파일 => depth 차이.
		 * \param objID 모든 오브젝트가 있는 json 파일 내의 id.
		 */
		void SetObjBasicInfo(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			// 이름 추가
			std::string objStr = json->GetValue<std::string>("GameObjectName", objID);
			tstring objTStr;
			objTStr.assign(objStr.begin(), objStr.end());

			gameObj->SetName(objTStr);

			// 태그 추가
			objStr = json->GetValue<std::string>("GameObjectTag", objID);
			objTStr.assign(objStr.begin(), objStr.end());

			gameObj->SetTag(objTStr);
		}

		/**
		* \brief Json에서 값을 파싱해서 게임오브젝트에 넣어줍니다.
		* \param gameObj 세팅해줄 게임 오브젝트
		* \param json 세팅할 값을 받아올 jsonDoc
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
		* \brief json에서 파싱한 메시 렌더러 값을 게임오브젝트에 설정해줍니다.
		* \param gameObj 세팅해줄 게임 오브젝트
		* \param json 세팅할 값을 받아올 JsonDoc
		* \param objID 오브젝트 IDX => Json 내의 오브젝트 순서
		*/
		void SetObjMeshRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto meshRendererVec = json->GetValueArray<std::string>("MeshRenderer", objID);

			// 0번에 모델 이름 1번에 모델 Path 순서.
			gameObj->AddComponent<MeshRenderer>()->Init(meshRendererVec[0], meshRendererVec[1], RendererBase::ModelType::NONE_AO);
		}

		/**
		 * \brief json에서 파싱한 스킨 렌더러 값을 게임오브젝트에 설정해줍니다.
		 * \param gameObj 세팅해줄 게임 오브젝트
		 * \param json 세팅할 값을 받아올 JsonDoc
		 * \param objID 오브젝트 IDX => Json 내의 오브젝트 순서
		 */
		void SetObjSkinnedRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto skinRendererVec = json->GetValueArray<std::string>("SkinnedRenderer", objID);

			// 0번에 모델 이름 1번에 모델 Path 순서.
			gameObj->AddComponent<SkinnedMeshRenderer>()->Init(skinRendererVec[0], skinRendererVec[1], RendererBase::ModelType::ALL);
		}

		/**
		 * \brief json에서 파싱한 애니메이션 값을 게임오브젝트에 설정해줍니다.
		 * \param gameObj 세팅해줄 게임 오브젝트
		 * \param json 세팅할 값을 받아올 JsonDoc
		 * \param objID 오브젝트 IDX => Json 내의 오브젝트 순서
		 */
		void SetObjAnimator(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID)
		{
			// 애니메이터를 역시 추가해줍니다..
			gameObj->AddComponent<Animator>();

			// 관련 내용을 파싱합니다.
			auto animatorVec = json->GetValue2DArray<std::string>("Animation", objID);

			// for문을 돌면서 애니메이션을 넣어줍니다.
			for (int i = 0; i < animatorVec.size(); i++)
			{
				// path가 앞.. 애니메이션 이름이 뒤.. => json에서는 반대.
				gameObj->GetComponent<Animator>()->AddAnimation(animatorVec[i][1], animatorVec[i][0]);
			}

			// 테스트 용으로 초창기 애니메이션을 Set해줍니다.
			// gameObj->GetComponent<Animator>()->SetAnimation(animatorVec[0]);
		}

		/**
		* \brief 반지름을 받아서 구 콜라이더를 세팅해줍니다
		* \param gameObj 세팅해줄 게임 오브젝트
		* \param json 세팅할 값을 받아올 jsonDoc
		* \param objID 오브젝트 IDX
		*/
		void SetObjSphereCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			// 기본적으로 float 하나만 받아옵니다..
			float radius = json->GetValue<float>("SphereCollider", objID);

			gameObj->AddComponent<SphereCollider>()->SetRadius(radius);
		}

		/**
		* \brief 사이즈를 받아서 박스 콜라이더를 세팅해줍니다
		* \param gameObj 세팅해줄 게임 오브젝트
		* \param json 세팅할 값을 받아올 jsonDoc
		* \param objID 오브젝트 ID
		*/
		void SetObjBoxCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json,
			int objID)
		{
			auto boxVec = json->GetValueArray<float>("BoxCollider", objID);

			Vector size = { boxVec[0], boxVec[1], boxVec[2] };

			// 콜라이더 추가 후 사이즈 변경
			gameObj->AddComponent<BoxCollider>()->SetSize(size);
		}

		/**
		 * \brief 클라이언트에서 만든 플레이어 컴포넌트를 게임오브젝트에 달아줍니다.
		 * \param gameObj 플레이어 컴포넌트 들이 달릴 오브젝트
		 * \param json 플레이어 정보가 담긴 json
		 * \param objID 전체 오브젝트에서 플레이어 정보가 담긴 ID
		 */
		void SetObjPlayer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID)
		{
			gameObj->AddComponent<Player::PlayerStateMachine>();
			gameObj->AddComponent<Player::PlayerController>();

			auto playerColVec = json->GetValueArray<float>("PlayerCollider", objID);

			Vector size = { playerColVec[0], playerColVec[1], playerColVec[2] };

			// 콜라이더 추가 후 사이즈 변경
			gameObj->AddComponent<Player::PlayerCollider>()->SetSize(size);

			// 레이 콜라이더 생성
			gameObj->AddComponent<RayCollider>();
			// 인벤토리 생성.
			gameObj->AddComponent<InventoryUI>();

			// 기타 메시들은 이미 만들어져 있을 거라 가정.
			GameState::GetInstance()->SetPlayer(gameObj);
		}

		/**
		 * \brief 무기 정보를 세팅해주고 플레이어의 자식으로 만들어주는 함수 => 기본적인 이름, 콜라이더 등이 추가 되었다는 가정 하에..
		 * \param gameObj 웨폰으로 설정해줄 게임 오브젝트
		 * \param playerObj 해당 웨폰을 child로 지정해줄 플레이어 게임 오브젝트
		 */
		void SetObjPlayerWeapon(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj)
		{
			playerObj->SetChild(gameObj);

			gameObj->AddComponent<Weapon::DefaultWeapon>();
			gameObj->AddComponent<Weapon::WeaponMonoBehaviour>();
		}

		/**
		 * \brief 카메라를 설정해줍니다.
		 * \param scene 메인 카메라로 설정해줄 씬
		 * \param gameObj 카메라로 설정해줄 게임 오브젝트
		 * \param playerObj 플레이어를 따라가야 하기 때문에 해당 타겟 설정에 필요한 플레이어
		 */
		void SetObjCamera(const std::shared_ptr<SceneBase>& scene, const std::shared_ptr<GameObject>& gameObj,
			const std::shared_ptr<GameObject>& playerObj)
		{
			// 카메라 컴포넌트를 받아와서 세팅.
			scene->SetMainCamera(gameObj->AddComponent<Camera>());
			gameObj->AddComponent<Player::PlayerCamera>();

			gameObj->GetComponent<Player::PlayerCamera>()->SetTarget(playerObj->GetComponent<Transform>());
		}

		// 사실 이쯤 오면 컴포넌트 입력 외에는 따로 제이슨 Doc를 받지 않는 듯 하다..
		void SetObjAltar(const std::shared_ptr<GameObject>& gameObj)
		{
			//gameObj->AddComponent<Altar::AltarState>();
			gameObj->AddComponent<Altar::AltarMonoBehaviour>();
		}

		// 클라이언트 컴포넌트 내부적으로 데이터를 받을 필요가 있을 때..! 제이슨을 받는다.
		void SetObjObstacle(const std::shared_ptr<GameObject>& gameObj)
		{
			// 나머지 정보들을 위에서 이미 채워져서 온다고 가정 => 이름, 태그, 트랜스폼, 렌더러 등등.
			auto box = gameObj->GetComponent<BoxCollider>();

			// 기존 트랜스 폼과 콜라이더 정보로 init
			gameObj->AddComponent<Obstacle::ObstacleState>()->Init(gameObj->GetComponent<Transform>()->GetWorldPosition(), box->GetSize().x, box->GetSize().z);

		}

		// 몬스터 컴포넌트 추가.
		void SetObjMonster(const std::shared_ptr<GameObject>& gameObj)
		{
			gameObj->AddComponent<Monster::MonsterState>();
			gameObj->AddComponent<Monster::MonsterMonoBehaviour>();
			// 고정된 스페어 콜라이더가 아니기 때문에 여기서 추가해줍니다. => json에서 추가.
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
