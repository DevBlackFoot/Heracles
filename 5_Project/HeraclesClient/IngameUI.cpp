#include "pch.h"
#include "InGameUI.h"

#include "UIManager.h"
#include "GraphicsManager.h"

#include "GameState.h"
#include "SpriteAnimator.h"

namespace Heracles
{
	namespace UI
	{
		GraphicsEngineSpace::Canvas* InGameUI::canvas = nullptr;

		InGameUI::InGameUI(std::weak_ptr<GameObject> obj)
			: ComponentBase(obj)
			, hpBar(nullptr)
			, curse{ nullptr, nullptr, nullptr }
			, weaponSlot(nullptr)
			, memoryFrags(nullptr)
			, currentHP(0)
			, maxHP(0)
			, currentBossHP(0)
			, bossMaxHP(0)
		{
			// 인게임 UI의 Canvas 생성
			if (canvas != nullptr)
			{
				hpBar = canvas->GetProgressBar("HP_Bar");
				bossHPBar = canvas->GetProgressBar("Boss_HP_Bar");

				for (int i = 0; i < CurseCount; i++)
					curse[i] = canvas->GetButtonUI("CurseSlot" + std::to_string(i + 1));

				weaponSlot = canvas->GetTextureUI("WeaponSlotUI");
				memoryFrags = canvas->GetTextUI("MemoriesAmount");
				bossBackUI = canvas->GetTextureUI("BossBackUI");

				SetBossHPBarEnable(false);

				return;
			}

			canvas = UIManager::GetInstance()->CreateCanvas("InGameUI_Canvas");

			// 팩토리 & 리소스 매니저 포인터 참조
			GraphicsEngineSpace::Factory* factory = GraphicsManager::GetInstance()->GetRenderer()->CreateFactory();
			GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

			// Weapon
			GraphicsEngineSpace::TextureBase* weaponSlot = resourceManager->GetTexture("WeaponSlot");

			if (weaponSlot == nullptr)
				weaponSlot = factory->CreateTexture("WeaponSlot", "Resources/UI/Weapon_BG.png");

			CreateWeaponUI(weaponSlot);

			GraphicsEngineSpace::TextureBase* weapon = resourceManager->GetTexture("MasterSword");

			/*if (weapon == nullptr)
				weapon = factory->CreateTexture("MasterSword", "Resources/UI/MasterSword.png");

			weapon = resourceManager->GetTexture("Club");

			if (weapon == nullptr)
				weapon = factory->CreateTexture("Club", "Resources/UI/Club.png");

			weapon = resourceManager->GetTexture("SpikeClub");

			if (weapon == nullptr)
				weapon = factory->CreateTexture("SpikeClub", "Resources/UI/SpikeClub.png");

			weapon = resourceManager->GetTexture("AtenaSword");

			if (weapon == nullptr)
				weapon = factory->CreateTexture("AtenaSword", "Resources/UI/AtenaSword.png");

			weapon = resourceManager->GetTexture("SkeletonArm");

			if (weapon == nullptr)
				weapon = factory->CreateTexture("SkeletonArm", "Resources/UI/SkeletonArm.png");*/

			// HP Bar의 Front 및 Back 텍스쳐 얻어서 HP Bar UI 생성
			GraphicsEngineSpace::TextureBase* frontBar = resourceManager->GetTexture("HP_FrontBar");

			if (frontBar == nullptr)
				frontBar = factory->CreateTexture("HP_FrontBar", "Resources/UI/HP_Bar.png");
			
			GraphicsEngineSpace::TextureBase* backBar = resourceManager->GetTexture("HP_BackBar");

			if (backBar == nullptr)
				backBar = factory->CreateTexture("HP_BackBar", "Resources/UI/HP_Bar_BG.png");

			GraphicsEngineSpace::TextureBase* bossBack = resourceManager->GetTexture("BossBackUI");

			if (bossBack == nullptr)
				bossBack = factory->CreateTexture("BossBackUI", "Resources/UI/boss_HP.png");

			CreateHpBar(frontBar, backBar, bossBack);

			// Curse UI 생성
			GraphicsEngineSpace::TextureBase* curseSlot = resourceManager->GetTexture("CurseSlot");

			if (curseSlot == nullptr)
				curseSlot = factory->CreateTexture("CurseSlot", "Resources/UI/CurseIcon_BG.png");

			GraphicsEngineSpace::TextureBase* curse = resourceManager->GetTexture("Blind");

			if (curse == nullptr)
				curse = factory->CreateTexture("Blind", "Resources/UI/CurseIcon_Blind.png");

			curse = resourceManager->GetTexture("Madness");

			if (curse == nullptr)
				curse = factory->CreateTexture("Madness", "Resources/UI/CurseIcon_Madness.png");

			curse = resourceManager->GetTexture("Pain");

			if (curse == nullptr)
				curse = factory->CreateTexture("Pain", "Resources/UI/CurseIcon_Pain.png");

			CreateCurse(curseSlot);

			// Memory Fragment;
			GraphicsEngineSpace::TextureBase* memoryIcon = resourceManager->GetTexture("MemoryIcon");

			if (memoryIcon == nullptr)
				memoryIcon = factory->CreateTexture("MemoryIcon", "Resources/UI/MemoryGem_Icon.png");

			CreateMemoryFragment(memoryIcon);
		}

