#pragma once
#include "Monobehaviour.h"

namespace Heracles
{
	/// <summary>
	/// 게임 실행시 전체 입구 종류만큼 생성해서 GameState에 vector로 저장해둔다
	/// 씬을 로드할때 다음맵2개를 정한다
	/// 정한 맵에 맞는 name을 세팅한다
	/// 따라서 씬에는 entrance오브젝트가 2개만 있으면 된다
	/// </summary>
	class Entrance : public MonoBehaviour
	{
	private:
		tstring nextSceneName;
		bool entranceType; //0:A , 1:B
		bool animationPlayed = false;
		bool triggerd = false;

		float timer = 1.2f;
		bool startTimer = false;

	public:
		Entrance(std::weak_ptr<GameObject> obj);
		virtual void Start() override;

		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;

		virtual void OnTriggerEnter(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerStay(std::shared_ptr<ColliderBase> other) override;
		virtual void OnTriggerExit(std::shared_ptr<ColliderBase> other) override;

		inline void SetNextScene(tstring sceneName) { this->nextSceneName = sceneName; }
		inline void SetEntranceType(bool type) { this->entranceType = type; }

		bool CompareSceneName(tstring name);
	};
}

