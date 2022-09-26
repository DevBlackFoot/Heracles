#pragma once


namespace Heracles
{
	class MapE : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(30.f, 0.f, -30.f);
		Vector SpawnB = Vector(-10.f, 0.f, -10.f);

		Vector ExitA = Vector(0.f, 0.f,30.f);
		Vector ExitB = Vector(-30.f, 0.f, 50.f);

		int rewardIdx;

	public:
		MapE(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};

}

