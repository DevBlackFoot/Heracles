#pragma once
#include "../GameEngine/SceneBase.h"

namespace Heracles
{
	// UI 화면 배치를 위해 확인용으로 만든 클래스
	class UITestScene : public GameEngineSpace::SceneBase
	{
	public:
		UITestScene(tstring sceneName);
		virtual ~UITestScene();

	public:
		virtual void BuildScene() override;
	};

}
