#include "pch.h"
#include "InventoryUI.h"

#include "GameState.h"
#include "UIManager.h"
#include "InputManager.h"
#include "PlayerStateMachine.h"

namespace Heracles
{
	InventoryUI::InventoryUI(std::weak_ptr<GameObject> obj)
		:ComponentBase(obj, ComponentType::MONOBEHAVIOR)
		, isOpen(false)
		, inventory(nullptr)
		, nowClickedSprite(nullptr)
	{

	}

	// ��� UI�� ���õ� ��������Ʈ ���ҽ����� ������ �� Init ���ش�.
		// �⺻������ UI�Ŵ������� ��� ������ �ְ�
		// inventoryUI�� �ٸ� ������ ��Ȳ�� �°� UI�� �� �ؼ� �����ִ� ���Ҹ�.
	void InventoryUI::Awake()
	{
		inventory = UIManager::GetInstance()->GetCanvas("Inventory");
	}
	void InventoryUI::Start()
	{
	}
	void InventoryUI::Update(float tick)
	{
		// �κ��丮 on / off
		if (InputManager::GetInstance()->GetInputState('I', KeyState::DOWN))
		{
			this->isOpen = !isOpen;
		}

		if (isOpen == true && InputManager::GetInstance()->GetInputState(VK_ESCAPE, KeyState::DOWN))
			this->isOpen = false;

		// �κ��丮 ���¿� ���� onOffInventroy
		OnOffInventory(isOpen);
		ActivateButtons(isOpen);
		if (isOpen == true)
			CheckState();
	}
	void InventoryUI::OnOffInventory(bool c)
	{
		inventory->SetEnable(c);
	}
	void InventoryUI::ActivateButtons(bool b)
	{
		// �⺻������ ����� ������ �� �ֵ��� �Ѵ�.
		inventory->GetButtonUI("InventoryWeaponButton")->SetEnable(b);

		auto gameState = GameState::GetInstance();
		auto player = GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>();

		// ���� ���¿� ����..
		if (gameState->cursePain == true)
			inventory->GetButtonUI("InventoryCursePainButton")->SetEnable(b);
		else
		{
			inventory->GetButtonUI("InventoryCursePainButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryCursePainIcon")->SetEnable(false);
		}

		if (gameState->curseMadness == true)
			inventory->GetButtonUI("InventoryCurseMadnessButton")->SetEnable(b);
		else
		{
			inventory->GetButtonUI("InventoryCurseMadnessButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryCurseMadnessIcon")->SetEnable(false);
		}

		if (gameState->curseSight == true)
			inventory->GetButtonUI("InventoryCurseBlindButton")->SetEnable(b);
		else
		{
			inventory->GetButtonUI("InventoryCurseBlindButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryCurseBlindIcon")->SetEnable(false);
		}

		if (player->maxHp > 100)
		{
			inventory->GetButtonUI("InventoryEnhanceHPButton")->SetEnable(b);
			inventory->GetTextureUI("InventoryEnhanceHPIcon")->SetEnable(true);
		}
		else
		{
			inventory->GetButtonUI("InventoryEnhanceHPButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryEnhanceHPIcon")->SetEnable(false);
		}

		if (player->attackPower > 10)
		{
			inventory->GetButtonUI("InventoryEnhanceAttackButton")->SetEnable(b);
			inventory->GetTextureUI("InventoryEnhanceAttackIcon")->SetEnable(true);

		}
		else
		{
			inventory->GetButtonUI("InventoryEnhanceAttackButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryEnhanceAttackIcon")->SetEnable(false);
		}

		if (player->armor > 0)
		{
			inventory->GetButtonUI("InventoryEnhanceArmorButton")->SetEnable(b);
			inventory->GetTextureUI("InventoryEnhanceArmorIcon")->SetEnable(true);
		}
		else
		{
			inventory->GetButtonUI("InventoryEnhanceArmorButton")->SetEnable(false);
			inventory->GetTextureUI("InventoryEnhanceArmorIcon")->SetEnable(false);
		}
	}

	void InventoryUI::CheckState()
	{

		if(inventory->GetButtonUI("InventoryWeaponButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryWeaponScript");
		}

		if (inventory->GetButtonUI("InventoryCursePainButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryCursePainScript");
		}

		if (inventory->GetButtonUI("InventoryCurseMadnessButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryCurseMadnessScript");
		}

		if (inventory->GetButtonUI("InventoryCurseBlindButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryCurseBlindScript");
		}

		if (inventory->GetButtonUI("InventoryEnhanceHPButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryEnhanceHPScript");
		}

		if (inventory->GetButtonUI("InventoryEnhanceArmorButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryEnhanceArmorScript");
		}

		if (inventory->GetButtonUI("InventoryEnhanceAttackButton")->GetButtonState() == ButtonState::DOWN)
		{
			if (nowClickedSprite != nullptr)
				nowClickedSprite->SetEnable(false);

			nowClickedSprite = inventory->GetTextureUI("InventoryEnhanceAttackScript");
		}
	}
}
