#pragma once

namespace Heracles
{
	class MapH : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-55.f, 0.f, -25.f);
		Vector SpawnB = Vector(-45.f, 0.f, -35.f);

		Vector ExitA = Vector(60.f, 0.f, 30.f);
		Vector ExitB = Vector(50.f, 0.f, 40.f);

		int rewardIdx;

	public:
		MapH(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
	};

}

