#include "pch.h"
#include "DialogUI.h"

#include "ComponentBase.h"
#include "SoundManager.h"
#include "UIManager.h"

#include "GameState.h"

// ������ �Լ���
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
				// ù �ƾ�����..
				// �ؽ�Ʈ�� �����ϴ� UI
				nowText = SelectText();

				// �����ϴ� �ؽ�Ʈ
				std::wstring tempStr = startCutSceneDialogVector.at(nowIndex)->contents;
				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// ��� �� �� ������ Ÿ���� ����
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
				// �� ���� �ݵ�� nowIndex�� �ٲ�� �־����.
				std::wstring tempStr = startStageDialogVector.at(nowIndex)->contents;

				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// ��� �� �� ������ Ÿ���� ����
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
				// �� ���� �ݵ�� nowIndex�� �ٲ�� �־����.
				std::wstring tempStr = startAltarDialogVector.at(nowIndex)->contents;

				if (isSetCharCount == false)
				{
					charCount = tempStr.size();
					isSetCharCount = true;

					// ��� �� �� ������ Ÿ���� ����
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

				// �����Է��� �� �Ǳ� ������ �Ҹ��� ������..
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

				// ó���� ������ ���̰�!
				if (firstFlick == false)
				{
					flickTickCount = 0;
					flickPointer = true;
					firstFlick = true;
				}
			}

			// �̰� ��� ��ȭâ�̵� �Ǿ����..
			if (charCount < 0)
			{
				charCount = 0;
			}

			// ������
			if (charCount == 0 && nowState == DialogState::FIRST_CUT_SCENE)
			{
				SetPointer(flickPointer);
			}

			if (charCount == 0 && (nowState == DialogState::ALTAR || nowState == DialogState::STAGESTART))
			{
				GraphicsEngineSpace::TextureUI* pointer = dialogCanvas->GetTextureUI("DialogPointer");
				pointer->SetEnable(flickPointer);
			}

			// �ε����� �Ѱ��ִ� ���� Ű �Է�����
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
					// ��� �� �� ���� Ÿ���� ��
					isTyping = false;
				}
				else
				{
					charCount = 0;
				}

				// enter ���� ����. �� ��������.
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
					// ��ȭâ ���ֱ�
					nowState = DialogState::NONE;
					// ��� �� �� ���� Ÿ���� ��
					isTyping = false;

					firstFlick = false;

					GameState::GetInstance()->interacting = false;

					// �� �� ó���̸� Ʃ�丮�� ����ֱ�.
					if (isCheckedControl == false)
					{
						isCheckedControl = true;
						nowState = DialogState::CONTROL_TUTORIAL;

						// ���� ���� �ֵ� �����ֱ�..
						dialogCanvas->SetEnable(false);

						SoundManager::GetInstance()->PlayEffect("TutorialUI");

						GameState::GetInstance()->interacting = true;
					}
				}
				else
				{
					charCount = 0;
				}

				// enter ���� ����. �� ��������.
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
				// ��ǥġ���� ���� �÷��ݴϴ�..
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
				// Index�� ���� �����ϸ�..
				if (nowIndex == 6)
				{
					// ��� �� �� ���� Ÿ���� ��
					isTyping = false;

					nowIndex = 0;
					nowState = DialogState::NONE;

					// ������ �͵� ���󺹱�.
					isEnterTitle = false;
					// ù ģ�� ��ġ �̵�.
					GraphicsEngineSpace::TextUI* tempText = monologueCanvas->GetTextUI("Monologue0");
					Vector nowPos = tempText->GetPosition();
					nowPos.y = 0.0f;
					tempText->SetPosition(nowPos);

					// ������ ģ�� �����ֱ�.
					tempText = monologueCanvas->GetTextUI("Monologue5");
					tempText->SetEnable(false);

					monologueCanvas->SetEnable(false);



					SceneManager::GetInstance()->LoadScene(TEXT("StartCurseMap"));
				}
			}
		}

		void DialogUI::StartNowDialog(int deadStage)
		{
			// 0 -> ���� ���
			// 1, 2, 3 -> ���� �������� 1
			// 4, 5, 6 -> ���� �������� 2

			// �������� ����
				// 1 = ����
				// 2, 3, 4 = ����
				// 5 = ���׷��̵�
				// 6, 7, 8 = ����

			// ���� �� �� char������ false�� ������
			isSetCharCount = false;
			GameState::GetInstance()->interacting = true;

			if (deadStage == 0)
			{
				nowIndex = 0;

				nowState = DialogState::STAGESTART;
				return;
			}

			// ���� ������ 0, 1, 2�� ������. �ű⿡ ���� �������� * 3�� �ؼ� �ε����� �̴´�.
				// rand + 3 * (deadstage - 1);
			engine = std::mt19937((unsigned int)time(NULL));

			// ���� ���� ������ ���ʷ����� �����.
			auto generator = bind(distribution, engine);

			int rand = generator() % 3;

			// ������ �ش� �ε����� �ִ� �༮�� �����;���.
			nowIndex = (3 * deadStage) - rand;

			if (nowIndex > 18)
				nowIndex = 18;

			nowState = DialogState::STAGESTART;
		}

		void DialogUI::StartAltarDialog(int altarIdx)
		{
			// 0 - hp
			// 1 - ����
			// 2 - ���ݷ�
			// 3 - ����ε�
			// 4 - ����
			// 5 - ����
			isSetCharCount = false;

			engine = std::mt19937((unsigned int)time(NULL));

			// �⺻������ 0, 1 �� ���ϴ� ��.
			auto generator = bind(distribution, engine);

			int rand = generator() % 2;

			nowIndex = rand + 2 * altarIdx;

			if (nowIndex > 11)
				nowIndex = 11;

			nowState = DialogState::ALTAR;
		}

		// ������ ����� ����..
		void DialogUI::CreateStartCutSceneDialog()
		{
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(0, L"Episode VIII : ����޵����� �߻��� ����");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(1, L" ...\n\n\n �ð��� �󸶳� �帥����.\n\n\n �ƴ�, ��� �ð��� �󸶳� �帥������ �߿�ġ �ʴ�.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(2, L" ���� ���� �Ƴ��� ���̵��� ���� �� ��.\n\n\n �װ͸��� �߿��� ���� ��.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(3, L" �����ϰ� ��ѷ��� �߰ſ� �� ������ ��.\n\n\n ������ �ľ���� �׵��� ���� �기 ������ ���� �̷길ŭ ���\n\n\n �ٽô� ������ �긮�� ���� ���̶� ����������...\n\n\n ǥ���� �Ҿ���� �� ���� �� �ٽ� �߰ſ� ������ �귯���ȴ�.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(4, L" ���� ������ �Ҿ���� ä\n\n\n ���ž��� �̷ﳽ ������ ����� 7��.");
			startCutSceneDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(5, L" �׵鿡�� �����̳��� �����ϱ� ����.\n\n\n 8��° ������ �����Ѵ�.\n\n\n �ε�, � ���� ����� ���ֿ��� ��� �׵��� ������ �� �� �ֱ�.");
			startCutSceneDialogVector.emplace_back(tempDialog);
		}

		void DialogUI::CreateStartDialog()
		{
			// ���� ��ĭ
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(0, L" ������ �Դ� ���̶�...\n\n\n\n �����غ���.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(1, L" ��...\n\n\n\n ó�� ���� ���� �ʱ�");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(1, L" �̰� ���� ���� �Ƿ��ΰ�?");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(1, L" ... �ŵ��� �˸� �뿩�� �Ͻðڱ�");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(2, L" ��...\n\n\n\n �� �� �� �غ���");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(2, L" .......");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(2, L" �̹� �÷��� ���� �ʱ�...");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(3, L" �̷� ��û�� ������.\n\n\n\n �� �ڽſ��� �����Ÿ��� �����±�");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(3, L" �� ������.. �������� ���ӵǴ� ���� �𸣰ڱ�");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(3, L" ... �������� ���̽��� ���Ⱑ �׸���..\n\n\n\n � ���͵簣�� ���İ��� ���� ���� �� �� �־��µ� ���̾�.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(4, L" ���� �ܿ� �� �Ա�.");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(4, L" ... \n\n\n\n �״� ������ ������ �ͼ������� �ʾ�");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(4, L" �������� ����ͱ� �׵���.");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(5, L" .....");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(5, L" �� �� °�ΰ���\n\n\n\n ���� �� ���� ���ڱ�...");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(5, L" ... ���̽ÿ�, ���� ���� �̷� �÷��� �ֽʴϱ�.\n\n\n\n �ε� �̸� �غ��� �� �ִ� ���� �ּҼ�");
			startStageDialogVector.emplace_back(tempDialog);

			tempDialog = std::make_shared<Dialog>(6, L" ��...���̾�\n\n\n\n ���ݸ� �� ��������");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(6, L" ...\n\n\n\n ���� ���� �ʾҾ�");
			startStageDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(6, L" �� �÷��� ������ �� �־��µ�..\n\n\n\n �ƽ���");
			startStageDialogVector.emplace_back(tempDialog);
		}

		void DialogUI::CreateAltarDialog()
		{
			// ���� ��ĭ
			// 102 = ü��
			std::shared_ptr<Dialog> tempDialog = std::make_shared<Dialog>(102, L" ���� �ܷ��� ü�º��� ��������\n\n\n\n �� ���� ü���̸� ���� ������ �ʰھ�");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(102, L" ��������. ����");
			startAltarDialogVector.emplace_back(tempDialog);

			// 103 = ����
			tempDialog = std::make_shared<Dialog>(103, L" ���� ������ �� ���Ϳ��� �����̾�");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(103, L" ���� ���� �ܴ�������.\n\n\n\n �� ������ ���̷������ ���ݿ��� ���� ���ڱ�");
			startAltarDialogVector.emplace_back(tempDialog);

			// 104 = ���ݷ�
			tempDialog = std::make_shared<Dialog>(104, L" �����̰� ��Ƽ�� ���� ��ŭ ���� ��������.\n\n\n\n �� ���������� ���Ⱑ �׸���. �̸���.. ���̽�����");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(104, L" �� ������ ���̶�� �÷��� �ϼ��� �� �� �ְھ�");
			startAltarDialogVector.emplace_back(tempDialog);

			// 401 = �þ�
			tempDialog = std::make_shared<Dialog>(401, L" �� ���������� ���ֿ��� �ع��̶��\n\n\n\n �ϱ��� �ʴ±�");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(401, L" �ϱ��� �ʾ�.\n\n\n\n ��� ���� ���δٴ� ���� �̷��Գ� �߿��� ���̾���");
			startAltarDialogVector.emplace_back(tempDialog);

			// 402 = ����
			tempDialog = std::make_shared<Dialog>(402, L" �׵��� �ٽ� ���� �� �ֱ�...");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(402, L" �� ������ ���ִ� ��������� ���� �� ������ ������� �ʴ±�");
			startAltarDialogVector.emplace_back(tempDialog);

			// 403 = ����
			tempDialog = std::make_shared<Dialog>(403, L" ���� �ع�..\n\n\n\n ���� �� �������� �����̱�");
			startAltarDialogVector.emplace_back(tempDialog);
			tempDialog = std::make_shared<Dialog>(403, L" �̰ɷ� ���� ���̷������ �������� ���� ������ �� �ְھ�. \n\n\n\n ��ڱ�");
			startAltarDialogVector.emplace_back(tempDialog);
		}

		GraphicsEngineSpace::TextUI* DialogUI::SelectText()
		{
			GraphicsEngineSpace::TextUI* tmpText = nullptr;

			// �ε����� �ش��ϴ� �༮�� ��� �������ش�.
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
						//���� �ΰ��� SetEnable�� ����..
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

			// �ε����� �ش��ϴ� �༮�� ��� ������ �ش�.
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

			// ������ enableSeting
			tmpPointer->SetEnable(isSetPointer);
		}
	}
}
