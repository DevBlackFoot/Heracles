#pragma once

namespace Heracles
{
	class MapB : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-30.f, 0.f, -35.f);
		Vector SpawnB = Vector(-35.f, 0.f, -30.f);

		Vector ExitA = Vector(20.f, 0.f, 27.5f);
		Vector ExitB = Vector(10.f, 0.f, 40.f);
		int rewardIdx;
	public:
		MapB(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};
}