		InGameUI::~InGameUI()
		{
			canvas->SetEnable(false);
		}

		void InGameUI::SetHP(int HP)
		{
			if (HP > maxHP)
				currentHP = maxHP;
			else
				currentHP = HP;
		}

		void InGameUI::SetMaxHP(int maxHP)
		{
			this->maxHP = maxHP;
		}

		void InGameUI::SetBossHP(int HP)
		{
			if (HP > bossMaxHP)
				currentBossHP = bossMaxHP;
			else
				currentBossHP = HP;
		}

		void InGameUI::SetBossMaxHP(int maxHP)
		{
			bossMaxHP = maxHP;
		}

		void InGameUI::SetCurse(const std::string& curseName)
		{
			for (int i = 0; i < CurseCount; i++)
			{
				// 비활성화된 슬롯을 찾아서 저주를 적용한다.
				if (curse[i] != nullptr && curse[i]->GetEnable() != true)
				{
					GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

					curse[i]->SetEnable(true);

					// Curse Texture
					GraphicsEngineSpace::TextureBase* curse = resourceManager->GetTexture(curseName);
					canvas->GetTextureUI("Curse" + std::to_string(i + 1))->SetTexture(curse);

					// Hover시 출력할 텍스트
					canvas->GetTextUI("CurseName" + std::to_string(i + 1))->SetText(curseName);

					break;
				}
			}
		}

		void InGameUI::SetOffCurse(const std::string& curseName)
		{
			for (int i = 0; i < CurseCount; i++)
			{
				// 비활성화시킬 저주가 없는 경우
				if (curse[i] == nullptr)
					return;

				const std::string& curseNameText = canvas->GetTextUI("CurseName" + std::to_string(i + 1))->GetText();

				if (curseNameText.compare(curseName) != 0)
					continue;

				// 비활성화 시킬 때 뒤에 다른 슬롯에 있는 저주를 1칸씩 당겨준다.
				for (int k = i; k < CurseCount; k++)
				{
					if (k == CurseCount - 1 || curse[k + 1]->GetEnable() != true)
					{
						curse[k]->SetEnable(false);

						return;
					}

					GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();

					std::string nextCurseName = canvas->GetTextUI("CurseName" + std::to_string(k + 2))->GetText();

					canvas->GetTextureUI("Curse" + std::to_string(k + 1))->SetTexture(resourceManager->GetTexture(nextCurseName));
					canvas->GetTextUI("CurseName" + std::to_string(k + 1))->SetText(nextCurseName);
				}
			}
		}

		void InGameUI::SetMemories(int memories)
		{
			memoryFrags->SetText(std::to_string(memories));
		}

		void InGameUI::SetEquipment(const std::string& weaponName)
		{
			//GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();
			//
			//GraphicsEngineSpace::TextureBase* weaponTexture = resourceManager->GetTexture(weaponName);
			//
			//canvas->GetTextureUI("EquipmentSlotUI")->SetTexture(weaponTexture);
		}

		void InGameUI::SetSwitching(const std::string& weaponName)
		{
			//GraphicsEngineSpace::ResourceManager* resourceManager = GraphicsManager::GetInstance()->GetRenderer()->GetResourceManager();
			//
			//GraphicsEngineSpace::TextureBase* weaponTexture = resourceManager->GetTexture(weaponName);
			//
			//canvas->GetTextureUI("SwitchingSlotUI")->SetTexture(weaponTexture);
		}

		void InGameUI::Swap()
		{
			//// 스왑할 무기가 없다면 바꾸지 않는다.
			//GraphicsEngineSpace::TextureBase* switchingWeapon = canvas->GetTextureUI("SwitchingSlotUI")->GetTexture();
			//
			//if (switchingWeapon == nullptr)
			//	return;
			//
			//canvas->GetTextureUI("SwitchingSlotUI")->SetTexture(canvas->GetTextureUI("EquipmentSlotUI")->GetTexture());
			//canvas->GetTextureUI("EquipmentSlotUI")->SetTexture(switchingWeapon);
		}

