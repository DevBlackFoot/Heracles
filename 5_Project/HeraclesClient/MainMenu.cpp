#include "pch.h"
#include "MainMenu.h"

#include "UIManager.h"
#include "GraphicsManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	GraphicsEngineSpace::Canvas* MainMenu::canvas = nullptr;
	bool isLoadCompleted = false;

	MainMenu::MainMenu(std::weak_ptr<GameObject> obj)
		: ComponentBase(obj)
		, startButton(nullptr)
		, settingButton(nullptr)
		, controlsButton(nullptr)
		, creditButton(nullptr)
		, exitButton(nullptr)
		, fileLoader(nullptr)
	{
		if (canvas != nullptr)
		{
			startButton = canvas->GetButtonUI("MenuStartButton");
			settingButton = canvas->GetButtonUI("MenuSettingTexture");
			controlsButton = canvas->GetButtonUI("MenuControlsButton");
			creditButton = canvas->GetButtonUI("MenuCreditsButton");
			exitButton = canvas->GetButtonUI("MenuExitButton");

			isLoadCompleted = true;

			return;
		}

		// Main Menu UI의 Canvas 생성
		canvas = UIManager::GetInstance()->CreateCanvas("MainMenuUI_Canvas");

		// 팩토리 & 리소스 매니저 포인터 참조
		GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		// Back Ground
		GraphicsEngineSpace::TextureBase* bgTexture = resourceManager->GetTexture("MainBackGround");

		if (bgTexture == nullptr)
			bgTexture = factory->CreateTexture("MainBackGround", "Resources/UI/TITLE_BG.png");

		GraphicsEngineSpace::TextureBase* titleTexture = resourceManager->GetTexture("MainTitle");

		if (titleTexture == nullptr)
			titleTexture = factory->CreateTexture("MainTitle", "Resources/UI/title.png");

		CreateBackGround(bgTexture, titleTexture);

		// Menu Button
		GraphicsEngineSpace::TextureBase* buttonHoverTexture = resourceManager->GetTexture("MenuButtonHover");

		if (buttonHoverTexture == nullptr)
			buttonHoverTexture = factory->CreateTexture("MenuButtonHover2", "Resources/UI/OnMouse_Title2.png");

		CreateMenuButton(buttonHoverTexture);

		// ASE Model Parsing & Texture Loading
		LoadFiles(factory, resourceManager);
	}

	MainMenu::~MainMenu()
	{
		if (fileLoader != nullptr)
		{
			fileLoader->join();
			delete fileLoader;
		}
	}

	void MainMenu::Awake()
	{
		////Debug
		//{
		//	UIManager::GetInstance()->CreateUIText("Loading", "Loading ...", Vector(1.f, 0.5f, 1.f), Vector(850.f, 20.f, 0.f), 0.f, Vector(2.f, 2.f, 2.f));
		//	UIManager::GetInstance()->SetTextUIIsEnable("Loading", false);
		//}
		//
		////버튼
		//{
		//	UIManager::GetInstance()->CreateUIButton("StartButton", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", 1000, 500, 200, 200, 0.f);
		//	UIManager::GetInstance()->CreateUIButton("Setting", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", 1000, 500, 200, 200, 0.f);
		//	UIManager::GetInstance()->CreateUIButton("Guide", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", 1000, 500, 200, 200, 0.f);
		//	UIManager::GetInstance()->CreateUIButton("Credit", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", 1000, 500, 200, 200, 0.f);
		//	UIManager::GetInstance()->CreateUIButton("ExitGame", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", "StartButton", "Resources/UI/startButton.PNG", 1000, 500, 200, 200, 0.f);
		//}

		////배경
		//UIManager::GetInstance()->CreateUISprite("Main", "Main", "Resources/UI/Main.PNG",0.f,0.f,1920.f,1080.f,1.f);

		canvas->SetEnable(true);
	}

	void MainMenu::Start()
	{
		SoundManager::GetInstance()->PlayBGM("MainMenu");

		auto ending = UIManager::GetInstance()->GetCanvas("EndingCanvas");
		ending->SetEnable(false);

		//
		//UIManager::GetInstance()->SetButtonIsEnable("StartButton", true);
		//UIManager::GetInstance()->SetButtonIsEnable("Setting", true);
		//UIManager::GetInstance()->SetButtonIsEnable("Guide", true);
		//UIManager::GetInstance()->SetButtonIsEnable("Credit", true);
		//UIManager::GetInstance()->SetButtonIsEnable("ExitGame", true);
	}

	void MainMenu::Update(float tick)
	{
		//if (UIManager::GetInstance()->GetButtonState("StartButton") == ButtonState::UP)
		//{
		//	UIManager::GetInstance()->SetTextUIIsEnable("Loading", true);
		//	SceneManager::GetInstance()->LoadScene(TEXT("DemoScene"));
		//}

		if (UIManager::GetInstance()->GetCanvas("SettingUI_Canvas")->GetEnable() == true)
			startButton->SetEnable(false);
		else if (isLoadCompleted == true)
			startButton->SetEnable(true);
	}

	void MainMenu::LateUpdate(float tick)
	{

	}

	void MainMenu::CreateBackGround(TextureBase* backGround, TextureBase* title)
	{
		// 배경
		GraphicsEngineSpace::TextureUI* bg = canvas->CreateTextureUI("MainBackGround");
		bg->SetAnchor({ GraphicsEngineSpace::HorizontalLocation::STRETCH, GraphicsEngineSpace::VerticalLocation::STRETCH });
		bg->SetPivot({ GraphicsEngineSpace::HorizontalLocation::LEFT, GraphicsEngineSpace::VerticalLocation::TOP });
		bg->SetPosition({ 0.0f, 0.0f, 0.3f });
		bg->SetTexture(backGround);

		// 타이틀
		GraphicsEngineSpace::TextureUI* titleUI = canvas->CreateTextureUI("MainTitle");
		titleUI->SetPosition({ 0.0f, -250.0f, 0.2f });
		titleUI->SetWidth(1135.0f);
		titleUI->SetHeight(228.0f);
		titleUI->SetTexture(title);
	}

	void MainMenu::CreateMenuButton(TextureBase* hoverTexture)
	{
		// 버튼 위치와 간격
		float yPos = 30.0f;
		float yOffset = 80.0f;

		UIManager::GetInstance()->CreateUISprite("MenuBackGroundBlur", "MenuBGBlur", "Resources/UI/blur_cube-02.png",
			0.0f, yOffset * 3, 536, 559, 0.25f, canvas);

		GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
		GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

		GraphicsEngineSpace::TextureBase* menuTex = resourceManager->GetTexture("MenuStartTex");
		if (menuTex == nullptr)
			menuTex = factory->CreateTexture("MenuStartTex", "Resources/UI/game_start_button.png");

		// Start Button Text
		GraphicsEngineSpace::TextureUI* startTextureUI = canvas->CreateTextureUI("MenuStartTexture");
		startTextureUI->SetPosition({ 0.0f, yPos, 0.2f });
		startTextureUI->SetTexture(menuTex);
		startTextureUI->SetHeight(59.f);
		startTextureUI->SetWidth(314.f);

		// Start Button Event
		startButton = canvas->CreateButtonUI("MenuStartButton");
		startButton->SetParent(startTextureUI);
		startButton->SetEnable(false);
		startButton->SetPosition({ -5.0f, 3.0f, -0.01f });
		startButton->SetWidth(526.0f);
		startButton->SetHeight(yOffset);
		startButton->SetHoverTexture(hoverTexture);
		startButton->SetEnterEvent([]()
		{
			SoundManager::GetInstance()->PlayEffect("TitleUIHover");
		});
		startButton->SetClickEvent([]()
			{
				if (isLoadCompleted == true)
				{
					SoundManager::GetInstance()->PlayEffect("TitleUISelect1");

					SceneManager::GetInstance()->LoadScene(TEXT("StartCutScene"));

					canvas->SetEnable(false);
				}
			});

		// Option Button Text
		menuTex = resourceManager->GetTexture("MenuSettingTex");
		if (menuTex == nullptr)
			menuTex = factory->CreateTexture("MenuSettingTex", "Resources/UI/settings_button.png");

		GraphicsEngineSpace::TextureUI* settingTextureUI = canvas->CreateTextureUI("MenuSettingTexture");
		settingTextureUI->SetPosition({ 0.0f, yPos + yOffset, 0.2f });
		settingTextureUI->SetTexture(menuTex);
		settingTextureUI->SetHeight(58.f);
		settingTextureUI->SetWidth(313.f);

		// Option Button Event
		settingButton = canvas->CreateButtonUI("MenuSettingTexture");
		settingButton->SetParent(settingTextureUI);
		settingButton->SetPosition({ 0.0f, 3.0f, -0.01f });
		settingButton->SetWidth(526.0f);
		settingButton->SetHeight(yOffset);
		settingButton->SetHoverTexture(hoverTexture);
		settingButton->SetEnterEvent([]()
		{
			SoundManager::GetInstance()->PlayEffect("TitleUIHover");
		});
		settingButton->SetClickEvent([&]()
			{
				UIManager::GetInstance()->GetCanvas("SettingUI_Canvas")->SetEnable(true);

				SoundManager::GetInstance()->PlayEffect("TitleUISelect1");

				canvas->GetButtonUI("MenuSettingTexture")->SetEnable(false);
				canvas->GetButtonUI("MenuControlsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuCreditsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuExitButton")->SetEnable(false);
			});

		// Controls Button Text
		menuTex = resourceManager->GetTexture("MenuControlTex");
		if (menuTex == nullptr)
			menuTex = factory->CreateTexture("MenuControlTex", "Resources/UI/controls_button.png");

		GraphicsEngineSpace::TextureUI* controlsTextureUI = canvas->CreateTextureUI("MenuControlsTexture");
		controlsTextureUI->SetPosition({ 0.0f, yPos + yOffset * 2, 0.2f });
		controlsTextureUI->SetTexture(menuTex);
		controlsTextureUI->SetHeight(58.f);
		controlsTextureUI->SetWidth(313.f);

		// 컨트롤 자체의 텍스쳐 UI 생성
		UIManager::GetInstance()->CreateUIButton("Controls", "Controls", "Resources/UI/controls_tab.png",
			0, 0, 956, 904, 0.1f, canvas);

		ButtonUI* control = canvas->GetButtonUI("Controls");
		control->SetEnable(false);

		control->SetClickEvent([&]()
			{
				canvas->GetButtonUI("Controls")->SetEnable(false);

				canvas->GetButtonUI("MenuSettingTexture")->SetEnable(true);
				canvas->GetButtonUI("MenuControlsButton")->SetEnable(true);
				canvas->GetButtonUI("MenuCreditsButton")->SetEnable(true);
				canvas->GetButtonUI("MenuExitButton")->SetEnable(true);
			});

		// Controls Button Event
		controlsButton = canvas->CreateButtonUI("MenuControlsButton");
		controlsButton->SetParent(controlsTextureUI);
		controlsButton->SetPosition({ 0.0f, 3.0f, -0.01f });
		controlsButton->SetWidth(526.0f);
		controlsButton->SetHeight(yOffset);
		controlsButton->SetHoverTexture(hoverTexture);
		controlsButton->SetEnterEvent([]()
		{
			SoundManager::GetInstance()->PlayEffect("TitleUIHover");
		});
		controlsButton->SetClickEvent([&]()
			{
				canvas->GetButtonUI("Controls")->SetEnable(true);

				SoundManager::GetInstance()->PlayEffect("TitleUISelect1");

				canvas->GetButtonUI("MenuSettingTexture")->SetEnable(false);
				canvas->GetButtonUI("MenuControlsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuCreditsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuExitButton")->SetEnable(false);
			});

		// Credits Button Text
		menuTex = resourceManager->GetTexture("MenuCreditsTex");
		if (menuTex == nullptr)
			menuTex = factory->CreateTexture("MenuCreditsTex", "Resources/UI/credits_button.png");

		GraphicsEngineSpace::TextureUI* creditsTextureUI = canvas->CreateTextureUI("MenuCreditsTexture");
		creditsTextureUI->SetPosition({ 0.0f, yPos + yOffset * 3, 0.2f });
		creditsTextureUI->SetTexture(menuTex);
		creditsTextureUI->SetHeight(58.f);
		creditsTextureUI->SetWidth(313.f);

		// 크리딧 자체의 텍스쳐 UI 생성.
		UIManager::GetInstance()->CreateUIButton("Credits", "Credits", "Resources/UI/credits_tab.png",
			0, 0, 956, 904, 0.1f, canvas);

		ButtonUI* credit = canvas->GetButtonUI("Credits");
		credit->SetEnable(false);

		credit->SetClickEvent([&]()
			{
				canvas->GetButtonUI("Credits")->SetEnable(false);

				canvas->GetButtonUI("MenuSettingTexture")->SetEnable(true);
				canvas->GetButtonUI("MenuControlsButton")->SetEnable(true);
				canvas->GetButtonUI("MenuCreditsButton")->SetEnable(true);
				canvas->GetButtonUI("MenuExitButton")->SetEnable(true);
			});

		// Credits Button Event
		creditButton = canvas->CreateButtonUI("MenuCreditsButton");
		creditButton->SetParent(creditsTextureUI);
		creditButton->SetPosition({ 0.0f, 3.0f, -0.01f });
		creditButton->SetWidth(526.0f);
		creditButton->SetHeight(yOffset);
		creditButton->SetHoverTexture(hoverTexture);
		creditButton->SetEnterEvent([]()
		{
			SoundManager::GetInstance()->PlayEffect("TitleUIHover");
		});
		creditButton->SetClickEvent([&]()
			{
				canvas->GetButtonUI("Credits")->SetEnable(true);

				SoundManager::GetInstance()->PlayEffect("TitleUISelect1");

				canvas->GetButtonUI("MenuSettingTexture")->SetEnable(false);
				canvas->GetButtonUI("MenuControlsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuCreditsButton")->SetEnable(false);
				canvas->GetButtonUI("MenuExitButton")->SetEnable(false);
			});

		// Exit Button Text
		menuTex = resourceManager->GetTexture("MenuExitTex");
		if (menuTex == nullptr)
			menuTex = factory->CreateTexture("MenuExitTex", "Resources/UI/quit_button.png");

		GraphicsEngineSpace::TextureUI* exitTextureUI = canvas->CreateTextureUI("MenuExitTexture");
		exitTextureUI->SetPosition({ 0.0f, yPos + yOffset * 4, 0.2f });
		exitTextureUI->SetTexture(menuTex);
		exitTextureUI->SetHeight(58.f);
		exitTextureUI->SetWidth(313.f);

		// Exit Button Event
		exitButton = canvas->CreateButtonUI("MenuExitButton");
		exitButton->SetParent(exitTextureUI);
		exitButton->SetPosition({ 0.0f, 3.0f, -0.01f });
		exitButton->SetWidth(526.0f);
		exitButton->SetHeight(yOffset);
		exitButton->SetHoverTexture(hoverTexture);
		exitButton->SetEnterEvent([]()
		{
			SoundManager::GetInstance()->PlayEffect("TitleUIHover");
		});
		exitButton->SetClickEvent([]()
			{
				PostQuitMessage(0);
			});
	}

	void MainMenu::LoadFiles(Factory* factory, ResourceManager* resourceManager)
	{
		SetCharacterList();
		SetObjectList();
		SetMapList();
		SetTextureList();
		SetWeaponList();

		fileLoader = new std::thread([&, factory, resourceManager]()
			{
				// Loading Progress Bar
				int resourcesSize = models.size() + animations.size() + textures.size();
				int doneCount = 0;

				// Sprite Animation UI
				GraphicsEngineSpace::TextureBase* loadingTexture = factory->CreateTexture("LoadingTexture", "Resources/UI/Loading_Run.png");

				GraphicsEngineSpace::SpriteAnimation* spriteTexture = factory->CreateSpriteAnimation("LoadingRun");
				spriteTexture->SetTexture(loadingTexture, 3690, 90, 41, 1, 0.05f);
				spriteTexture->SetWidth(90.0f);
				spriteTexture->SetHeight(90.0f);
				spriteTexture->SetLoop(true);

				GraphicsEngineSpace::SpriteAnimationUI* loadingSpriteUI = canvas->CreateSpriteAnimationUI("LoadingSprite");
				loadingSpriteUI->SetAnchor({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::BOTTOM });
				loadingSpriteUI->SetPivot({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::BOTTOM });
				loadingSpriteUI->SetPosition({ -100.0f, -100.0f, 0.15f });
				loadingSpriteUI->SetWidth(90.0f);
				loadingSpriteUI->SetHeight(90.0f);
				loadingSpriteUI->SetSpriteTexture(spriteTexture);

				// Loading Text
				GraphicsEngineSpace::TextUI* loadingText = canvas->CreateTextUI("LoadingTextUI");
				loadingText->SetAnchor({ GraphicsEngineSpace::HorizontalLocation::CENTER, GraphicsEngineSpace::VerticalLocation::BOTTOM });
				loadingText->SetPivot({ GraphicsEngineSpace::HorizontalLocation::CENTER, GraphicsEngineSpace::VerticalLocation::TOP });
				loadingText->SetParent(loadingSpriteUI);
				loadingText->SetFont("EBS-Light");
				loadingText->SetColor({ 115.0f / 255.0f, 63.0f / 255.0f, 49.0f / 255.0f });

				// Load Texture
				for (auto& iter : textures)
				{
					loadingText->SetText(std::to_string((doneCount * 100) / resourcesSize) + "%");

					if (resourceManager->GetTexture(iter.name) == nullptr)
						factory->CreateTexture(iter.name, iter.path);

					doneCount++;
				}

				// Load Model
				for (auto& iter : models)
				{
					loadingText->SetText(std::to_string((doneCount * 100) / resourcesSize) + "%");

					if (resourceManager->GetModel(iter.name) == nullptr)
						factory->CreateModelFromASEFile(iter.name, iter.path);

					if (iter.metallic.length() != 0)
					{
						TextureBase* metallicMap = resourceManager->GetTexture(iter.metallic);

						if (metallicMap != nullptr)
							resourceManager->GetModel(iter.name)->SetMetallicMap(metallicMap);
					}

					if (iter.roughness.length() != 0)
					{
						TextureBase* roughtnessMap = resourceManager->GetTexture(iter.roughness);

						if (roughtnessMap != nullptr)
							resourceManager->GetModel(iter.name)->SetRoughnessMap(roughtnessMap);
					}

					if (iter.ambientocclusion.length() != 0)
					{
						TextureBase* aoMap = resourceManager->GetTexture(iter.ambientocclusion);

						if (aoMap != nullptr)
							resourceManager->GetModel(iter.name)->SetAmbientOcclusionMap(aoMap);
					}

					doneCount++;
				}

				// Load Animation
				for (auto& iter : animations)
				{
					loadingText->SetText(std::to_string((doneCount * 100) / resourcesSize) + "%");

					if (resourceManager->GetModel(iter.file.name) == nullptr)
						factory->CreateModelFromASEFile(iter.file.name, iter.file.path);

					factory->CreateAnimationFromASEFile(iter.file.name, iter.file.path, iter.animationName);

					doneCount++;
				}

				// Done!
				loadingText->SetEnable(false);
				loadingSpriteUI->SetEnable(false);

				// Set Enable Start Button
				isLoadCompleted = true;
				//canvas->GetTextUI("MenuStartText")->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				startButton->SetEnable(true);
			});
	}

	void MainMenu::SetCharacterList()
	{
		// Heracles
		models.push_back({ "Heracles", "Resources/Model/Heracles_attack3.ase", "HeraclesMetallic", "HeraclesRoughness", "HeraclesAO" });

		animations.push_back({ { "Heracles", "Resources/Model/Heracles_idle.ase"}, "Idle", 1.0f, true });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_run.ase"}, "Run", 1.0f, true });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_attack1.ase"}, "Attack1", 1.0f, false });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_attack2.ase"}, "Attack2", 1.0f, false });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_attack3.ase"}, "Attack3", 1.0f, false });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_hit.ase"}, "Hit", 1.0f, false });
		animations.push_back({ { "Heracles", "Resources/Model/Heracles_death.ase"}, "Death", 1.0f, false });

		// Boss
		models.push_back({ "Boss", "Resources/Model/Boss.ase", "", "BossRoughness", "BossAO" });

		animations.push_back({ { "Boss", "Resources/Model/Boss_idle.ase"}, "BossIdle", 1.0f, true });
		animations.push_back({ { "Boss", "Resources/Model/Boss_run.ase"}, "BossRun", 1.0f, true });
		animations.push_back({ { "Boss", "Resources/Model/Boss_death.ase"}, "BossDeath", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_stun.ase"}, "BossStun", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_count.ase"}, "BossCounterAttack", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_attack1.ase"}, "BossAttack1", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_attack2.ase"}, "BossAttack2", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_attack3.ase"}, "BossAttack3", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_jump1.ase"}, "BossCloseJump", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_jump2.ase"}, "BossLongJump", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_doljin_start.ase"}, "BossPrepareDash", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_doljin_run.ase"}, "BossDash", 1.0f, true });
		animations.push_back({ { "Boss", "Resources/Model/Boss_badak.ase"}, "BossShockWave", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_tanmak.ase"}, "BossBarrage", 1.0f, false });
		animations.push_back({ { "Boss", "Resources/Model/Boss_pohyo.ase"}, "BossRoar", 1.0f, false });

		// Monster
		models.push_back({ "Skeleton", "Resources/Model/Skeleton.ase", "", "", "" });

		animations.push_back({ { "Skeleton", "Resources/Model/Skeleton_idle.ase"}, "MonsterIdle", 1.0f, true });
		animations.push_back({ { "Skeleton", "Resources/Model/Skeleton_run.ase"}, "MonsterRun", 1.0f, true });
		animations.push_back({ { "Skeleton", "Resources/Model/Skeleton_attack.ase"}, "MonsterAttack", 1.0f, false });
		animations.push_back({ { "Skeleton", "Resources/Model/Skeleton_hit.ase"}, "MonsterHit", 1.0f, false });
		animations.push_back({ { "Skeleton", "Resources/Model/Skeleton_death.ase"}, "MonsterDeath", 1.0f, false });
	}

	void MainMenu::SetObjectList()
	{
		// Door
		models.push_back({ "Door0", "Resources/Model/Door_OpenAnimation0.ase", "DoorMetallic", "DoorRoughness" });
		animations.push_back({ {"Door0", "Resources/Model/Door_OpenAnimation0.ase"}, "DoorOpen", 1.0f, false });
		//animations.push_back({ {"Door0", "Resources/Model/Door_CloseAnimation.ase"}, "DoorClose", 1.0f, false });

		models.push_back({ "Door1", "Resources/Model/Door_OpenAnimation1.ase", "DoorMetallic", "DoorRoughness" });
		animations.push_back({ {"Door1", "Resources/Model/Door_OpenAnimation1.ase"}, "DoorOpen", 1.0f, false });
		//animations.push_back({ {"Door1", "Resources/Model/Door_CloseAnimation.ase"}, "DoorClose", 1.0f, false });

		models.push_back({ "Door2", "Resources/Model/Door_OpenAnimation2.ase", "DoorMetallic", "DoorRoughness" });
		animations.push_back({ {"Door2", "Resources/Model/Door_OpenAnimation2.ase"}, "DoorOpen", 1.0f, false });
		//animations.push_back({ {"Door2", "Resources/Model/Door_CloseAnimation.ase"}, "DoorClose", 1.0f, false });

		models.push_back({ "Door3", "Resources/Model/Door_OpenAnimation3.ase", "DoorMetallic", "DoorRoughness" });
		animations.push_back({ {"Door3", "Resources/Model/Door_OpenAnimation3.ase"}, "DoorOpen", 1.0f, false });
		//animations.push_back({ {"Door3", "Resources/Model/Door_CloseAnimation.ase"}, "DoorClose", 1.0f, false });

		// Boss Door
		models.push_back({ "BossDoor", "Resources/Model/BossDoor_OpenAnimation.ase", "BossDoorMetallic", "BossDoorRoughness" });
		animations.push_back({ {"BossDoor", "Resources/Model/BossDoor_OpenAnimation.ase"}, "DoorOpen", 1.0f, false });

		// Treasure Box
		models.push_back({ "HPReward", "Resources/Model/HPTreasureBox_OpenAnimation.ase",  "TreasureBoxMetallic",  "TreasureBoxRoughness" });
		animations.push_back({ {"HPReward", "Resources/Model/HPTreasureBox_OpenAnimation.ase"}, "HPRewardOpen", 1.0f, false });

		models.push_back({ "MemoryReward", "Resources/Model/MemoryTreasureBox_OpenAnimation.ase",  "TreasureBoxMetallic",  "TreasureBoxRoughness" });
		animations.push_back({ {"MemoryReward", "Resources/Model/MemoryTreasureBox_OpenAnimation.ase"}, "MemoryRewardOpen", 1.0f, false });

		models.push_back({ "WeaponReward", "Resources/Model/WeaponTreasureBox_OpenAnimation.ase",  "TreasureBoxMetallic",  "TreasureBoxRoughness" });
		animations.push_back({ {"WeaponReward", "Resources/Model/WeaponTreasureBox_OpenAnimation.ase"}, "WeaponRewardOpen", 1.0f, false });

		// Memory Gem
		models.push_back({ "MemoryGem", "Resources/Model/MemoryGem.ase", "MemoryGemMetallic", "MemoryGemRoughness" });

		// Weapon Coin
		models.push_back({ "WeaponCoin", "Resources/Model/Weapon_Coin_Box.ase", "WeaponCoinMetallic", "WeaponCoinRoughness" });
	}

	void MainMenu::SetMapList()
	{
		// Test Map
		models.push_back({ "MapFloor", "Resources/Model/Stage_40_80_2Hole_Floor.ase" });
		models.push_back({ "MapFloorFrame", "Resources/Model/Stage_40_80_2Hole_FloorFrame.ase" });
		models.push_back({ "MapWall", "Resources/Model/Stage_40_80_2Hole_Wall.ase" });

		// Boss Map
		models.push_back({ "BossMapFloor", "Resources/Model/BossStage_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "BossMapBrazier", "Resources/Model/BossStage_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "BossMapFloorFrame", "Resources/Model/BossStage_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "BossMapPillar", "Resources/Model/BossStage_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "BossMapPot", "Resources/Model/BossStage_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "BossMapTapestry", "Resources/Model/BossStage_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "BossMapWall", "Resources/Model/BossStage_Wall.ase", "BossWallMetallic", "BossWallRoughness" });
		models.push_back({ "BossMapMemoryGem", "Resources/Model/BossStage_MemoryGem.ase", "MemoryGemMetallic", "MemoryGemRoughness" });
		models.push_back({ "BossMapWeaponCoin", "Resources/Model/BossStage_WeaponCoin.ase", "WeaponCoinMetallic", "WeaponCoinRoughness" });

		// Before Boss Map
		models.push_back({ "BeforeBossMapFloor", "Resources/Model/Stage_40_80_BeforeBoss_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "BeforeBossMapBrazier", "Resources/Model/Stage_40_80_BeforeBoss_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "BeforeBossMapFloorFrame", "Resources/Model/Stage_40_80_BeforeBoss_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "BeforeBossMapPillar", "Resources/Model/Stage_40_80_BeforeBoss_HeraStatue.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "BeforeBossMapHeraStatue", "Resources/Model/Stage_40_80_BeforeBoss_HeraStatue.ase", "HeraStatueMetallic", "HeraStatueRoughness" });
		models.push_back({ "BeforeBossMapTapestry", "Resources/Model/Stage_40_80_BeforeBoss_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "BeforeBossMapWall", "Resources/Model/Stage_40_80_BeforeBoss_Wall.ase", "WallMetallic", "WallRoughness" });

		// Altar
		models.push_back({ "AltarMapFloor", "Resources/Model/Stage_40_80_Altar_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "AltarMapBrazier", "Resources/Model/Stage_40_80_Altar_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "AltarMapFloorFrame", "Resources/Model/Stage_40_80_Altar_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "AltarMapPot", "Resources/Model/Stage_40_80_Altar_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "AltarMapTapestry", "Resources/Model/Stage_40_80_Altar_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "AltarMapWall", "Resources/Model/Stage_40_80_Altar_Wall.ase", "WallMetallic", "WallRoughness" });
		models.push_back({ "AltarMapMemoryGem", "Resources/Model/Stage_40_80_Altar_MemoryGem.ase", "MemoryGemMetallic", "MemoryGemRoughness" });
		models.push_back({ "AltarMapAltar", "Resources/Model/Stage_40_80_Altar_Altar.ase", "AltarMetallic", "AltarRoughness" });

		// Start Scene
		models.push_back({ "StartMapFloor", "Resources/Model/Stage_40_80_2Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "StartMapBrazier", "Resources/Model/Stage_40_80_2Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "StartMapFloorFrame", "Resources/Model/Stage_40_80_2Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "StartMapPillar", "Resources/Model/Stage_40_80_2Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "StartMapPot", "Resources/Model/Stage_40_80_2Hole_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "StartMapTapestry", "Resources/Model/Stage_40_80_2Hole_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "StartMapWall", "Resources/Model/Stage_40_80_2Hole_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map A
		models.push_back({ "MapAFloor", "Resources/Model/Stage_80_80_1Wall_1Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapAFloorFrame", "Resources/Model/Stage_80_80_1Wall_1Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapABrazier", "Resources/Model/Stage_80_80_1Wall_1Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapAHeraStatue", "Resources/Model/Stage_80_80_1Wall_1Hole_HeraStatue.ase", "HeraStatueMetallic", "HeraStatueRoughness" });
		models.push_back({ "MapAPillar", "Resources/Model/Stage_80_80_1Wall_1Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapAPot", "Resources/Model/Stage_80_80_1Wall_1Hole_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "MapATapestry", "Resources/Model/Stage_80_80_1Wall_1Hole_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "MapAWall", "Resources/Model/Stage_80_80_1Wall_1Hole_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map B
		models.push_back({ "MapBFloor", "Resources/Model/Stage_80_80_2Wall_1Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapBFloorFrame", "Resources/Model/Stage_80_80_2Wall_1Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapBBrazier", "Resources/Model/Stage_80_80_2Wall_1Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapBHeraStatue", "Resources/Model/Stage_80_80_2Wall_1Hole_HeraStatue.ase", "HeraStatueMetallic", "HeraStatueRoughness" });
		models.push_back({ "MapBPillar", "Resources/Model/Stage_80_80_2Wall_1Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapBPot", "Resources/Model/Stage_80_80_2Wall_1Hole_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "MapBTapestry", "Resources/Model/Stage_80_80_2Wall_1Hole_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "MapBWall", "Resources/Model/Stage_80_80_2Wall_1Hole_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map C
		models.push_back({ "MapCFloor", "Resources/Model/Stage_80_80_4Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapCFloorFrame", "Resources/Model/Stage_80_80_4Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapCBrazier", "Resources/Model/Stage_80_80_4Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapCPillar", "Resources/Model/Stage_80_80_4Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapCWall", "Resources/Model/Stage_80_80_4Hole_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map E
		models.push_back({ "MapEFloor", "Resources/Model/Stage_96_96_3Wall_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapEFloorFrame", "Resources/Model/Stage_96_96_3Wall_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapEBrazier", "Resources/Model/Stage_96_96_3Wall_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapEPillar", "Resources/Model/Stage_96_96_3Wall_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapEPot", "Resources/Model/Stage_96_96_3Wall_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "MapETapestry", "Resources/Model/Stage_96_96_3Wall_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "MapEWall", "Resources/Model/Stage_96_96_3Wall_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map F
		models.push_back({ "MapFFloor", "Resources/Model/Stage_64_128_1Wall_2Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapFFloorFrame", "Resources/Model/Stage_64_128_1Wall_2Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapFBrazier", "Resources/Model/Stage_64_128_1Wall_2Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapFHeraStatue", "Resources/Model/Stage_64_128_1Wall_2Hole_HeraStatue.ase", "HeraStatueMetallic", "HeraStatueRoughness" });
		models.push_back({ "MapFPillar", "Resources/Model/Stage_64_128_1Wall_2Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapFTapestry", "Resources/Model/Stage_64_128_1Wall_2Hole_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "MapFWall", "Resources/Model/Stage_64_128_1Wall_2Hole_Wall.ase", "WallMetallic", "WallRoughness" });

		// Map I
		models.push_back({ "MapIFloor", "Resources/Model/Stage_80_128_2Wall_3Hole_Floor.ase", "FloorMetallic", "FloorRoughness" });
		models.push_back({ "MapIFloorFrame", "Resources/Model/Stage_80_128_2Wall_3Hole_FloorFrame.ase", "FloorFrameMetallic", "FloorFrameRoughness" });
		models.push_back({ "MapIBrazier", "Resources/Model/Stage_80_128_2Wall_3Hole_Brazier.ase", "BrazierMetallic", "BrazierRoughness" });
		models.push_back({ "MapIHeraStatue", "Resources/Model/Stage_80_128_2Wall_3Hole_HeraStatue.ase", "HeraStatueMetallic", "HeraStatueRoughness" });
		models.push_back({ "MapIPillar", "Resources/Model/Stage_80_128_2Wall_3Hole_Pillar.ase", "PillarMetallic", "PillarRoughness" });
		models.push_back({ "MapIPot", "Resources/Model/Stage_80_128_2Wall_3Hole_Pot.ase", "PotMetallic", "PotRoughness" });
		models.push_back({ "MapITapestry", "Resources/Model/Stage_80_128_2Wall_3Hole_Tapestry.ase", "TapestryMetallic", "TapestryRoughness" });
		models.push_back({ "MapIWall", "Resources/Model/Stage_80_128_2Wall_3Hole_Wall.ase", "WallMetallic", "WallRoughness" });
	}

	void MainMenu::SetTextureList()
	{
		// In Game UI Texture
		textures.push_back({ "WeaponSlot", "Resources/UI/Weapon_BG.png" });
		textures.push_back({ "MasterSword", "Resources/UI/MasterSword.png" });
		textures.push_back({ "Club", "Resources/UI/Club.png" });
		textures.push_back({ "SpikeClub", "Resources/UI/SpikeClub.png" });
		textures.push_back({ "AtenaSword", "Resources/UI/AtenaSword.png" });
		textures.push_back({ "SkeletonArm", "Resources/UI/SkeletonArm.png" });
		textures.push_back({ "HP_FrontBar", "Resources/UI/HP_Bar.png" });
		textures.push_back({ "HP_BackBar", "Resources/UI/HP_BackBar.png" });
		textures.push_back({ "CurseSlot", "Resources/UI/CurseIcon_BG.png" });
		textures.push_back({ "Blind", "Resources/UI/CurseIcon_Blind.png" });
		textures.push_back({ "Madness", "Resources/UI/CurseIcon_Madness.png" });
		textures.push_back({ "Pain", "Resources/UI/CurseIcon_Pain.png" });
		textures.push_back({ "MemoryIcon", "Resources/UI/MemoryGem_Icon.png" });

		// Heracles
		textures.push_back({ "HeraclesMetallic", "Resources/Texture/Heracles_Metallic.png" });
		textures.push_back({ "HeraclesRoughness", "Resources/Texture/Heracles_Roughness.png" });
		textures.push_back({ "HeraclesAO", "Resources/Texture/Heracles_Mixed_AO.png" });

		// Boss
		textures.push_back({ "BossRoughness", "Resources/Texture/boss_Roughness.png" });
		textures.push_back({ "BossAO", "Resources/Texture/boss_Mixed_AO.png" });

		// Altar
		textures.push_back({ "AltarMetallic", "Resources/Texture/Altar_Metallic.png" });
		textures.push_back({ "AltarRoughness", "Resources/Texture/Altar_Roughness.png" });

		// Boss Door
		textures.push_back({ "BossDoorMetallic", "Resources/Texture/BossDoor_Metallic.png" });
		textures.push_back({ "BossDoorRoughness", "Resources/Texture/BossDoor_Roughness.png" });

		// Boss Wall
		textures.push_back({ "BossWallMetallic", "Resources/Texture/BossWall_Metallic.png" });
		textures.push_back({ "BossWallRoughness", "Resources/Texture/BossWall_Roughness.png" });

		// Brazier
		textures.push_back({ "BrazierMetallic", "Resources/Texture/Brazier_Metallic.png" });
		textures.push_back({ "BrazierRoughness", "Resources/Texture/Brazier_Roughness.png" });

		// Door
		textures.push_back({ "DoorMetallic", "Resources/Texture/Door_Metallic.png" });
		textures.push_back({ "DoorRoughness", "Resources/Texture/Door_Roughness.png" });

		// Floor
		textures.push_back({ "FloorMetallic", "Resources/Texture/Floor_Metallic.png" });
		textures.push_back({ "FloorRoughness", "Resources/Texture/Floor_Roughness.png" });

		// Floor Frame
		textures.push_back({ "FloorFrameMetallic", "Resources/Texture/FloorFrame_Metallic.png" });
		textures.push_back({ "FloorFrameRoughness", "Resources/Texture/FloorFrame_Roughness.png" });

		// Hera Statue
		textures.push_back({ "HeraStatueMetallic", "Resources/Texture/HeraStatue_Metallic.png" });
		textures.push_back({ "HeraStatueRoughness", "Resources/Texture/HeraStatue_Roughness.png" });

		// Memory Gem
		textures.push_back({ "MemoryGemMetallic", "Resources/Texture/MemoryGem_Metallic.png" });
		textures.push_back({ "MemoryGemRoughness", "Resources/Texture/MemoryGem_Roughness.png" });

		// Pillar
		textures.push_back({ "PillarMetallic", "Resources/Texture/Pillar_Metallic.png" });
		textures.push_back({ "PillarRoughness", "Resources/Texture/Pillar_Roughness.png" });

		// Pot
		textures.push_back({ "PotMetallic", "Resources/Texture/Pot_Metallic.png" });
		textures.push_back({ "PotRoughness", "Resources/Texture/Pot_Roughness.png" });

		// Tapestry
		textures.push_back({ "TapestryMetallic", "Resources/Texture/Tapestry_Metallic.png" });
		textures.push_back({ "TapestryRoughness", "Resources/Texture/Tapestry_Roughness.png" });

		// ThornyObstacle
		textures.push_back({ "ThornyObstacleMetallic", "Resources/Texture/ThornyObstacle_Metallic.png" });
		textures.push_back({ "ThornyObstacleRoughness", "Resources/Texture/ThornyObstacle_Roughness.png" });

		// Treasure Box
		textures.push_back({ "TreasureBoxMetallic", "Resources/Texture/TreasureBox_Metallic.png" });
		textures.push_back({ "TreasureBoxRoughness", "Resources/Texture/TreasureBox_Roughness.png" });

		// Wall
		textures.push_back({ "WallMetallic", "Resources/Texture/Wall_Metallic.png" });
		textures.push_back({ "WallRoughness", "Resources/Texture/Wall_Roughness.png" });

		// Weapon Coin
		textures.push_back({ "WeaponCoinMetallic", "Resources/Texture/Weapon_Coin_Metallic.png" });
		textures.push_back({ "WeaponCoinRoughness", "Resources/Texture/Weapon_Coin_Roughness.png" });

		// Wine
		textures.push_back({ "WineMetallic", "Resources/Texture/Wine_Metallic.png" });
		textures.push_back({ "WineRoughness", "Resources/Texture/Wine_Roughness.png" });
	}

	void MainMenu::SetWeaponList()
	{
		// Club
		models.push_back({ "WeaponClub", "Resources/Model/Heracles_attack1_weapon.ase" });

		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_attack1_weapon.ase"}, "Attack1", 1.0f, false });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_attack2_weapon.ase"}, "Attack2", 1.0f, false });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_attack3_weapon.ase"}, "Attack3", 1.0f, false });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_death_weapon.ase"}, "Death", 1.0f, false });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_hit_weapon.ase"}, "Hit", 1.0f, false });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_idle_weapon.ase"}, "Idle", 1.0f, true });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_run_weapon.ase"}, "Run", 1.0f, true });
		animations.push_back({ {"WeaponClub", "Resources/Model/Heracles_skill_weapon.ase"}, "Skill", 1.0f, false });
	}
}