#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		using GraphicsEngineSpace::TextureBase;

		// ��ȭ ���¿� ���� ��Ÿ���� enumClass
		enum class DialogState
		{
			FIRST_CUT_SCENE,	// ù �� ��

			STAGESTART,			// �������� ����

			ALTAR,				// ����

			CONTROL_TUTORIAL,	// Ʃ�丮�� ����.

			NONE				// ��ȭâ �ȶߴ� ���� = default
		};

		struct Dialog
		{
			// Dead�������� ��.. �ߺ� ������ type
			uint32 typeID;
			std::wstring contents;

			Dialog(uint32 id, const std::wstring& content)
			{
				typeID = id;
				contents = content;
			}
		};

		/**
		 * \brief �⺻������ ��ȭ�� ���õ� UI���� �����ϴ� ������Ʈ.
		 *
		 * �� ������Ʈ�� �� ��, ������ �ؽ�Ʈ ����Ʈ ���ǵ� ��ŭ�� ������ ����Ѵ�.
		 *
		 * �� ������ ���� ��, �����͸� ����Ѵ�.
		 *
		 * �� ������ �������� �Է��� ������ �ش� ������ ��� ����Ѵ�.
		 */
		class DialogUI : public ComponentBase
		{
			GraphicsEngineSpace::Canvas* dialogCanvas;
			GraphicsEngineSpace::Canvas* monologueCanvas;

			// ���� ���;��ϴ� ��� ��ȣ
			int nowIndex;
			// ������ ���;��ϴ°�..
			int charCount;
			bool isSetCharCount;
			bool isEnterTitle;
			bool titleSet;

			float textSpeed;
			float moveSpeed;

			float tickCount;

			wchar_t nowChar;
			bool flickPointer;
			bool firstFlick;
			float flickTime;

			float flickTickCount;

			// ����� ��� String �����͸� ������ �ִ� ��.
			std::vector<std::shared_ptr<Dialog>> startCutSceneDialogVector;
			std::vector<std::shared_ptr<Dialog>> startStageDialogVector;
			std::vector<std::shared_ptr<Dialog>> startAltarDialogVector;

			DialogState nowState;
			GraphicsEngineSpace::TextUI* nowText;

			// ���� ������..
			std::mt19937 engine;
			uniform_int_distribution<int> distribution;

			// ���� �ؽ�Ʈ�� ������ ���ΰ�
			bool isTyping;

			// ��Ʈ�� Ʃ�丮���� ���Դ°�
		public:
			static bool isCheckedControl;

		public:
			DialogUI(std::weak_ptr<GameObject> obj);
			virtual ~DialogUI();

			void Awake() override;
			void Start() override;
			void PreUpdate(float tick) override;
			void Update(float tick) override;
			void LateUpdate(float tick) override;

			void SetNowState(DialogState state) { nowState = state; }
			DialogState GetNowState() { return nowState; }

			// ���� ������ ���� �ε��� ���ϱ�
			void StartNowDialog(int deadStage);
			void StartAltarDialog(int altarIdx);

			void SetDialogCanvasEnable(bool isEnable) { dialogCanvas->SetEnable(isEnable); }

			bool GetIsTyping() { return isTyping; }

		private:
			void CreateStartCutSceneDialog();
			void CreateStartDialog();
			void CreateAltarDialog();

			GraphicsEngineSpace::TextUI* SelectText();
			void SetPointer(bool isSetPointer);
		};
	}
}


