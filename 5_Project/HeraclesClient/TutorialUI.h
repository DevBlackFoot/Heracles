#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		class DialogUI;
		using GraphicsEngineSpace::TextureBase;

		// ���� Ʃ�丮�� ����
		enum class TutorialState
		{
			WEAPON_TUTORIAL1,
			WEAPON_TUTORIAL2,

			ALTAR_TUTORIAL,

			NONE
		};

		class TutorialUI : public ComponentBase
		{
			// ĳ�̿뵵.
			GraphicsEngineSpace::Canvas* tutorial;
			// ������Ʈ �ϳ� ĳ��
			std::shared_ptr<UI::DialogUI> dialog;

			// ���� ����
			TutorialState nowState;

			// ���� â�� �� �ִ°�
			bool isDisplay;

			// Ʃ�丮���� �� �� �ô°�
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
