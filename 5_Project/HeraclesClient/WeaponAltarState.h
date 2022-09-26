#pragma once

#include "ComponentBase.h"
#include "UIManager.h"

#include "AltarScreen.h"

namespace Heracles
{

	namespace Altar
	{
		class WeaponAltarState : public ComponentBase
		{
		public:
			bool activated = false;
			bool canBeTriggerd = false;

			ScreenState currScreen = ScreenState::NONE;

			int costWeaponUpgrade;

			std::map<ScreenState, std::shared_ptr<ScreenStateBase>> screenList;

		private:
			Canvas* weaponAltar;

			std::shared_ptr<UIManager> ui;
			 
		public:
			WeaponAltarState(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;

			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			inline std::shared_ptr<ScreenStateBase> GetScreenState(ScreenState state) { return screenList[state]; }
			inline std::shared_ptr<ScreenStateBase> GetCurrScreen() { return this->screenList[this->currScreen]; }
			inline void SetCurrState(ScreenState screen) { this->currScreen = screen; }
			inline void SetTrigger(bool b) { this->canBeTriggerd = b; }

			void Activate();
			void Deactivate();
		};


	}
}