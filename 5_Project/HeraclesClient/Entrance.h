#pragma once
#include "Monobehaviour.h"

namespace Heracles
{
	/// <summary>
	/// ���� ����� ��ü �Ա� ������ŭ �����ؼ� GameState�� vector�� �����صд�
	/// ���� �ε��Ҷ� ������2���� ���Ѵ�
	/// ���� �ʿ� �´� name�� �����Ѵ�
	/// ���� ������ entrance������Ʈ�� 2���� ������ �ȴ�
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

