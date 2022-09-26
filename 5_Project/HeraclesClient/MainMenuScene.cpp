#include "pch.h"
#include "MainMenuScene.h"

#include "MainMenu.h"
#include "SettingUI.h"

#include "GameState.h"
#include "UIManager.h"

namespace Heracles
{
	MainMenuScene::MainMenuScene(tstring sceneName)
		: SceneBase(sceneName)
	{

	}

	MainMenuScene::~MainMenuScene()
	{

	}

	void MainMenuScene::BuildScene()
	{
		GameState::GetInstance()->currMap = SceneType::MAIN_MENU;
		this->CreateUI();
	}

	std::shared_ptr<GameObject> MainMenuScene::CreateUI()
	{
		std::shared_ptr<GameObject> ui = CreateEmpty();
		ui->AddComponent<MainMenu>();
		ui->AddComponent<UI::SettingUI>()->SetBackButtonEnable(false);


		return ui;
	}
}