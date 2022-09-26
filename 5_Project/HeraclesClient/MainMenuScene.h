#pragma once

namespace Heracles
{
	class MainMenuScene : public GameEngineSpace::SceneBase
	{
	private:

	public:
		MainMenuScene(tstring sceneName);

		virtual ~MainMenuScene();

		virtual void BuildScene() override;

	private:
		std::shared_ptr<GameObject> CreateUI();

	};

}