		void InGameUI::SetBossHPBarEnable(bool isEnable)
		{
			bossBackUI->SetEnable(isEnable);
		}

		void InGameUI::Awake()
		{
			canvas->SetEnable(true);
		}

		void InGameUI::Start()
		{

		}

		void InGameUI::PreUpdate(float tick)
		{
			// 저주 해제
			if (!GameState::GetInstance()->cursePain)
			{
				this->SetOffCurse("Pain");
			}
			if (!GameState::GetInstance()->curseSight)
			{
				this->SetOffCurse("Blind");
			}
			if (!GameState::GetInstance()->curseMadness)
			{
				this->SetOffCurse("Madness");
			}
			SetMaxHP(GameState::GetInstance()->playerInfo->maxHp);
		}

		void InGameUI::Update(float tick)
		{
			SetHP(GameState::GetInstance()->playerInfo->currHp);
			SetMemories(GameState::GetInstance()->playerInfo->memoryPieceNum);

			SetBossHP(GameState::GetInstance()->bossHp);
			SetBossMaxHP(GameState::GetInstance()->bossMaxHP);

			// Test
			//if (InputManager::GetInstance()->GetInputState(VK_RETURN, KeyState::DOWN) == true)
			//	GetGameObject()->GetParent()->GetChild(_T("TestName"))->GetComponent<SpriteAnimator>()->Play();
		
			//if (InputManager::GetInstance()->GetInputState(VK_RETURN, KeyState::DOWN) == true)
			//{
			//	GetGameObject()->GetParent()->GetComponent<Camera>()->SetShaking(500.0f);
			//}
		}

		void InGameUI::LateUpdate(float tick)
		{
			// 체력 퍼센트 업데이트
			if (hpBar != nullptr)
			{
				hpBar->SetPercent(static_cast<float>(currentHP) / static_cast<float>(maxHP));

				std::string HPText = std::to_string(currentHP) + " / " + std::to_string(maxHP);

				canvas->GetTextUI("HP_Amount")->SetText(HPText);
			}

			if (bossBackUI->GetEnable() == true && bossHPBar != nullptr)
			{
				bossHPBar->SetPercent(static_cast<float>(currentBossHP) / static_cast<float>(bossMaxHP));

				std::string HPText = std::to_string(currentBossHP) + " / " + std::to_string(bossMaxHP);

				canvas->GetTextUI("Boss_HP_Amount")->SetText(HPText);
			}
		}

		void InGameUI::CreateHpBar(TextureBase* frontBar, TextureBase* backBar, TextureBase* bossBack)
		{
			// 체력바 이미지
			hpBar = canvas->CreateProgressBar("HP_Bar");
			hpBar->SetEnable(true);
			hpBar->SetPosition({ -695.0f, 500.0f, 0.2f });
			hpBar->SetWidth(500.0f);
			hpBar->SetHeight(50.0f);
			hpBar->SetDirection(GraphicsEngineSpace::Direction::LEFT);
			hpBar->SetFrontTexture(frontBar);
			hpBar->SetFrontBarTexCoord(0.0f, 0.0f, 470.0f, 72.0f);
			hpBar->SetBackTexture(backBar);
			hpBar->SetBackBarTexCoord(0.0f, 0.0f, 470.0f, 72.0f);
			hpBar->SetPercent(1.0f);
			
			// 체력바 텍스트
			GraphicsEngineSpace::TextUI* HPText = canvas->CreateTextUI("HP_Amount");
			HPText->SetAnchor({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::MIDDLE });
			HPText->SetPivot({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::MIDDLE });
			HPText->SetEnable(true);
			HPText->SetParent(hpBar);
			HPText->SetPosition({ -50.0f, -3.0f, -0.01f });
			HPText->SetWidth(200);
			HPText->SetHeight(50);
			HPText->SetFont("H2MJRE");
			HPText->SetFontSize(18.0f);
			HPText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			// Boss 체력바
			bossBackUI = canvas->CreateTextureUI("BossBackUI");
			bossBackUI->SetEnable(false);
			bossBackUI->SetPosition({ 0.0f, -450.0f, 0.15f });
			bossBackUI->SetWidth(712.0f);
			bossBackUI->SetHeight(217.0f);
			bossBackUI->SetTexture(bossBack);

			bossHPBar = canvas->CreateProgressBar("Boss_HP_Bar");
			bossHPBar->SetParent(bossBackUI);
			bossHPBar->SetEnable(true);
			bossHPBar->SetPosition({ 0.0f, 0.0f, -0.01f });
			bossHPBar->SetWidth(500.0f);
			bossHPBar->SetHeight(50.0f);
			bossHPBar->SetDirection(GraphicsEngineSpace::Direction::LEFT);
			bossHPBar->SetFrontTexture(frontBar);
			bossHPBar->SetFrontBarTexCoord(0.0f, 0.0f, 470.0f, 72.0f);
			bossHPBar->SetBackTexture(backBar);
			bossHPBar->SetBackBarTexCoord(0.0f, 0.0f, 470.0f, 72.0f);
			bossHPBar->SetPercent(1.0f);

			GraphicsEngineSpace::TextUI* bossHPText = canvas->CreateTextUI("Boss_HP_Amount");
			bossHPText->SetAnchor({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::MIDDLE });
			bossHPText->SetPivot({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::MIDDLE });
			bossHPText->SetEnable(true);
			bossHPText->SetParent(bossHPBar);
			bossHPText->SetPosition({ -50.0f, -3.0f, -0.01f });
			bossHPText->SetWidth(200);
			bossHPText->SetHeight(50);
			bossHPText->SetFont("H2MJRE");
			bossHPText->SetFontSize(18.0f);
			bossHPText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}

