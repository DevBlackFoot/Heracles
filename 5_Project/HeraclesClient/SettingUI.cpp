#include "pch.h"
#include "SettingUI.h"

#include "UIManager.h"
#include "GraphicsManager.h"
#include "DialogUI.h"
#include "TutorialUI.h"

namespace Heracles
{
	namespace UI
	{
		GraphicsEngineSpace::Canvas* SettingUI::canvas = nullptr;

		SettingUI::SettingUI(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj)
			, bgmVolumeBar(nullptr)
			, effectVolumeBar(nullptr)
			, backButton(nullptr)
			, closeButton(nullptr)
		{
			// Setting UI의 Canvas 생성
			if (canvas != nullptr)
			{
				bgmVolumeBar = canvas->GetProgressBar("BGMVolumeBar");
				effectVolumeBar = canvas->GetProgressBar("EffectVolumeBar");
				backButton = canvas->GetButtonUI("BackToMainButton");
				closeButton = canvas->GetButtonUI("SettingCloseButtonBack");

				// Bar default
				float barWidth = 504.0f;
				float frontBarWidth = 450.0f;

				auto bgmEventLambda = [=]()
				{
					Vector mousePos = InputManager::GetInstance()->GetMousePos();
					const Rect& barRect = canvas->GetButtonUI(bgmVolumeBar->GetName() + "BarButton")->GetUIRect();

					float posX = mousePos.x - (barRect.left + barRect.right) / 2.0f;

					if (posX < -frontBarWidth / 2.0f)
						posX = -frontBarWidth / 2.0f;
					else if (posX > frontBarWidth / 2.0f)
						posX = frontBarWidth / 2.0f;

					canvas->GetTextureUI(bgmVolumeBar->GetName() + "Cursor")->SetPosition({ posX, 0.0f, -0.01f });
					bgmVolumeBar->SetPercent(posX / barWidth + 0.5f);
					SoundManager::GetInstance()->SetSoundVoulume(posX / frontBarWidth + 0.5f, PlayMode::BGM);
				};

				canvas->GetButtonUI(bgmVolumeBar->GetName() + "BarButton")->SetClickEvent(bgmEventLambda);
				canvas->GetButtonUI(bgmVolumeBar->GetName() + "BarButton")->SetPressEvent(bgmEventLambda);

				auto effectEventLambda = [=]()
				{
					Vector mousePos = InputManager::GetInstance()->GetMousePos();
					const Rect& barRect = canvas->GetButtonUI(effectVolumeBar->GetName() + "BarButton")->GetUIRect();

					float posX = mousePos.x - (barRect.left + barRect.right) / 2.0f;

					if (posX < -frontBarWidth / 2.0f)
						posX = -frontBarWidth / 2.0f;
					else if (posX > frontBarWidth / 2.0f)
						posX = frontBarWidth / 2.0f;

					canvas->GetTextureUI(effectVolumeBar->GetName() + "Cursor")->SetPosition({ posX, 0.0f, -0.01f });
					effectVolumeBar->SetPercent(posX / barWidth + 0.5f);
					SoundManager::GetInstance()->SetSoundVoulume(posX / frontBarWidth + 0.5f, PlayMode::Effect);
				};

				canvas->GetButtonUI(effectVolumeBar->GetName() + "BarButton")->SetClickEvent(effectEventLambda);
				canvas->GetButtonUI(effectVolumeBar->GetName() + "BarButton")->SetPressEvent(effectEventLambda);

				return;
			}

			canvas = UIManager::GetInstance()->CreateCanvas("SettingUI_Canvas");

			// 팩토리 & 리소스 매니저 포인터 참조
			GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
			GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

			// Board
			GraphicsEngineSpace::TextureBase* boardTexture = resourceManager->GetTexture("BoardTexture");

			if (boardTexture == nullptr)
				boardTexture = factory->CreateTexture("BoardTexture", "Resources/UI/settings_BG.png");

			CreateBoard(boardTexture);

			// Volume Bar
			GraphicsEngineSpace::TextureBase* volumeIcon = resourceManager->GetTexture("VolumeIcon");

			if (volumeIcon == nullptr)
				volumeIcon = factory->CreateTexture("VolumeIcon", "Resources/UI/volume_icon.png");

			GraphicsEngineSpace::TextureBase* frontBar = resourceManager->GetTexture("VolumeFront");

			if (frontBar == nullptr)
				frontBar = factory->CreateTexture("VolumeFront", "Resources/UI/soundbar.png");

			GraphicsEngineSpace::TextureBase* backBar = resourceManager->GetTexture("VolumeBack");

			if (backBar == nullptr)
				backBar = factory->CreateTexture("VolumeBack", "Resources/UI/soundbar_BG-07.png");

			GraphicsEngineSpace::TextureBase* cursor = resourceManager->GetTexture("Cursor");

			if (cursor == nullptr)
				cursor = factory->CreateTexture("Cursor", "Resources/UI/cursor-08.png");

			bgmVolumeBar = canvas->CreateProgressBar("BGMVolumeBar");
			bgmVolumeBar->SetPosition({ 0.0f, -150.0f, 0.15f });
			CreateVolumeBar(bgmVolumeBar, volumeIcon, frontBar, backBar, cursor, PlayMode::BGM);

			effectVolumeBar = canvas->CreateProgressBar("EffectVolumeBar");
			effectVolumeBar->SetPosition({ 0.0f, -50.0f, 0.15f });
			CreateVolumeBar(effectVolumeBar, volumeIcon, frontBar, backBar, cursor, PlayMode::Effect);

			// Back To Main
			GraphicsEngineSpace::TextureBase* hoverTexture = resourceManager->GetTexture("AltarHover");

			if (hoverTexture == nullptr)
				hoverTexture = factory->CreateTexture("AltarHover", "Resources/UI/Altar_OnMouseFrame.png");

			CreateBackToMainButton(hoverTexture);

			// Close
			GraphicsEngineSpace::TextureBase* buttonBackTexture = resourceManager->GetTexture("ButtonBG");

			if (buttonBackTexture == nullptr)
				buttonBackTexture = factory->CreateTexture("ButtonBG", "Resources/UI/Button1_BG.png");
			
			GraphicsEngineSpace::TextureBase* buttonFrontTexture = resourceManager->GetTexture("ButtonX");

			if (buttonFrontTexture == nullptr)
				buttonFrontTexture = factory->CreateTexture("ButtonX", "Resources/UI/Button1_X.png");

			CreateCloseButton(buttonBackTexture, buttonFrontTexture);
		}

