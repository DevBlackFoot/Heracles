#pragma once

namespace Heracles
{
	class FirstScene :public GameEngineSpace::SceneBase
	{
	public:
		FirstScene(tstring sceneName);

	public:
		virtual void BuildScene() override;

	private:
		void CreateWallCollider();
		void SpawnMonster();
		void CreateObjects();
	};
}

