#pragma once
#include "../GameEngine/SceneBase.h"

namespace Heracles
{
	// UI ȭ�� ��ġ�� ���� Ȯ�ο����� ���� Ŭ����
	class UITestScene : public GameEngineSpace::SceneBase
	{
	public:
		UITestScene(tstring sceneName);
		virtual ~UITestScene();

	public:
		virtual void BuildScene() override;
	};

}
