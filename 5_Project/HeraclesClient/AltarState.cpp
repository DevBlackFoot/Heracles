#include "pch.h"
#include "AltarState.h"

#include "InputManager.h"
#include "GameState.h"
#include "PlayerStateMachine.h"
#include "SoundManager.h"
#include "TutorialUI.h"


namespace Heracles
{
	namespace Altar
	{
		AltarState::AltarState(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj, ComponentType::MONOBEHAVIOR)
			, costHp(2)
			, costAttackPower(2)
			, costArmor(2)
			, costCursePain(1)
			, costCurseMadness(1)
			, costCurseSight(1)
			, tutorial(nullptr)
		{
			this->screenList.emplace(ScreenState::DISABLE, std::make_shared<DisableState>());
			this->screenList.emplace(ScreenState::ENHANCE, std::make_shared<EnhanceScreen>());
			this->screenList.emplace(ScreenState::REMOVE_CURSE, std::make_shared<RemoveCurseScreen>());
			this->screenList.emplace(ScreenState::NONE, std::make_shared<NoneState>());
		}
		void AltarState::Awake()
		{
			curseAltar = UIManager::GetInstance()->GetCanvas("CurseAltar");
			enhanceAltar = UIManager::GetInstance()->GetCanvas("EnhanceAltar");
		}
		void AltarState::Update(float tick)
		{
			if (tutorial == nullptr)
				return;
			
			if (tutorial->GetIsDisplay() != true)
				this->GetCurrScreen()->EventCheck(this);


			if (this->canBeTriggerd) //  Ãæµ¹ + ÀÎÇ²
			{
				if (GameState::GetInstance()->interacting == false
					&& (InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
						|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
						)
				{
					SoundManager::GetInstance()->PlayEffect("AltarActivate");
					tutorial->StartAltarTutorial();
					
					this->activated = true;
				}
			}
		}

		void AltarState::LateUpdate(float tick)
		{

			if (this->currScreen == ScreenState::DISABLE)
			{
				this->Deactivate();
			}
		}

		void AltarState::Activate()
		{
			this->currScreen = ScreenState::REMOVE_CURSE;
			this->GetScreenState(this->currScreen)->EventCheck(this);
		}
		void AltarState::Deactivate()
		{
			curseAltar->SetEnable(false);
			enhanceAltar->SetEnable(false);
		}

	}
}
