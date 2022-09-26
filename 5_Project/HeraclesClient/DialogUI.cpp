#include "pch.h"
#include "DialogUI.h"

#include "ComponentBase.h"
#include "SoundManager.h"
#include "UIManager.h"

#include "GameState.h"

// 랜덤용 함수들
#include <random>
#include <ctime>

namespace Heracles
{
	namespace UI
	{
		bool DialogUI::isCheckedControl = false;

		DialogUI::DialogUI(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj, ComponentType::CANVAS)
			, nowIndex(0)
			, charCount(0)
			, isSetCharCount(false)
			, isEnterTitle(false)
			, titleSet(true)
			, textSpeed(10.0f)
			, moveSpeed(250.0f)
			, tickCount(0.0f)
			, nowChar(L' ')
			, flickPointer(false)
			, firstFlick(false)
			, flickTime(1.5f)
			, flickTickCount(0.0f)
			, nowState(DialogState::NONE)
			, isTyping(false)
		{
			dialogCanvas = UIManager::GetInstance()->GetCanvas("DialogCanvas");
			monologueCanvas = UIManager::GetInstance()->GetCanvas("MonologueCanvas");
			nowText = nullptr;
			CreateStartCutSceneDialog();
			CreateStartDialog();
			CreateAltarDialog();

			distribution = uniform_int_distribution<int>(0, 100);
		}

		DialogUI::~DialogUI()
		{
		}

		void DialogUI::Awake()
		{
			monologueCanvas->SetEnable(false);
			dialogCanvas->SetEnable(false);
		}

		void DialogUI::Start()
		{

		}

		void DialogUI::PreUpdate(float tick)
		{
			switch (nowState)
			{
				case DialogState::FIRST_CUT_SCENE:
				{

					monologueCanvas->SetEnable(true);
					break;
				}

				case DialogState::STAGESTART:
				case DialogState::ALTAR:
				{

					dialogCanvas->SetEnable(true);
					break;
				}

				case DialogState::CONTROL_TUTORIAL:
				{
					GraphicsEngineSpace::Canvas* tutorial = UIManager::GetInstance()->GetCanvas("TutorialCanvas");
					tutorial->SetEnable(true);
					UIManager::GetInstance()->SetSpriteIsEnable("ControlTutorial", true, tutorial);
					break;
				}

				case DialogState::NONE:
				{
					monologueCanvas->SetEnable(false);
					dialogCanvas->SetEnable(false);
				}
				break;

				default:
				break;
			}

			tickCount += textSpeed * tick;
			flickTickCount += flickTime * tick;
		}

		void DialogUI::Update(float tick)
		{
			if (nowState == DialogState::FIRST_CUT_SCENE)
			{
				// 첫 컷씬에서..
				// 텍스트가 들어가야하는 UI
				nowText = SelectText();

				// 들어가야하는 텍스트
				std::wstring tempStr = startCutSceneDialogVector.at(nowIndex)->contents;
				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// 사실 상 이 때부터 타이핑 시작
					isTyping = true;
				}

				int nowIdx = tempStr.length() - charCount;

				if (nowIdx == tempStr.length())
					nowIdx--;

				nowChar = tempStr.at(nowIdx);
				tempStr = tempStr.substr(0, tempStr.length() - charCount);

				nowText->SetText(tempStr);
			}

			if (nowState != DialogState::NONE)
			{
				nowText = dialogCanvas->GetTextUI("DialogText");
			}

			if (nowState == DialogState::STAGESTART)
			{
				// 이 전에 반드시 nowIndex가 바뀌어 있어야함.
				std::wstring tempStr = startStageDialogVector.at(nowIndex)->contents;

				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// 사실 상 이 때부터 타이핑 시작
					isTyping = true;
				}

				int nowIdx = tempStr.length() - charCount;

				if (nowIdx == tempStr.length())
					nowIdx--;

				nowChar = tempStr.at(nowIdx);
				tempStr = tempStr.substr(0, tempStr.length() - charCount);

				nowText->SetText(tempStr);
			}

