#pragma once

namespace Heracles
{
	// 안쓸 예정
	class MapG : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(60.f, 0.f, 0.f);
		Vector SpawnB = Vector(-40.f, 0.f, -40.f);

		Vector ExitA = Vector(60.f, 0.f, 20.f);
		Vector ExitB = Vector(40.f, 0.f, 40.f);

		int rewardIdx;

	public:
		MapG(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
	};

}

