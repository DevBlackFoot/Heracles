#pragma once
namespace Heracles
{
	enum MonsterType
	{
		DEFAULT = 1,
		THROW = 2,
		JUMP = 3,
	};

	class MonsterSpawner
	{
	private:
		std::vector<std::shared_ptr<GameObject>> monsterList;

	public:
		void SetMonsterToList(std::shared_ptr<GameObject> monster);
		void SpawnMonster();
	};

	class SceneController : public ComponentBase
	{
	private:
		std::queue<std::shared_ptr<MonsterSpawner>> spawner;
		std::shared_ptr<GameObject> rewardBox = nullptr;
	public:
		SceneController(std::weak_ptr<GameObject> obj);

		virtual void Start() override;

		virtual void PreUpdate(float tick) override;
		virtual void Update(float tick) override;
		virtual void LateUpdate(float tick) override;

		inline void SetRewardBox(std::shared_ptr<GameObject> obj) { this->rewardBox = obj; }
		
		void CreateMonsterSpawner(SceneBase* scene, std::vector<std::pair<int,Vector>>& v);
		void SelectNextMap();
	private:
		void SetNextWave();

	};
}

