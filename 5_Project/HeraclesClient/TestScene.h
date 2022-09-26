#pragma once

namespace Heracles
{
	class TestScene : public GameEngineSpace::SceneBase
	{
	public:
		TestScene(tstring sceneName);

		virtual ~TestScene();

	public:
		void BuildScene() override;
	};
}

