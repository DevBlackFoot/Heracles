#pragma once

namespace Heracles
{
	class MapF : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-55.f, 0.f, -15.f);
		Vector SpawnB = Vector(-50.f, 0.f, -25.f);

		Vector ExitA = Vector(60.f, 0.f, 0.f);
		Vector ExitB = Vector(50.f, 0.f, 10.f);

		int rewardIdx;

	public:
		MapF(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};

}

