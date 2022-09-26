#pragma once


namespace Heracles
{
	class BossMap : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-75.f, 0.f, 0.f);
		Vector SpawnB = Vector(-75.f, 0.f, 0.f);

		Vector ExitA = Vector(0.f, 0.f, 0.f);
		Vector ExitB = Vector(0.f, 0.f, 0.f);

	public:
		BossMap(tstring sceneName);

		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateMap();
		void CreateUI();
		void CreatePointLights();
	};
}

