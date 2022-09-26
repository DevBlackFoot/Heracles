#include "pch.h"
#include "DebugComponent.h"

#include "PlayerStateMachine.h"
#include "InventoryUI.h"

#include "InputManager.h"
#include "GameState.h"

namespace Heracles
{
	namespace Debug
	{
		DebugComponent::DebugComponent(std::weak_ptr<GameObject> obj)
			:ComponentBase(obj, ComponentType::CANVAS)
		{
		}
		void DebugComponent::Awake()
		{
			debugCanvas = UIManager::GetInstance()->GetCanvas("DebugCanvas");

		}

		void DebugComponent::Update(float tick)
		{
			if (this->activated)
			{
				this->PrintMousePos();
				this->PrintInventoryState();
				this->PrintPlayerInfo();
			}
			else
			{
				this->DeActivateAll();
			}
		}

		void DebugComponent::LateUpdate(float tick)
		{
			if (InputManager::GetInstance()->GetInputState(VK_EXECUTE, KeyState::DOWN))
			{
				this->activated = false;
			}
		}

		void DebugComponent::PrintMousePos()
		{
			Vector temp = InputManager::GetInstance()->GetMousePos();
			std::string text = "MousePos : ";
			text.append(std::to_string(temp.x));
			text.append(", ");
			text.append(std::to_string(temp.y));
			UIManager::GetInstance()->SetTextUIText("MousePos", text, debugCanvas);
		}

		void DebugComponent::PrintInventoryState()
		{
			std::shared_ptr<InventoryUI> inven = GameState::GetInstance()->GetPlayer()->GetComponent<InventoryUI>();

			if (inven->CheckIsActivated())
			{
				UIManager::GetInstance()->SetTextUIText("Inventory", "Inventory : ON", debugCanvas);
			}
			else
			{
				UIManager::GetInstance()->SetTextUIText("Inventory", "Inventory : OFF", debugCanvas);
			}
		}

		void DebugComponent::PrintPlayerInfo()
		{
			std::shared_ptr<Player::PlayerStateMachine> player = GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>();
			if (player == nullptr)
			{
				return;
			}

			if (player->isInvincable)
			{
				UIManager::GetInstance()->SetTextUIText("IsInvincable", "Invincable : True", debugCanvas);
				UIManager::GetInstance()->SetTextUIColor("IsInvincable", Vector(1.f, 0.f, 0.f), debugCanvas);
			}
			else
			{
				UIManager::GetInstance()->SetTextUIText("IsInvincable", "Invincable : False", debugCanvas);
				UIManager::GetInstance()->SetTextUIColor("IsInvincable", Vector(1.f, 1.f, 1.f), debugCanvas);
			}

			std::string hp = "MaxHP : ";
			hp.append(std::to_string(player->maxHp));
			UIManager::GetInstance()->SetTextUIText("HP", hp, debugCanvas);

			std::string AttackPower = "AttackPower : ";
			AttackPower.append(std::to_string(player->attackPower));
			UIManager::GetInstance()->SetTextUIText("AttackPower", AttackPower, debugCanvas);

			std::string rotateSpeed = "RotateSpeed : ";
			rotateSpeed.append(std::to_string(player->rotateSpeed));
			UIManager::GetInstance()->SetTextUIText("RotateSpeed", rotateSpeed, debugCanvas);

			std::string CursePain = "CursePain : ON";
			if (GameState::GetInstance()->cursePain == false)
			{
				CursePain = "CursePain : OFF";
			}
			UIManager::GetInstance()->SetTextUIText("CursePain", CursePain, debugCanvas);

			std::string CurseMadness = "CurseMadness : ON";
			if (GameState::GetInstance()->curseMadness == false) { CurseMadness = "CurseMadness : OFF"; }
			UIManager::GetInstance()->SetTextUIText("CurseMadness", CurseMadness, debugCanvas);

			std::string CurseSight = "CurseSight : ON";
			if (GameState::GetInstance()->curseSight == false) { CurseSight = "CurseSight : OFF"; }
			UIManager::GetInstance()->SetTextUIText("CurseSight", CurseSight, debugCanvas);
		}

		void DebugComponent::DeActivateAll()
		{
			UIManager::GetInstance()->SetTextUIIsEnable("MousePos", false, debugCanvas);
		}





	}
}