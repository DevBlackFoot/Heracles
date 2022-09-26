#pragma once

#include "ComponentBase.h"
#include "UIManager.h"

#include "AltarScreen.h"

namespace Heracles
{
	namespace UI
	{
		class DialogUI;
		class TutorialUI;
	}

	namespace Altar
	{
		class AltarState : public ComponentBase
		{
		public:
			bool activated = false;
			
			bool canBeTriggerd = false; // collider���� on

			ScreenState currScreen = ScreenState::NONE;
			
			int costHp;
			int costAttackPower;
			int costArmor;

			int costCursePain; // ���ֵ� ���ָ��� Ŭ������ �����
			int costCurseMadness;
			int costCurseSight;

			std::map<ScreenState,std::shared_ptr<ScreenStateBase>> screenList;
			
		private:
			Canvas* curseAltar;
			Canvas* enhanceAltar;

			// ������Ʈ �ϳ� ĳ��
			std::shared_ptr<UI::DialogUI> dialog;
			std::shared_ptr<UI::TutorialUI> tutorial;


		public:
			AltarState(std::weak_ptr<GameObject> obj);

			virtual void Awake() override;

			virtual void Update(float tick) override;
			virtual void LateUpdate(float tick) override;

			inline std::shared_ptr<ScreenStateBase> GetScreenState(ScreenState state) { return screenList[state]; }
			inline std::shared_ptr<ScreenStateBase> GetCurrScreen() { return this->screenList[this->currScreen]; }
			inline void SetCurrState(ScreenState screen) { this->currScreen = screen; }
			inline void SetTrigger(bool b) { this->canBeTriggerd = b; }

			void Activate();
			void Deactivate();

			void SetDialog(const std::shared_ptr<UI::DialogUI>& _dialog) { dialog = _dialog; }
			void SetTutorial(const std::shared_ptr<UI::TutorialUI>& _tutorial) { tutorial = _tutorial; }

			const std::shared_ptr<UI::DialogUI>& GetDialog() { return dialog; }

		private:

		};
	}
}

