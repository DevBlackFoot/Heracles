#pragma once


namespace Heracles
{
	class BeforeBossMap : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		
		Vector SpawnA = Vector(-35.f, 0.f, 0.f);
		Vector SpawnB = Vector(-30.f, 0.f, -15.f);

		Vector ExitA = Vector(40.f, 0.f, 0.f);
		Vector ExitB = Vector(40.f, 0.f, 0.f);

	public:
		BeforeBossMap(tstring sceneName);

		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};
}

