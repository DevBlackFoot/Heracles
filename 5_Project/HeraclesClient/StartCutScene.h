#pragma once
#include "../GameEngine/SceneBase.h"

namespace Heracles
{

	class StartCutScene : public GameEngineSpace::SceneBase
	{
	public:
		StartCutScene(tstring sceneName);
		virtual ~StartCutScene();

	public:
		virtual void BuildScene() override;
	};

}