		SettingUI::~SettingUI()
		{
			canvas->SetEnable(false);
		}

		void SettingUI::SetBackButtonEnable(bool isEnable)
		{
			backButton->SetEnable(isEnable);
			canvas->GetTextUI("BackText")->SetEnable(isEnable);
		}

		void SettingUI::Awake()
		{
			canvas->SetEnable(false);
		}

		void SettingUI::Start()
		{

		}

		void SettingUI::PreUpdate(float tick)
		{

		}

		void SettingUI::Update(float tick)
		{
			if (InputManager::GetInstance()->GetInputState(VK_ESCAPE, KeyState::DOWN) == true)
			{
				if (UIManager::GetInstance()->GetCanvas("MainMenuUI_Canvas")->GetEnable() != true)
					canvas->SetEnable(!canvas->GetEnable());
			}
		}

		void SettingUI::LateUpdate(float tick)
		{

		}

		void SettingUI::CreateBoard(TextureBase* boardTexture)
		{
			GraphicsEngineSpace::TextureUI* board = canvas->CreateTextureUI("EquipmentSlotUI");
			board->SetPosition({ 0.0f, 0.0f, 0.2f });
			board->SetWidth(956.0f);
			board->SetHeight(904.0f);
			board->SetTexture(boardTexture);
		}

