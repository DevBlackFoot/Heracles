#pragma once

namespace Heracles
{
	//안쓸예정
	class MapD : public GameEngineSpace::SceneBase
	{
	private:
		std::shared_ptr<GameObject> sceneController;

		Vector SpawnA = Vector(-50.f, 0.f, -30.f);
		Vector SpawnB = Vector(-30.f, 0.f, -50.f);

		Vector ExitA = Vector(50.f, 0.f, 30.f);
		Vector ExitB = Vector(30.f, 0.f, 50.f);

		int rewardIdx;

	public:
		MapD(tstring sceneName);
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SetMonsterWave();
		void CreateObjects();
		void CreateUI();
	};
}


