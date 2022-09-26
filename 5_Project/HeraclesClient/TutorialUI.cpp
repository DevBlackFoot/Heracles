#include "pch.h"
#include "TutorialUI.h"

#include "DialogUI.h"
#include "GameState.h"
#include "UIManager.h"
#include "SceneManager.h"
#include "SoundManager.h"

namespace Heracles
{
	namespace UI
	{
		bool TutorialUI::isCheckWeapon = false;
		bool TutorialUI::isCheckAltar = false;

		TutorialUI::TutorialUI(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj, ComponentType::CANVAS)
			, tutorial(nullptr)
			, nowState(TutorialState::NONE)
			, isDisplay(false)
		{
		}

		TutorialUI::~TutorialUI()
		{
		}

		void TutorialUI::Awake()
		{
			tutorial = UIManager::GetInstance()->GetCanvas("TutorialCanvas");
			tutorial->SetEnable(true);

			if (SceneManager::GetInstance()->GetCurrentScene()->GetSceneName() == TEXT("StartCurseMap")
				&& isCheckWeapon == false)
			{
				nowState = TutorialState::WEAPON_TUTORIAL1;
				isDisplay = true;

			}
		}

		void TutorialUI::PreUpdate(float tick)
		{
			if (nowState == TutorialState::WEAPON_TUTORIAL1 && isDisplay == true)
			{
				tutorial->GetTextureUI("WeaponTutorial1")->SetEnable(true);
				dialog->SetIsEnable(false);

				if (isCheckWeapon == false)
				{
					SoundManager::GetInstance()->PlayEffect("TutorialUI");
					GameState::GetInstance()->interacting = true;

					isCheckWeapon = true;
				}
			}

			if (nowState == TutorialState::WEAPON_TUTORIAL2 && isDisplay == true)
			{
				tutorial->GetTextureUI("WeaponTutorial2")->SetEnable(true);
				dialog->SetIsEnable(false);
			}

			if (nowState == TutorialState::ALTAR_TUTORIAL && isDisplay == true)
			{
				tutorial->GetTextureUI("AltarTutorial")->SetEnable(true);
				dialog->SetIsEnable(false);

				if (isCheckAltar == false)
				{
					SoundManager::GetInstance()->PlayEffect("TutorialUI");
					GameState::GetInstance()->interacting = true;

					isCheckAltar = true;
				}
			}
		}

		void TutorialUI::Update(float tick)
		{
			if (nowState == TutorialState::WEAPON_TUTORIAL1
				&& (InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
				&& isDisplay == true)
			{
				isDisplay = true;
				nowState = TutorialState::WEAPON_TUTORIAL2;
				SoundManager::GetInstance()->PlayEffect("TutorialPage");

			}
			else if (nowState == TutorialState::WEAPON_TUTORIAL2
				&& (InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
				&& isDisplay == true)
			{
				tutorial->GetTextureUI("WeaponTutorial1")->SetEnable(false);
				isDisplay = false;
				nowState = TutorialState::NONE;
				SoundManager::GetInstance()->PlayEffect("TutorialPage");
				GameState::GetInstance()->interacting = false;

			}
			else if (nowState == TutorialState::ALTAR_TUTORIAL
				&& (InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
				&& isDisplay == true)
			{
				isDisplay = false;
				nowState = TutorialState::NONE;
				SoundManager::GetInstance()->PlayEffect("TutorialPage");
				GameState::GetInstance()->interacting = false;
			}
		}

		void TutorialUI::LateUpdate(float tick)
		{
			if (isDisplay == false)
			{
				nowState = TutorialState::NONE;

				tutorial->GetTextureUI("WeaponTutorial1")->SetEnable(false);
				tutorial->GetTextureUI("WeaponTutorial2")->SetEnable(false);
				tutorial->GetTextureUI("AltarTutorial")->SetEnable(false);

				dialog->SetIsEnable(true);
			}
		}

		void TutorialUI::StartAltarTutorial()
		{
			if (isCheckAltar == false)
			{
				nowState = TutorialState::ALTAR_TUTORIAL;
				isDisplay = true;

			}
		}
	}

}