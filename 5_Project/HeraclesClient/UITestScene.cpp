#include "pch.h"
#include "UITestScene.h"

#include "UIManager.h"
#include "GameEngineManager.h"
#include "DialogUI.h"

namespace Heracles
{

	UITestScene::UITestScene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	UITestScene::~UITestScene()
	{
	}

	void UITestScene::BuildScene()
	{
		auto camera = CreateEmpty();
		camera->GetComponent<Transform>()->SetPosition({ 0.f, 1.0f, -10.0f });
		SetMainCamera(camera->AddComponent<Camera>());
		camera->AddComponent<BasicCameraController>();

		/*auto monologue = CreateEmpty();
		monologue->AddComponent<UI::DialogUI>()->StartAltarDialog(2);*/
	}
}
