#pragma once

namespace Heracles
{

	class EndingScene : public GameEngineSpace::SceneBase
	{
	public:
		EndingScene(tstring sceneName);
		virtual ~EndingScene();

	public:
		virtual void BuildScene() override;
	};

}
