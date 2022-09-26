#include "pch.h"
#include "AltarScreen.h"

#include "AltarState.h"
#include "DialogUI.h"
#include "WeaponAltarState.h"
#include "GameState.h"
#include "GraphicsManager.h"
#include "PlayerStateMachine.h"

namespace Heracles
{
	namespace Altar
	{
#pragma region DisableState
		DisableState::DisableState()
			:ScreenStateBase(ScreenState::DISABLE)
		{
		}
		void DisableState::EventCheck(AltarState* altar)
		{
			if (!this->running)
			{
				this->Enter(altar);
			}
		}

		void DisableState::EventCheck(WeaponAltarState* weaponAltar)
		{
			if (!this->running)
			{
				this->Enter(weaponAltar);
			}
		}

		void DisableState::Enter(AltarState* altar)
		{
			this->running = false;
			return this->Exit(altar, ScreenState::NONE);
		}

		void DisableState::Enter(WeaponAltarState* weaponAltar)
		{
			this->running = false;
			return this->Exit(weaponAltar, ScreenState::NONE);
		}

		void DisableState::Exit(AltarState* altar, ScreenState nextState)
		{
			altar->Deactivate();

			GameState::GetInstance()->interacting = false;
			altar->activated = false;
			altar->SetCurrState(nextState);
			return altar->GetCurrScreen()->EventCheck(altar);
		}

		void DisableState::Exit(WeaponAltarState* weaponAltar, ScreenState nextState)
		{
			weaponAltar->Deactivate();

			GameState::GetInstance()->interacting = false;
			weaponAltar->activated = false;
			weaponAltar->SetCurrState(nextState);
			return weaponAltar->GetCurrScreen()->EventCheck(weaponAltar);
		}
#pragma endregion

#pragma region EnhanceScreen
		EnhanceScreen::EnhanceScreen()
			:ScreenStateBase(ScreenState::ENHANCE)
		{
			for (int i = 0; i < 3; i++)
				isEnhanced[i] = 0;

			buttonName[0] = "EnhanceHPButton";
			buttonName[1] = "EnhanceArmorButton";
			buttonName[2] = "EnhanceAttackButton";
			buttonName[3] = "EnhanceAltarEnhanceSelectButton";
			buttonName[4] = "EnhanceAltarCurseSelectButton";
			buttonName[5] = "StartAltarClose";

		}

