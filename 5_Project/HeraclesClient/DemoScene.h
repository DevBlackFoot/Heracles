#pragma once

namespace Heracles
{
	class DemoScene :public GameEngineSpace::SceneBase
	{
	public:
		DemoScene(tstring sceneName);

		virtual ~DemoScene();

	public:
		virtual void BuildScene() override;

	private:
	};


}