		void InGameUI::CreateCurse(TextureBase* curseIconSlot)
		{
			for (int i = 0; i < CurseCount; i++)
			{
				// Curse Slot, Hover기능을 위해 Button으로 생성
				curse[i] = canvas->CreateButtonUI("CurseSlot" + std::to_string(i + 1));
				curse[i]->SetEnable(false);
				curse[i]->SetPosition({ -400.0f + 90.0f * i, 500.0f, 0.2f });
				curse[i]->SetWidth(100.0f);
				curse[i]->SetHeight(100.0f);
				curse[i]->SetDefaultTexture(curseIconSlot);
				
				// 걸린 저주를 알려주기 위한 Texture
				GraphicsEngineSpace::TextureUI* curseTexture = canvas->CreateTextureUI("Curse" + std::to_string(i + 1));
				curseTexture->SetParent(curse[i]);
				curseTexture->SetWidth(100.0f);
				curseTexture->SetHeight(100.0f);
				
				// Hover시에 저주의 이름을 출력할 UI
				GraphicsEngineSpace::TextUI* textUI = canvas->CreateTextUI("CurseName" + std::to_string(i + 1));
				textUI->SetParent(curse[i]);
				textUI->SetPosition({ 0.0f, -60.0f, -0.01f });
				textUI->SetFont("H2MJRE");
				textUI->SetFontSize(16.0f);
				textUI->SetEnable(false);

				curse[i]->SetHoverEvent([=]()
					{
						textUI->SetEnable(true);
					});

				curse[i]->SetHoverOutEvent([=]()
					{
						textUI->SetEnable(false);
					});
			}
		}

		void InGameUI::CreateWeaponUI(TextureBase* weaponSlotTexture)
		{
			// Slot
			weaponSlot = canvas->CreateTextureUI("WeaponSlotUI");
			weaponSlot->SetTexture(weaponSlotTexture);
			weaponSlot->SetPosition({ -850.0f, 415.0f, 0.3f });
			weaponSlot->SetWidth(118.0f);
			weaponSlot->SetHeight(125.0f);
		}

		void UI::InGameUI::CreateMemoryFragment(TextureBase* memoryIcon)
		{
			GraphicsEngineSpace::TextureUI* memoryIconUI = canvas->CreateTextureUI("MemoryIconUI");
			memoryIconUI->SetPosition({ 850.0f, 500.0f, 0.2f });
			memoryIconUI->SetWidth(48.0f);
			memoryIconUI->SetHeight(76.0f);
			memoryIconUI->SetTexture(memoryIcon);

			memoryFrags = canvas->CreateTextUI("MemoriesAmount");
			memoryFrags->SetParent(memoryIconUI);
			memoryFrags->SetPivot({ GraphicsEngineSpace::HorizontalLocation::RIGHT, GraphicsEngineSpace::VerticalLocation::MIDDLE });
			memoryFrags->SetPosition({ -40.0f, 00.0f, -0.01f });
			memoryFrags->SetFont("H2MJRE");
			memoryFrags->SetFontSize(25.0f);
		}
	}
}