		void EnhanceScreen::EventCheck(AltarState* altar)
		{
			if (!this->running)
			{
				return this->Enter(altar);
			}

			// ���������� ���ذ��鼭 �ؽ�Ʈ �� �ٲ��ֱ�.
			int nowGemCount = playerState->memoryPieceNum;

			if (nowGemCount < altar->costHp)
				ui->SetTextUIColor("EnhanceHPGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, enhanceAltar);
			else
				ui->SetTextUIColor("EnhanceHPGemCount", { 1.0f, 1.f, 1.f, 1.0f }, enhanceAltar);

			if (nowGemCount < altar->costArmor)
				ui->SetTextUIColor("EnhanceArmorGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, enhanceAltar);
			else
				ui->SetTextUIColor("EnhanceArmorGemCount", { 1.0f, 1.f, 1.f, 1.0f }, enhanceAltar);

			if (nowGemCount < altar->costAttackPower)
				ui->SetTextUIColor("EnhanceAttackGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, enhanceAltar);
			else
				ui->SetTextUIColor("EnhanceAttackGemCount", { 1.0f, 1.f, 1.f, 1.0f }, enhanceAltar);

			// ��ȭâ�� �߰� ������.. ��� ��ư�� ���� �Ұ����ϰ�..
			if (altar->GetDialog()->GetIsTyping() == true)
			{
				SetAllButtonEnable(false);
				ui->SetButtonIsEnable("StartAltarClose", false, enhanceAltar);
			}
			else if(isNotice == true)
			{
				SetAllButtonEnable(false);
			}
			else
			{
				SetAllButtonEnable(true);
				ui->SetButtonIsEnable("StartAltarClose", true, enhanceAltar);
			}

			// tick�� 0�� �Ǹ�
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.0f;

				// �޽����� �����ְ�
				ui->SetSpriteIsEnable("EnhanceNotice", false, enhanceAltar);
			}

			// ��ư �Է½� �������� �� üũ
			if (ui->GetButtonState("EnhanceHPButton", enhanceAltar) == ButtonState::DOWN)
			{
				if (nowGemCount < altar->costHp)
				{
					ui->SetSpriteIsEnable("EnhanceNotice", true, enhanceAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;

				}
				else
				{
					playerState->memoryPieceNum -= altar->costHp;
					playerState->maxHp = playerState->maxHp + 20;
					playerState->currHp = playerState->maxHp;
					isEnhanced[0] += 1;

					sound->PlayEffect("PlayerUpgrade");

					// �ش� ��ư�� ��� �Ұ����ϰ� �ؾ���.
					if (isEnhanced[0] > 2)
					{
						ui->SetButtonIsEnable("EnhanceHPButton", false, enhanceAltar);

						ui->SetTextUIColor("EnhanceHPText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, enhanceAltar);
						ui->SetTextUIColor("EnhanceHPSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, enhanceAltar);
					}
					altar->GetDialog()->StartAltarDialog(0);

				}
			}

			if (ui->GetButtonState("EnhanceArmorButton", enhanceAltar) == ButtonState::DOWN)
			{
				if (nowGemCount < altar->costArmor)
				{
					ui->SetSpriteIsEnable("EnhanceNotice", true, enhanceAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;
				}
				else
				{
					playerState->memoryPieceNum -= altar->costArmor;

					//int armorPer = static_cast<int>(playerState->armor / 10);

					// �ʱ� ���� 0�̾�..
					playerState->armor = playerState->armor + 1.0f;
					isEnhanced[1] += 1;

					sound->PlayEffect("PlayerUpgrade");

					if (isEnhanced[1] > 2)
					{
						ui->SetButtonIsEnable("EnhanceArmorButton", false, enhanceAltar);

						ui->SetTextUIColor("EnhanceArmorText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, enhanceAltar);
						ui->SetTextUIColor("EnhanceArmorSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, enhanceAltar);
					}
					altar->GetDialog()->StartAltarDialog(1);

				}
			}

			if (ui->GetButtonState("EnhanceAttackButton", enhanceAltar) == ButtonState::DOWN)
			{
				if (nowGemCount < altar->costAttackPower)
				{
					ui->SetSpriteIsEnable("EnhanceNotice", true, enhanceAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;

				}
				else
				{
					playerState->memoryPieceNum -= altar->costAttackPower;

					int attackPer = static_cast<int>(playerState->attackPower / 10);

					playerState->attackPower = playerState->attackPower + attackPer;
					isEnhanced[2] += 1;

					sound->PlayEffect("PlayerUpgrade");

					if (isEnhanced[2] > 2)
					{
						ui->SetButtonIsEnable("EnhanceAttackButton", false, enhanceAltar);

						ui->SetTextUIColor("EnhanceAttackText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, enhanceAltar);
						ui->SetTextUIColor("EnhanceAttackSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, enhanceAltar);
					}

					altar->GetDialog()->StartAltarDialog(2);

				}
			}

			if (ui->GetButtonState("StartAltarClose", enhanceAltar) == ButtonState::DOWN)
				this->Exit(altar, ScreenState::DISABLE);



			if (ui->GetButtonState("EnhanceAltarCurseSelectButton", enhanceAltar) == ButtonState::DOWN)
				this->Exit(altar, ScreenState::REMOVE_CURSE);

			if (isNotice == true)
				noticeTickCount += Timer::GetInstance()->DeltaTime();

		}

		void EnhanceScreen::EventCheck(WeaponAltarState* weaponAltar)
		{
		}

		void EnhanceScreen::Enter(AltarState* altar)
		{
			this->running = true;

			// ��ü ���� ����ֱ�
			enhanceAltar->SetEnable(true);

			// ��Ƽ�� ��ȿȭ
			isNotice = false;
			noticeTickCount = 0.0f;
			// �޽����� �����ְ�
			ui->SetSpriteIsEnable("EnhanceNotice", false, enhanceAltar);

			// �� ��ư�� ���� �����ϰ� ������ݴϴ�.
			SetAllButtonEnable(true);

			altar->SetCurrState(ScreenState::ENHANCE);
		}

		void EnhanceScreen::Enter(WeaponAltarState* weaponAltar)
		{
		}

		void EnhanceScreen::Exit(AltarState* altar, ScreenState nextState)
		{
			this->running = false;

			enhanceAltar->SetEnable(false);

			return altar->GetScreenState(nextState)->EventCheck(altar);
		}

		void EnhanceScreen::Exit(WeaponAltarState* weaponAltar, ScreenState nextState)
		{
		}

		void EnhanceScreen::SetAllButtonEnable(bool enable)
		{
			// 0
			if (isEnhanced[0] < 2)
				ui->SetButtonIsEnable("EnhanceHPButton", enable, enhanceAltar);
			// 1
			if (isEnhanced[1] < 2)
				ui->SetButtonIsEnable("EnhanceArmorButton", enable, enhanceAltar);
			// 2
			if (isEnhanced[2] < 2)
				ui->SetButtonIsEnable("EnhanceAttackButton", enable, enhanceAltar);
			//ui->SetButtonIsEnable("StartAltarClose", enable, enhanceAltar);
			ui->SetButtonIsEnable("EnhanceAltarEnhanceSelectButton", enable, enhanceAltar);
			ui->SetButtonIsEnable("EnhanceAltarCurseSelectButton", enable, enhanceAltar);
		}
#pragma endregion

#pragma region RemoveCurseScreen
		RemoveCurseScreen::RemoveCurseScreen()
			:ScreenStateBase(ScreenState::REMOVE_CURSE)
		{
		}

		void RemoveCurseScreen::EventCheck(AltarState* altar)
		{
			if (!this->running)
			{
				this->Enter(altar);
			}

			// ���������� ���ذ��鼭 �ؽ�Ʈ �� �ٲ��ֱ�.
			int nowGemCount = playerState->memoryPieceNum;

			if (nowGemCount < altar->costCursePain)
				ui->SetTextUIColor("CursePainGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, curseAltar);
			else
				ui->SetTextUIColor("CursePainGemCount", { 1.0f, 1.f, 1.f, 1.0f }, curseAltar);

			if (nowGemCount < altar->costCurseMadness)
				ui->SetTextUIColor("CurseMadnessGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, curseAltar);
			else
				ui->SetTextUIColor("CurseMadnessGemCount", { 1.0f, 1.f, 1.f, 1.0f }, curseAltar);

			if (nowGemCount < altar->costCurseSight)
				ui->SetTextUIColor("CurseBlindGemCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, curseAltar);
			else
				ui->SetTextUIColor("CurseBlindGemCount", { 1.0f, 1.f, 1.f, 1.0f }, curseAltar);

			// ��ȭâ�� �߰� ������.. ��� ��ư�� ���� �Ұ����ϰ�..
			if (altar->GetDialog()->GetIsTyping() == true)
			{
				SetAllButtonEnable(false);
				ui->SetButtonIsEnable("StartAltarClose", false, curseAltar);
			}
			else if (isNotice == true)
			{
				SetAllButtonEnable(false);
			}
			else
			{
				SetAllButtonEnable(true);
				ui->SetButtonIsEnable("StartAltarClose", true, curseAltar);
			}

			// tick�� 0�� �Ǹ�
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.f;

				// �޽����� �����ְ�
				ui->SetSpriteIsEnable("CurseNotice", false, curseAltar);
			}

			// ��ư �Է½� �������� �� üũ
			if (ui->GetButtonState("CursePainButton", curseAltar) == ButtonState::DOWN)
			{
				if (playerState->memoryPieceNum < altar->costCursePain)
				{
					ui->SetSpriteIsEnable("CurseNotice", true, curseAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;
				}
				else
				{
					playerState->memoryPieceNum -= altar->costCursePain;
					GameState::GetInstance()->cursePain = false;

					sound->PlayEffect("CurseOff");

					// �ش� ��ư�� ��� �Ұ����ϰ� �ؾ���.
					ui->SetButtonIsEnable("CursePainButton", false, curseAltar);

					GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CursePainText");

					ui->SetTextUIColor("CursePainText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
					ui->SetTextUIColor("CursePainSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

					tmpText->SetText(L"������ ���� �ع�");
					tmpText = curseAltar->GetTextUI("CursePainSubText");
					tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n ������ 10% �����մϴ�.");

					GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CursePain");

					tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreePain"));

					// ��ȭâ ���
					altar->GetDialog()->StartAltarDialog(4);
				}
			}

			if (ui->GetButtonState("CurseMadnessButton", curseAltar) == ButtonState::DOWN)
			{
				if (playerState->memoryPieceNum < altar->costCurseMadness)
				{
					// ���� ���ڶ��ϴ�!
					ui->SetSpriteIsEnable("CurseNotice", true, curseAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;
				}
				else
				{
					playerState->memoryPieceNum -= altar->costCurseMadness;
					GameState::GetInstance()->curseMadness = false;

					sound->PlayEffect("CurseOff");

					ui->SetButtonIsEnable("CurseMadnessButton", false, curseAltar);

					GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseMadnessText");

					ui->SetTextUIColor("CurseMadnessText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
					ui->SetTextUIColor("CurseMadnessSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

					tmpText->SetText(L"������ ���� �ع�");
					tmpText = curseAltar->GetTextUI("CurseMadnessSubText");
					tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n ���� �̵��ӵ��� 10% �����մϴ�.");

					GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseMadness");

					tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeMadness"));

					altar->GetDialog()->StartAltarDialog(5);

				}
			}

			if (ui->GetButtonState("CurseBlindButton", curseAltar) == ButtonState::DOWN)
			{
				if (playerState->memoryPieceNum < altar->costCurseSight)
				{
					// ���� ���ڶ��ϴ�!
					ui->SetSpriteIsEnable("CurseNotice", true, curseAltar);
					sound->PlayEffect("AltarWrong");

					isNotice = true;
					noticeTickCount = 0.f;
				}
				else
				{
					playerState->memoryPieceNum -= altar->costCurseSight;
					GameState::GetInstance()->curseSight = false;

					sound->PlayEffect("CurseOff");

					ui->SetButtonIsEnable("CurseBlindButton", false, curseAltar);

					GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseBlindText");

					ui->SetTextUIColor("CurseBlindText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
					ui->SetTextUIColor("CurseBlindSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

					tmpText->SetText(L"������ ���� �ع�");
					tmpText = curseAltar->GetTextUI("CurseBlindSubText");
					tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n �þ߰� �����ο����ϴ�.");

					GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseBlind");

					tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeBlind"));

					altar->GetDialog()->StartAltarDialog(3);

				}
			}

			if (ui->GetButtonState("StartAltarClose", curseAltar) == ButtonState::DOWN)
			{
				GraphicsEngineSpace::ButtonUI* button = curseAltar->GetButtonUI("StartAltarClose");
				button->SetButtonState(ButtonState::DEFAULT);

				this->Exit(altar, ScreenState::DISABLE);
			}


			if (ui->GetButtonState("CurseAltarEnhanceSelectButton", curseAltar) == ButtonState::DOWN)
			{
				GraphicsEngineSpace::ButtonUI* button = curseAltar->GetButtonUI("CurseAltarEnhanceSelectButton");
				button->SetButtonState(ButtonState::DEFAULT);
				this->Exit(altar, ScreenState::ENHANCE);
			}

			// ��Ƽ���� ������ ����..
			if(isNotice == true)
				noticeTickCount += Timer::GetInstance()->DeltaTime();

			
		}

		void RemoveCurseScreen::EventCheck(WeaponAltarState* weaponAltar)
		{
		}

		void RemoveCurseScreen::Enter(AltarState* altar)
		{
			this->running = true;

			// ��ü ���� ����ֱ�
			curseAltar->SetEnable(true);
			altar->SetCurrState(ScreenState::REMOVE_CURSE);

			// ��Ƽ�� ��ȿȭ
			isNotice = false;
			noticeTickCount = 0.0f;
			// �޽����� �����ְ�
			ui->SetSpriteIsEnable("CurseNotice", false, curseAltar);

			// �� ��ư�� ���� �����ϰ� ������ݴϴ�.
				// �� ���̶� false�� ����Ǹ� -> ������Ʈ ���¿� ���� ������� �ʽ��ϴ�..
			SetAllButtonEnable(true);

			// ���� ������Ʈ�� ���¿� ���� �������ݴϴ�.
			if (GameState::GetInstance()->cursePain != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CursePainText");

				ui->SetTextUIColor("CursePainText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CursePainSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"������ ���� �ع�");
				tmpText = curseAltar->GetTextUI("CursePainSubText");
				tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n ������ 10% �����մϴ�.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CursePain");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreePain"));
			}
			if (GameState::GetInstance()->curseMadness != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseMadnessText");

				ui->SetTextUIColor("CurseMadnessText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CurseMadnessSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"������ ���� �ع�");
				tmpText = curseAltar->GetTextUI("CurseMadnessSubText");
				tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n ���� �̵��ӵ��� 10% �����մϴ�.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseMadness");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeMadness"));
			}
			if (GameState::GetInstance()->curseSight != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseBlindText");

				ui->SetTextUIColor("CurseBlindText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CurseBlindSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"������ ���� �ع�");
				tmpText = curseAltar->GetTextUI("CurseBlindSubText");
				tmpText->SetText(L" ����� ���ָ� �ع����\n\n\n\n �þ߰� �����ο����ϴ�.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseBlind");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeBlind"));
			}

			// �ʱ� ���� ȭ���̱� ������ ������ �����ش�.
			GameState::GetInstance()->interacting = true;
		}

		void RemoveCurseScreen::Enter(WeaponAltarState* weaponAltar)
		{
		}

		void RemoveCurseScreen::Exit(AltarState* altar, ScreenState nextState)
		{
			this->running = false;

			curseAltar->SetEnable(false);

			return altar->GetScreenState(nextState)->EventCheck(altar);
		}

		void RemoveCurseScreen::Exit(WeaponAltarState* weaponAltar, ScreenState nextState)
		{
		}

		void RemoveCurseScreen::SetAllButtonEnable(bool enable)
		{
			if (GameState::GetInstance()->cursePain == true)
				ui->SetButtonIsEnable("CursePainButton", enable, curseAltar);

			if (GameState::GetInstance()->curseMadness == true)
				ui->SetButtonIsEnable("CurseMadnessButton", enable, curseAltar);

			if (GameState::GetInstance()->curseSight == true)
				ui->SetButtonIsEnable("CurseBlindButton", enable, curseAltar);

			//ui->SetButtonIsEnable("StartAltarClose", enable, enhanceAltar);
			ui->SetButtonIsEnable("CurseAltarEnhanceSelectButton", enable, curseAltar);
			ui->SetButtonIsEnable("CurseAltarCurseSelectButton", enable, curseAltar);
		}
#pragma endregion

#pragma region NoneState
		NoneState::NoneState()
			:ScreenStateBase(ScreenState::NONE)
		{
		}
		void NoneState::EventCheck(AltarState* altar)
		{
			if (altar->activated && altar->canBeTriggerd)
				altar->Activate();

		}

		void NoneState::EventCheck(WeaponAltarState* weaponAltar)
		{
			if (weaponAltar->activated && weaponAltar->canBeTriggerd)
				weaponAltar->Activate();
		}

		void NoneState::Enter(AltarState* altar)
		{
		}

		void NoneState::Enter(WeaponAltarState* weaponAltar)
		{
		}

		void NoneState::Exit(AltarState* altar, ScreenState nextState)
		{
		}

		void NoneState::Exit(WeaponAltarState* weaponAltar, ScreenState nextState)
		{
		}

#pragma endregion

#pragma region Weapon
		WeaponScreen::WeaponScreen()
			: ScreenStateBase(ScreenState::WEAPON)
			, isUpgrade(false)
		{
			weaponAltarCanvas = ui->GetCanvas("WeaponAltar");
		}

		void WeaponScreen::EventCheck(AltarState* altar)
		{
		}

		void WeaponScreen::EventCheck(WeaponAltarState* weaponAltar)
		{
			if (!this->running)
			{
				return this->Enter(weaponAltar);
			}

			int nowCoinCount = playerState->money;

			if (nowCoinCount < weaponAltar->costWeaponUpgrade)
			{
				ui->SetTextUIColor("WeaponCoinCount", { 0.6667f, 0.3020f, 0.4118f, 1.0f }, weaponAltarCanvas);
			}
			else
			{
				ui->SetTextUIColor("WeaponCoinCount", { 1.0f, 1.0f, 1.0f, 1.0f }, weaponAltarCanvas);
			}

			// ��ư ���� Tick
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.f;

				// �޽����� �����
				ui->SetSpriteIsEnable("WeaponNotice", false, weaponAltarCanvas);

				// ��ư�� �������� ���ϰ� ���ݴϴ�
				SetAllButtonEnable(true);
			}


			// ���⸦ �ϳ��� ����ϱ� ������ �÷��̾� ������ �ʿ� ����..
			if (ui->GetButtonState("Weapon1Button", weaponAltarCanvas) == ButtonState::DOWN)
			{
				if (nowCoinCount < weaponAltar->costWeaponUpgrade)
				{
					isNotice = true;
					noticeTickCount = 0.f;
					sound->PlayEffect("AltarWrong");

					ui->SetSpriteIsEnable("WeaponNotice", true, weaponAltarCanvas);
					SetAllButtonEnable(false);
				}
				else
				{
					playerState->money -= weaponAltar->costWeaponUpgrade;
					// ���⿡ ���� �ϱ�.
					//playerState->GetWeapon()
					sound->PlayEffect("UIAltarSelect");

					isUpgrade = true;

					// �ش� ��ư ��� �Ұ��� �ϰ�.
					ui->SetButtonIsEnable("Weapon1Button", false, weaponAltarCanvas);

					ui->SetTextUIColor("Weapon1Text", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, weaponAltarCanvas);
					ui->SetTextUIColor("Weapon1SubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, weaponAltarCanvas);
				}
			}

			if (ui->GetButtonState("WeaponAltarClose", weaponAltarCanvas) == ButtonState::DOWN)
				this->Exit(weaponAltar, ScreenState::DISABLE);

			if (isNotice == true)				
				noticeTickCount += Timer::GetInstance()->DeltaTime();
		}

		void WeaponScreen::Enter(AltarState* altar)
		{
		}

		void WeaponScreen::Enter(WeaponAltarState* weaponAltar)
		{
			this->running = true;

			weaponAltarCanvas->SetEnable(true);
			weaponAltar->SetCurrState(ScreenState::WEAPON);

			// ��Ƽ�� ��ȿȭ
			isNotice = false;
			noticeTickCount = 0.0f;
			// �޽����� �����ְ�
			ui->SetSpriteIsEnable("WeaponNotice", false, weaponAltarCanvas);

			GameState::GetInstance()->interacting = true;

		}

		void WeaponScreen::Exit(AltarState* altar, ScreenState nextState)
		{
		}

		void WeaponScreen::Exit(WeaponAltarState* weaponAltar, ScreenState nextState)
		{
			this->running = false;
			weaponAltarCanvas->SetEnable(false);

			return weaponAltar->GetScreenState(nextState)->EventCheck(weaponAltar);
		}

		void WeaponScreen::SetAllButtonEnable(bool enable)
		{
			if (isUpgrade != true)
				ui->SetButtonIsEnable("Weapon1Button", enable, weaponAltarCanvas);
		}

#pragma endregion
	}
}
