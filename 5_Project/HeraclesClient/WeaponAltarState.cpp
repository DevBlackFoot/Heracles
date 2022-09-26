#include "pch.h"
#include "WeaponAltarState.h"

#include "InputManager.h"

namespace Heracles
{
	namespace Altar
	{
		WeaponAltarState::WeaponAltarState(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj, ComponentType::MONOBEHAVIOR)
			, costWeaponUpgrade(5)
		{
			this->screenList.emplace(ScreenState::DISABLE, std::make_shared<DisableState>());
			this->screenList.emplace(ScreenState::WEAPON, std::make_shared<WeaponScreen>());
			this->screenList.emplace(ScreenState::NONE, std::make_shared<NoneState>());
		}

		void WeaponAltarState::Awake()
		{
			weaponAltar = UIManager::GetInstance()->GetCanvas("WeaponAltar");
		}

		void WeaponAltarState::Update(float tick)
		{
			this->GetCurrScreen()->EventCheck(this);

			if(this->canBeTriggerd)
			{
				if(InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
				{
					this->activated = true;
				}
			}
		}

		void WeaponAltarState::LateUpdate(float tick)
		{
			if (this->currScreen == ScreenState::DISABLE)
			{
				this->Deactivate();
			}
		}

		void WeaponAltarState::Activate()
		{
			this->currScreen = ScreenState::WEAPON;
			this->GetScreenState(this->currScreen)->EventCheck(this);
		}

		void WeaponAltarState::Deactivate()
		{
			weaponAltar->SetEnable(false);
		}
	}
}
