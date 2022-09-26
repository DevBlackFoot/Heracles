#pragma once
namespace Heracles
{
	class MapA : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-30.f, 0.f, -35.f);
		Vector SpawnB = Vector(-35.f, 0.f, -30.f);

		Vector ExitA = Vector(40.f, 0.f, 30.f);
		Vector ExitB = Vector(30.f, 0.f, 40.f);
		int rewardIdx;

	public:
		MapA(tstring sceneName);

		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};
}

