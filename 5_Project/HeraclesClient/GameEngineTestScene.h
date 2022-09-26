#pragma once
#include "../GameEngine/SceneBase.h"

namespace Heracles
{
	class GameEngineTestScene : public GameEngineSpace::SceneBase
	{
	public:
		GameEngineTestScene(tstring sceneName);
		virtual ~GameEngineTestScene();

	public:
		virtual void BuildScene() override;
	};

}
