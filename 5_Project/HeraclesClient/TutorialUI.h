#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		class DialogUI;
		using GraphicsEngineSpace::TextureBase;

		// 현재 튜토리얼 상태
		enum class TutorialState
		{
			WEAPON_TUTORIAL1,
			WEAPON_TUTORIAL2,

			ALTAR_TUTORIAL,

			NONE
		};

		class TutorialUI : public ComponentBase
		{
			// 캐싱용도.
			GraphicsEngineSpace::Canvas* tutorial;
			// 컴포넌트 하나 캐싱
			std::shared_ptr<UI::DialogUI> dialog;

			// 현재 상태
			TutorialState nowState;

			// 현재 창이 떠 있는가
			bool isDisplay;

			// 튜토리얼을 한 번 봤는가
		public:
			static bool isCheckWeapon;
			static bool isCheckAltar;

		public:
			TutorialUI(std::weak_ptr<GameObject> obj);
			virtual ~TutorialUI();

			void Awake() override;
			void PreUpdate(float tick) override;
			void Update(float tick) override;
			void LateUpdate(float tick) override;

			void SetNowState(TutorialState state) { nowState = state; }
			TutorialState GetNowState() { return nowState; }

			bool GetIsDisplay() { return isDisplay; }

			void SetDialog(const std::shared_ptr<UI::DialogUI>& _dialog) { dialog = _dialog; }

			void StartAltarTutorial();
		};

	}
}
