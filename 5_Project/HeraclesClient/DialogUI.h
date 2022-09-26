#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		using GraphicsEngineSpace::TextureBase;

		// 대화 상태에 대해 나타내는 enumClass
		enum class DialogState
		{
			FIRST_CUT_SCENE,	// 첫 컷 씬

			STAGESTART,			// 스테이지 시작

			ALTAR,				// 제단

			CONTROL_TUTORIAL,	// 튜토리얼 상태.

			NONE				// 대화창 안뜨는 상태 = default
		};

		struct Dialog
		{
			// Dead스테이지 등.. 중복 가능한 type
			uint32 typeID;
			std::wstring contents;

			Dialog(uint32 id, const std::wstring& content)
			{
				typeID = id;
				contents = content;
			}
		};

		/**
		 * \brief 기본적으로 대화와 관련된 UI들을 관리하는 컴포넌트.
		 *
		 * 매 업데이트를 돌 때, 지정된 텍스트 프린트 스피드 만큼의 문장을 출력한다.
		 *
		 * 한 문장이 끝난 뒤, 포인터를 출력한다.
		 *
		 * 한 문장이 끝나기전 입력을 받으면 해당 내용을 모두 출력한다.
		 */
		class DialogUI : public ComponentBase
		{
			GraphicsEngineSpace::Canvas* dialogCanvas;
			GraphicsEngineSpace::Canvas* monologueCanvas;

			// 현재 나와야하는 대사 번호
			int nowIndex;
			// 어디까지 나와야하는가..
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

			// 출력할 모든 String 데이터를 가지고 있는 맵.
			std::vector<std::shared_ptr<Dialog>> startCutSceneDialogVector;
			std::vector<std::shared_ptr<Dialog>> startStageDialogVector;
			std::vector<std::shared_ptr<Dialog>> startAltarDialogVector;

			DialogState nowState;
			GraphicsEngineSpace::TextUI* nowText;

			// 난수 생성기..
			std::mt19937 engine;
			uniform_int_distribution<int> distribution;

			// 현재 텍스트가 나오는 중인가
			bool isTyping;

			// 컨트롤 튜토리얼이 나왔는가
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

			// 랜덤 돌려서 현재 인덱스 정하기
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


