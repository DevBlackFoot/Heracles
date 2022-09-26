#include "pch.h"
#include "StartCutScene.h"

#include "UIManager.h"
#include "DialogUI.h"
#include "SoundManager.h"

namespace Heracles
{
	StartCutScene::StartCutScene(tstring sceneName)
		: SceneBase(sceneName)
	{
	}

	StartCutScene::~StartCutScene()
	{
	}

	void StartCutScene::BuildScene()
	{
		auto monologue = CreateEmpty();
		monologue->AddComponent<UI::DialogUI>()->SetNowState(UI::DialogState::FIRST_CUT_SCENE);

		SoundManager::GetInstance()->PlayBGM("CutScene");
	}
}