		void SettingUI::CreateVolumeBar(ProgressBar* bar, TextureBase* icon, TextureBase* frontBar, TextureBase* backBar, TextureBase* cursor, PlayMode mode)
		{
			// Bar default
			float barWidth = 504.0f;
			float barHeight = 61.0f;

			float frontBarWidth = 450.0f;

			bar->SetFrontTexture(frontBar);
			bar->SetFrontBarTexCoord(0.0f, 0.0f, barWidth, barHeight);
			bar->SetBackTexture(backBar);
			bar->SetBackBarTexCoord(0.0f, 0.0f, barWidth, barHeight);
			bar->SetWidth(barWidth);
			bar->SetHeight(barHeight);

			// Icon
			GraphicsEngineSpace::TextureUI* barIcon = canvas->CreateTextureUI(bar->GetName() + "Icon");
			barIcon->SetParent(bar);
			barIcon->SetPosition({ 0.0f, -25.0f, -0.01f });
			barIcon->SetTexture(icon);
			barIcon->SetWidth(66.0f);
			barIcon->SetHeight(75.0f);

			// Cursor
			GraphicsEngineSpace::TextureUI* cursorUI = canvas->CreateTextureUI(bar->GetName() + "Cursor");
			cursorUI->SetParent(bar);
			cursorUI->SetTexture(cursor);
			cursorUI->SetPosition({ frontBarWidth / 2.0f, 0.0f, -0.01f });
			cursorUI->SetWidth(66.0f);
			cursorUI->SetHeight(75.0f);

			// Scrolling
			GraphicsEngineSpace::ButtonUI* barButton = canvas->CreateButtonUI(bar->GetName() + "BarButton");
			barButton->SetParent(bar);
			barButton->SetPosition({ 0.0f, 0.0f, -0.01f });
			barButton->SetWidth(frontBarWidth);
			barButton->SetHeight(barHeight);

			auto eventLambda = [=]()
			{
				Vector mousePos = InputManager::GetInstance()->GetMousePos();
				const Rect& barRect = barButton->GetUIRect();

				float posX = mousePos.x - (barRect.left + barRect.right) / 2.0f;

				if (posX < -frontBarWidth / 2.0f)
					posX = -frontBarWidth / 2.0f;
				else if (posX > frontBarWidth / 2.0f)
					posX = frontBarWidth / 2.0f;

				cursorUI->SetPosition({ posX, 0.0f, -0.01f });
				bar->SetPercent(posX / barWidth + 0.5f);
				SoundManager::GetInstance()->SetSoundVoulume(posX / frontBarWidth + 0.5f, mode);
			};

			barButton->SetClickEvent(eventLambda);
			barButton->SetPressEvent(eventLambda);
		}

		void SettingUI::CreateBackToMainButton(TextureBase* hoverTexture)
		{
			backButton = canvas->CreateButtonUI("BackToMainButton");

			GraphicsEngineSpace::TextUI* backText = canvas->CreateTextUI("BackText");
			backText->SetText(L"메인 메뉴로");
			backText->SetPosition({ 0.0f, 150.0f, 0.15f });
			backText->SetFont("H2MJRE");
			backText->SetFontSize(32.0f);

			backButton->SetParent(backText);
			backButton->SetHoverTexture(hoverTexture);
			backButton->SetClickEvent([]()
				{
					SceneManager::GetInstance()->LoadScene(TEXT("MainMenu"));
					canvas->GetButtonUI("BackToMainButton")->SetEnable(false);
					DialogUI::isCheckedControl = false;
					TutorialUI::isCheckWeapon = false;
					TutorialUI::isCheckAltar = false;
				});
		}

		void SettingUI::CreateCloseButton(TextureBase* backGround, TextureBase* closeTexture)
		{
			GraphicsEngineSpace::TextureUI* closeButtonBack = canvas->CreateTextureUI("SettingCloseButtonBack");
			closeButtonBack->SetPosition({ 0.0f, 330.0f, 0.15f });
			closeButtonBack->SetWidth(75.0f);
			closeButtonBack->SetHeight(75.0f);
			closeButtonBack->SetTexture(backGround);

			closeButton = canvas->CreateButtonUI("SettingCloseButton");
			closeButton->SetParent(closeButtonBack);
			closeButton->SetWidth(75.0f);
			closeButton->SetHeight(75.0f);
			closeButton->SetDefaultTexture(closeTexture);
			closeButton->SetClickEvent([]()
				{
					GraphicsEngineSpace::Canvas* mainMenuCanvas = UIManager::GetInstance()->GetCanvas("MainMenuUI_Canvas");
					canvas->SetEnable(false);

					mainMenuCanvas->GetButtonUI("MenuSettingTexture")->SetEnable(true);
					mainMenuCanvas->GetButtonUI("MenuControlsButton")->SetEnable(true);
					mainMenuCanvas->GetButtonUI("MenuCreditsButton")->SetEnable(true);
					mainMenuCanvas->GetButtonUI("MenuExitButton")->SetEnable(true);
				});
		}
	}
}