			if (nowState == DialogState::ALTAR)
			{
				// 이 전에 반드시 nowIndex가 바뀌어 있어야함.
				std::wstring tempStr = startAltarDialogVector.at(nowIndex)->contents;

				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// 사실 상 이 때부터 타이핑 시작
					isTyping = true;
				}

				int nowIdx = tempStr.length() - charCount;

				if (nowIdx == tempStr.length())
					nowIdx--;

				nowChar = tempStr.at(nowIdx);
				tempStr = tempStr.substr(0, tempStr.length() - charCount);

				nowText->SetText(tempStr);
			}
		}

		void DialogUI::LateUpdate(float tick)
		{
			if (tickCount > 1)
			{
				charCount--;
				tickCount = 0;

				// 문자입력이 다 되기 전에는 소리를 내주자..
				if (charCount >= 0)
				{
					if (nowChar != L' ' && nowChar != L'\n')
						SoundManager::GetInstance()->PlayEffect("TextType");
				}
			}

			if (flickTickCount > 1)
			{
				flickPointer = !flickPointer;
				flickTickCount = 0;

				// 처음은 무조건 보이게!
				if (firstFlick == false)
				{
					flickTickCount = 0;
					flickPointer = true;
					firstFlick = true;
				}
			}

			// 이건 어느 대화창이든 되어야함..
			if (charCount < 0)
			{
				charCount = 0;
			}

			// 포인터
			if (charCount == 0 && nowState == DialogState::FIRST_CUT_SCENE)
			{
				SetPointer(flickPointer);
			}

			if (charCount == 0 && (nowState == DialogState::ALTAR || nowState == DialogState::STAGESTART))
			{
				GraphicsEngineSpace::TextureUI* pointer = dialogCanvas->GetTextureUI("DialogPointer");
				pointer->SetEnable(flickPointer);
			}

			// 인덱스를 넘겨주는 것을 키 입력으로
			if (nowState == DialogState::FIRST_CUT_SCENE &&
				(InputManager::GetInstance()->GetInputState(VK_RETURN, KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonB, XPadCommand::Push))
				)
			{
				if (nowIndex == 0 && charCount == 0 && isEnterTitle == false)
				{
					isEnterTitle = true;
					titleSet = false;
				}
				else if (charCount == 0 && titleSet == true)
				{
					isSetCharCount = false;
					nowIndex++;
					firstFlick = false;
					// 사실 상 이 때가 타이핑 끝
					isTyping = false;
				}
				else
				{
					charCount = 0;
				}

				// enter 누른 순간. 안 보여야함.
				SetPointer(false);
			}

			if ((nowState == DialogState::STAGESTART || nowState == DialogState::ALTAR)
				&& (InputManager::GetInstance()->GetInputState(VK_RETURN, KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonB, XPadCommand::Push))
				)
			{
				if (charCount == 0)
				{
					//isSetCharCount = false;
					// 대화창 꺼주기
					nowState = DialogState::NONE;
					// 사실 상 이 때가 타이핑 끝
					isTyping = false;

					firstFlick = false;

					GameState::GetInstance()->interacting = false;

					// 이 때 처음이면 튜토리얼 띄워주기.
					if (isCheckedControl == false)
					{
						isCheckedControl = true;
						nowState = DialogState::CONTROL_TUTORIAL;

						// 지금 나온 애들 없애주기..
						dialogCanvas->SetEnable(false);

						SoundManager::GetInstance()->PlayEffect("TutorialUI");

						GameState::GetInstance()->interacting = true;
					}
				}
				else
				{
					charCount = 0;
				}

				// enter 누른 순간. 안 보여야함.
				GraphicsEngineSpace::TextureUI* pointer = dialogCanvas->GetTextureUI("DialogPointer");
				pointer->SetEnable(false);
			}

			if (nowState == DialogState::CONTROL_TUTORIAL
				&& (InputManager::GetInstance()->GetInputState('F', KeyState::DOWN)
					|| InputManager::GetInstance()->GetGamePad()->IsPadCommand(XPadInput::ButtonX, XPadCommand::Push))
				)
			{
				nowState = DialogState::NONE;

				GraphicsEngineSpace::Canvas* tutorial = UIManager::GetInstance()->GetCanvas("TutorialCanvas");
				UIManager::GetInstance()->SetSpriteIsEnable("ControlTutorial", false, tutorial);
				//tutorial->SetEnable(false);

				SoundManager::GetInstance()->PlayEffect("TutorialPage");

				GameState::GetInstance()->interacting = false;
			}

			if (titleSet == false)
			{
				// 목표치까지 위로 올려줍니다..
				GraphicsEngineSpace::TextUI* tempText = SelectText();

				// -400.f..
				Vector nowPos = tempText->GetPosition();

				nowPos.y = nowPos.y - moveSpeed * tick;

				if (nowPos.y <= -400.0f)
				{
					nowPos.y = -400.0f;
					titleSet = true;
				}
				tempText->SetPosition(nowPos);

				SetPointer(false);
			}

			if (nowState == DialogState::FIRST_CUT_SCENE)
			{
				// Index의 끝에 도달하면..
				if (nowIndex == 6)
				{
					// 사실 상 이 때가 타이핑 끝
					isTyping = false;

					nowIndex = 0;
					nowState = DialogState::NONE;

					// 기존의 것들 원상복귀.
					isEnterTitle = false;
					// 첫 친구 위치 이동.
					GraphicsEngineSpace::TextUI* tempText = monologueCanvas->GetTextUI("Monologue0");
					Vector nowPos = tempText->GetPosition();
					nowPos.y = 0.0f;
					tempText->SetPosition(nowPos);

					// 마지막 친구 지워주기.
					tempText = monologueCanvas->GetTextUI("Monologue5");
					tempText->SetEnable(false);

					monologueCanvas->SetEnable(false);



					SceneManager::GetInstance()->LoadScene(TEXT("StartCurseMap"));
				}
			}
		}

		void DialogUI::StartNowDialog(int deadStage)
		{
			// 0 -> 시작 대사
			// 1, 2, 3 -> 죽은 스테이지 1
			// 4, 5, 6 -> 죽은 스테이지 2

			// 스테이지 서순
				// 1 = 제단
				// 2, 3, 4 = 전투
				// 5 = 업그레이드
				// 6, 7, 8 = 전투

			// 셋을 할 때 char세팅을 false로 해주자
			isSetCharCount = false;
			GameState::GetInstance()->interacting = true;

			if (deadStage == 0)
			{
				nowIndex = 0;

				nowState = DialogState::STAGESTART;
				return;
			}

			// 랜덤 돌려서 0, 1, 2가 나오면. 거기에 죽은 스테이지 * 3을 해서 인덱스를 뽑는다.
				// rand + 3 * (deadstage - 1);
			engine = std::mt19937((unsigned int)time(NULL));

			// 난수 쓰는 곳에서 제너레이터 만들기.
			auto generator = bind(distribution, engine);

			int rand = generator() % 3;

			// 벡터의 해당 인덱스에 있는 녀석을 가져와야함.
			nowIndex = (3 * deadStage) - rand;

			if (nowIndex > 18)
				nowIndex = 18;

			nowState = DialogState::STAGESTART;
		}

		void DialogUI::StartAltarDialog(int altarIdx)
		{
			// 0 - hp
			// 1 - 방어력
			// 2 - 공격력
			// 3 - 블라인드
			// 4 - 고통
			// 5 - 광기
			isSetCharCount = false;

			engine = std::mt19937((unsigned int)time(NULL));

			// 기본적으로 0, 1 을 더하는 식.
			auto generator = bind(distribution, engine);

			int rand = generator() % 2;

			nowIndex = rand + 2 * altarIdx;

			if (nowIndex > 11)
				nowIndex = 11;

			nowState = DialogState::ALTAR;
		}

		// 실제로 만들어 보자..
		void DialogUI::CreateStartCutSceneDialog()
		{
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(0, L"Episode VIII : 디오메데스의 야생마 생포");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(1, L" ...\n\n\n 시간이 얼마나 흐른거지.\n\n\n 아니, 사실 시간이 얼마나 흐른건지는 중요치 않다.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(2, L" 내가 나의 아내와 아이들을 죽인 그 죄.\n\n\n 그것만이 중요한 것일 뿐.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(3, L" 낭자하게 흩뿌려진 뜨거운 피 웅덩이 속.\n\n\n 차갑게 식어버린 그들을 보며 흘린 눈물이 강을 이룰만큼 깊어\n\n\n 다시는 눈물을 흘리지 못할 것이라 생각했지만...\n\n\n 표정을 잃어버린 볼 위로 또 다시 뜨거운 눈물이 흘러내렸다.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(4, L" 삶의 이유를 잃어버린 채\n\n\n 정신없이 이뤄낸 과업은 어느덧 7개.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(5, L" 그들에게 조금이나마 속죄하기 위해.\n\n\n 8번째 과업을 시작한다.\n\n\n 부디, 어서 빨리 헤라의 저주에서 벗어나 그들의 곁으로 갈 수 있길.");
			startCutSceneDialogVector.emplace_back(tempDialog);
		}

		void DialogUI::CreateStartDialog()
		{
			// 개행 네칸
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(0, L" 인육을 먹는 말이라...\n\n\n\n 시작해볼까.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(1, L" 으...\n\n\n\n 처음 부터 쉽지 않군");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(1, L" 이게 정말 나의 실력인가?");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(1, L" ... 신들이 알면 노여워 하시겠군");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(2, L" 후...\n\n\n\n 한 번 더 해보자");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(2, L" .......");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(2, L" 이번 시련은 쉽지 않군...");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(3, L" 이런 멍청이 같으니.\n\n\n\n 내 자신에게 욕짓거리가 나오는군");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(3, L" 이 고통은.. 언제까지 지속되는 건지 모르겠군");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(3, L" ... 대장장이 스미스의 무기가 그립군..\n\n\n\n 어떤 몬스터든간에 순식간에 베는 힘을 낼 수 있었는데 말이야.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(4, L" 이제 겨우 반 왔군.");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(4, L" ... \n\n\n\n 죽는 순간의 고통은 익숙해지지 않아");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(4, L" 지독히도 보고싶군 그들이.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(5, L" .....");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(5, L" 몇 번 째인거지\n\n\n\n 이제 셀 수도 없겠군...");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(5, L" ... 신이시여, 어찌 제게 이런 시련을 주십니까.\n\n\n\n 부디 이를 극복할 수 있는 힘을 주소서");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(6, L" 곧...곧이야\n\n\n\n 조금만 더 힘내보자");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(6, L" ...\n\n\n\n 끝이 머지 않았어");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(6, L" 곧 시련을 마주할 수 있었는데..\n\n\n\n 아쉽군");
			startStageDialogVector.emplace_back(tempDialog);
		}

		void DialogUI::CreateAltarDialog()
		{
			// 개행 네칸
			// 102 = 체력
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(102, L" 몸의 단련은 체력부터 시작이지\n\n\n\n 이 정도 체력이면 쉽게 죽지는 않겠어");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(102, L" 강해졌군. 좋아");
			startAltarDialogVector.emplace_back(tempDialog);

			// 103 = 방어력
			tempDialog = std::make_shared<Dialog>(103, L" 사자 가죽이 좀 두터워진 느낌이야");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(103, L" 몸이 아주 단단해졌어.\n\n\n\n 이 정도면 스켈레톤들의 공격에도 끄떡 없겠군");
			startAltarDialogVector.emplace_back(tempDialog);

			// 104 = 공격력
			tempDialog = std::make_shared<Dialog>(104, L" 몽둥이가 버티지 못할 만큼 힘이 강해졌군.\n\n\n\n 그 대장장이의 무기가 그리워. 이름이.. 스미스였나");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(104, L" 이 정도의 힘이라면 시련을 완수해 낼 수 있겠어");
			startAltarDialogVector.emplace_back(tempDialog);

			// 401 = 시야
			tempDialog = std::make_shared<Dialog>(401, L" 이 지긋지긋한 저주에서 해방이라니\n\n\n\n 믿기지 않는군");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(401, L" 믿기지 않아.\n\n\n\n 모든 것이 보인다는 것은 이렇게나 중요한 것이었군");
			startAltarDialogVector.emplace_back(tempDialog);

			// 402 = 고통
			tempDialog = std::make_shared<Dialog>(402, L" 그들을 다시 만날 수 있길...");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(402, L" 이 고통의 저주는 사라졌지만 마음 속 고통은 사라지지 않는군");
			startAltarDialogVector.emplace_back(tempDialog);

			// 403 = 광기
			tempDialog = std::make_shared<Dialog>(403, L" 저주 해방..\n\n\n\n 몸이 좀 가벼워진 느낌이군");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(403, L" 이걸로 인해 스켈레톤들의 움직임을 쉽게 따돌릴 수 있겠어. \n\n\n\n 기쁘군");
			startAltarDialogVector.emplace_back(tempDialog);
		}

		GraphicsEngineSpace::TextUI* DialogUI::SelectText()
		{
			GraphicsEngineSpace::TextUI* tmpText = nullptr;

			// 인덱스에 해당하는 녀석을 골라서 내보내준다.
			if (nowState == DialogState::FIRST_CUT_SCENE)
			{
				switch (nowIndex)
				{
					case 0:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue0");
						tmpText->SetEnable(true);
						break;
					}

					case 1:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue1");

						tmpText->SetEnable(true);
						break;
					}

					case 2:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue2");

						monologueCanvas->GetTextUI("Monologue1")->SetEnable(false);
						tmpText->SetEnable(true);
						break;
					}

					case 3:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue3");
						//위의 두개의 SetEnable을 해제..
						monologueCanvas->GetTextUI("Monologue2")->SetEnable(false);
						tmpText->SetEnable(true);
						break;
					}

					case 4:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue4");
						monologueCanvas->GetTextUI("Monologue3")->SetEnable(false);
						tmpText->SetEnable(true);
						break;
					}

					case 5:
					{
						tmpText = monologueCanvas->GetTextUI("Monologue5");
						monologueCanvas->GetTextUI("Monologue4")->SetEnable(false);

						tmpText->SetEnable(true);
						break;
					}

					default:
					break;
				}
			}

			return tmpText;
		}

		void DialogUI::SetPointer(bool isSetPointer)
		{
			GraphicsEngineSpace::TextureUI* tmpPointer = monologueCanvas->GetTextureUI("MonologuePointer");

			if (tmpPointer == nullptr)
				return;

			// 인덱스에 해당하는 녀석을 골라서 세팅해 준다.
			if (nowState == DialogState::FIRST_CUT_SCENE)
			{
				switch (nowIndex)
				{
					case 0:
					{
						if (isEnterTitle == false)
							tmpPointer->SetPosition({ 350.0f, 15.f, 0.2f });
						else
							tmpPointer->SetPosition({ 350.0f, -385.f, 0.2f });
						break;
					}

					case 1:
					{
						tmpPointer->SetPosition({ -110.0f, 100.f, 0.2f });
						break;
					}

					case 2:
					{
						tmpPointer->SetPosition({ -430.0f, 60.f, 0.2f });
						break;
					}

					case 3:
					{
						tmpPointer->SetPosition({ -30.0f, 140.f, 0.2f });

						break;
					}

					case 4:
					{
						tmpPointer->SetPosition({ -300.0f, 60.f, 0.2f });

						break;
					}

					case 5:
					{
						tmpPointer->SetPosition({ 60.0f, 100.f, 0.2f });

						break;
					}

					default:
					break;
				}
			}

			// 포인터 enableSeting
			tmpPointer->SetEnable(isSetPointer);
		}
	}
}
