#pragma once
#include "JsonDoc.h"

/// 제이슨 파싱을 위한 각종 함수들을 정리해둔 utils 함수 목록.
/// 기본적으로 클라이언트 컴포넌트를 자동으로 만들어서 붙여주는 것에 목적이 있다.


namespace Heracles
{
	namespace JsonObject
	{
		// 기본적으로 각종 제이슨 파일을 가져와서 실제로 맵을 만드는 함수
			// TODO 이 함수는 최종적으로 제이슨 파일과 데이터 테이블이 완성되면 만들 예정.
			// 일단은 각종 세팅 함수 부터 먼저 만듭시다. => CreateEmpty는 여기서 / 나머지 세세한 세팅은 아래의 함수들에서.
		void BuildJsonMap(const std::shared_ptr<SceneBase>& scene, const tstring& mapJsonPath, const tstring& objInfoJsonPath, const tstring& eventInfoJsonPath = TEXT(""));

		// 기본적인 이름, 태그 등을 변경해 주는 함수.
		void SetObjBasicInfo(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 오브젝트 트랜스폼 설정
		void SetObjTransform(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 오브젝트 매시 렌더러 설정
		void SetObjMeshRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 오브젝트 스킨 렌더러 설정 => 애니메이션이 존재하면 반드시 스킨 렌더러로 설정.
		void SetObjSkinnedRenderer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 애니메이터 설정
		void SetObjAnimator(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 구 콜라이더 설정
		void SetObjSphereCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 박스 콜라이더 설정.
		void SetObjBoxCollider(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);

		// 플레이어 오브젝트 세팅
		void SetObjPlayer(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<JsonDoc>& json, int objID);
		// 플레이어 자식 -> 웨폰 세팅 => 어떻게든 위에서 플레이어를 찾아서 넣어주자..
		void SetObjPlayerWeapon(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj);

		// 카메라 정보 -> 어떻게든 위에서 플레이어를 찾아서 넣어주자..
		void SetObjCamera(const std::shared_ptr<SceneBase>& scene, const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& playerObj);

		// 각종 오브젝트 세팅 - 제단
		void SetObjAltar(const std::shared_ptr<GameObject>& gameObj);
		// 장애물 + 벽 세팅 -> 해당 컴포넌트는 Init이 존재.
		void SetObjObstacle(const std::shared_ptr<GameObject>& gameObj);

		// 몬스터 세팅
		void SetObjMonster(const std::shared_ptr<GameObject>& gameObj);
		// 몬스터 공격 범위 세팅
		void SetObjMonsterAttack(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& MonsterObj);

		// Scene controller 세팅
		void SetObjSceneController(const std::shared_ptr<GameObject>& gameObj);

		// 보스 세팅
		void SetObjBoss(const std::shared_ptr<GameObject>& gameObj);
		void SetObjBossAttack(const std::shared_ptr<GameObject>& gameObj, const std::shared_ptr<GameObject>& bossObj);
	}
}
