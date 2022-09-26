#pragma once

namespace Heracles
{
	// 안쓸 예정
	class MapI : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-50.f, 0.f, 0.f);
		Vector SpawnB = Vector(0.f, 0.f, -35.f);

		Vector ExitA = Vector(50.f, 0.f, 0.f);
		Vector ExitB = Vector(40.f, 0.f, 20.f);

		int rewardIdx;

	public:
		MapI(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
		void CreatePointLights();
	};

}

