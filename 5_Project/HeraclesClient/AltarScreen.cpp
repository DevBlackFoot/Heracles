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

			// 내부적으로 비교해가면서 텍스트 색 바꿔주기.
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

			// 대화창이 뜨고 있으면.. 모든 버튼을 선택 불가능하게..
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

			// tick이 0이 되면
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.0f;

				// 메시지를 지워주고
				ui->SetSpriteIsEnable("EnhanceNotice", false, enhanceAltar);
			}

			// 버튼 입력시 벌어지는 일 체크
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

					// 해당 버튼을 사용 불가능하게 해야함.
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

					// 초기 값이 0이야..
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

			// 전체 제단 띄워주기
			enhanceAltar->SetEnable(true);

			// 노티스 무효화
			isNotice = false;
			noticeTickCount = 0.0f;
			// 메시지를 지워주고
			ui->SetSpriteIsEnable("EnhanceNotice", false, enhanceAltar);

			// 각 버튼을 선택 가능하게 만들어줍니다.
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

			// 내부적으로 비교해가면서 텍스트 색 바꿔주기.
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

			// 대화창이 뜨고 있으면.. 모든 버튼을 선택 불가능하게..
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

			// tick이 0이 되면
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.f;

				// 메시지를 지워주고
				ui->SetSpriteIsEnable("CurseNotice", false, curseAltar);
			}

			// 버튼 입력시 벌어지는 일 체크
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

					// 해당 버튼을 사용 불가능하게 해야함.
					ui->SetButtonIsEnable("CursePainButton", false, curseAltar);

					GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CursePainText");

					ui->SetTextUIColor("CursePainText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
					ui->SetTextUIColor("CursePainSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

					tmpText->SetText(L"고통의 저주 해방");
					tmpText = curseAltar->GetTextUI("CursePainSubText");
					tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 방어력이 10% 증가합니다.");

					GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CursePain");

					tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreePain"));

					// 대화창 출력
					altar->GetDialog()->StartAltarDialog(4);
				}
			}

			if (ui->GetButtonState("CurseMadnessButton", curseAltar) == ButtonState::DOWN)
			{
				if (playerState->memoryPieceNum < altar->costCurseMadness)
				{
					// 젬이 모자랍니다!
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

					tmpText->SetText(L"광기의 저주 해방");
					tmpText = curseAltar->GetTextUI("CurseMadnessSubText");
					tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 적의 이동속도가 10% 감소합니다.");

					GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseMadness");

					tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeMadness"));

					altar->GetDialog()->StartAltarDialog(5);

				}
			}

			if (ui->GetButtonState("CurseBlindButton", curseAltar) == ButtonState::DOWN)
			{
				if (playerState->memoryPieceNum < altar->costCurseSight)
				{
					// 젬이 모자랍니다!
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

					tmpText->SetText(L"착란의 저주 해방");
					tmpText = curseAltar->GetTextUI("CurseBlindSubText");
					tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 시야가 자유로워집니다.");

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

			// 노티스가 떠있을 때만..
			if(isNotice == true)
				noticeTickCount += Timer::GetInstance()->DeltaTime();

			
		}

		void RemoveCurseScreen::EventCheck(WeaponAltarState* weaponAltar)
		{
		}

		void RemoveCurseScreen::Enter(AltarState* altar)
		{
			this->running = true;

			// 전체 제단 띄워주기
			curseAltar->SetEnable(true);
			altar->SetCurrState(ScreenState::REMOVE_CURSE);

			// 노티스 무효화
			isNotice = false;
			noticeTickCount = 0.0f;
			// 메시지를 지워주고
			ui->SetSpriteIsEnable("CurseNotice", false, curseAltar);

			// 각 버튼을 선택 가능하게 만들어줍니다.
				// 한 번이라도 false로 변경되면 -> 스테이트 상태에 따라 변경되지 않습니다..
			SetAllButtonEnable(true);

			// 게임 스테이트의 상태에 따라 변경해줍니다.
			if (GameState::GetInstance()->cursePain != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CursePainText");

				ui->SetTextUIColor("CursePainText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CursePainSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"고통의 저주 해방");
				tmpText = curseAltar->GetTextUI("CursePainSubText");
				tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 방어력이 10% 증가합니다.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CursePain");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreePain"));
			}
			if (GameState::GetInstance()->curseMadness != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseMadnessText");

				ui->SetTextUIColor("CurseMadnessText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CurseMadnessSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"광기의 저주 해방");
				tmpText = curseAltar->GetTextUI("CurseMadnessSubText");
				tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 적의 이동속도가 10% 감소합니다.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseMadness");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeMadness"));
			}
			if (GameState::GetInstance()->curseSight != true)
			{
				GraphicsEngineSpace::TextUI* tmpText = curseAltar->GetTextUI("CurseBlindText");

				ui->SetTextUIColor("CurseBlindText", { 0.4196f, 0.4510f, 0.4784f, 1.0f }, curseAltar);
				ui->SetTextUIColor("CurseBlindSubText", { 0.2196f, 0.2510f, 0.2784f, 1.0f }, curseAltar);

				tmpText->SetText(L"착란의 저주 해방");
				tmpText = curseAltar->GetTextUI("CurseBlindSubText");
				tmpText->SetText(L" 헤라의 저주를 해방시켜\n\n\n\n 시야가 자유로워집니다.");

				GraphicsEngineSpace::TextureUI* tmpTex = curseAltar->GetTextureUI("CurseBlind");

				tmpTex->SetTexture(GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager()->GetTexture("FreeBlind"));
			}

			// 초기 제단 화면이기 때문에 게임을 멈춰준다.
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

			// 버튼 관련 Tick
			if (noticeTickCount > 2.f)
			{
				isNotice = false;
				noticeTickCount = 0.f;

				// 메시지를 지우고
				ui->SetSpriteIsEnable("WeaponNotice", false, weaponAltarCanvas);

				// 버튼을 선택하지 못하게 해줍니다
				SetAllButtonEnable(true);
			}


			// 무기를 하나만 사용하기 때문에 플레이어 정보는 필요 없다..
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
					// 무기에 뭔가 하기.
					//playerState->GetWeapon()
					sound->PlayEffect("UIAltarSelect");

					isUpgrade = true;

					// 해당 버튼 사용 불가능 하게.
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

			// 노티스 무효화
			isNotice = false;
			noticeTickCount = 0.0f;
			// 메시지를 지워주고
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
