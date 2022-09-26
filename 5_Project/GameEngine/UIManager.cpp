#include "pch.h"
#include "UIManager.h"

#include "GraphicsManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "WindowManager.h"
#include "SoundManager.h"
#include "Timer.h"

namespace GameEngineSpace
{
	std::shared_ptr<UIManager> UIManager::instance = nullptr;

	std::shared_ptr<UIManager> UIManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<UIManager>();

		return instance;
	}

	void UIManager::Init()
	{
		// �׷��Ƚ� �Ŵ��� ĳ��
		graphicsInstance = GraphicsManager::GetInstance();
		inputInstance = InputManager::GetInstance();
		soundInstance = SoundManager::GetInstance();

		// ĵ���� ����i
		auto renderer = graphicsInstance->GetRenderer();
		auto width = WindowManager::GetInstance()->GetClientWidth();
		auto height = WindowManager::GetInstance()->GetClientHeight();
		nowWidth = width;
		nowHeight = height;

		mainCanvas = renderer->CreateFactory()->CreateCanvas("Canvas", width, height);

		/// ���ӿ��� ����� ��� ĵ�������� ���
		// ���� ��ġ ���� ���� - CurseAltar
		mainCanvas->CreateCanvas("CurseAltar");
		// ���� ��ġ ���� �ʿ� - EnhanceAltar
		mainCanvas->CreateCanvas("EnhanceAltar");
		// ���� ���� - WeaponAltar
		mainCanvas->CreateCanvas("WeaponAltar");
		// �κ��丮 - Inventory
		mainCanvas->CreateCanvas("Inventory");
		// ����� ĵ���� -> DebugCanvas;
		mainCanvas->CreateCanvas("DebugCanvas");

		// ���α� ĵ����
		mainCanvas->CreateCanvas("MonologueCanvas");
		// ��ȭâ ĵ���� -> DialogCanvas;
		mainCanvas->CreateCanvas("DialogCanvas");

		// Ʃ�丮�� ���� ��Ƶ� Ʃ�丮�� ĵ����
		mainCanvas->CreateCanvas("TutorialCanvas");

		// ���� ��
		mainCanvas->CreateCanvas("EndingCanvas");
	}

	void UIManager::CreateCanvasAll()
	{

		CreateInventory();
		CreateCurseAltar();
		CreateEnhanceAltar();
		CreateWeaponAltar();
		CreateDebugCanvas();
		//CreateBossCanvas();
		CreateMonologueCanvas();
		CreateDialogCanvas();
		CreateTutorialCanvas();
		CreateEndingCanvas();
	}

	void UIManager::Render()
	{

		// ĵ������ ����
		mainCanvas->Render(graphicsInstance->GetRenderer().get(), Timer::GetInstance()->DeltaTime());
	}

	// �ش� �Լ��� Input ������Ʈ ���Ŀ� �־���Ѵ�.
	void UIManager::Update()
	{
		auto mousePos = inputInstance->GetMousePos();

		ButtonUI* collidedButton = mainCanvas->CollidedButtonWithMouse(mousePos.x, mousePos.y, inputInstance->GetInputState(VK_LBUTTON, KeyState::STAY));

	}

	void UIManager::LateUpdate()
	{
	}

	void UIManager::Release()
	{
		mainCanvas->Release();
	}

	Canvas* UIManager::CreateCanvas(const std::string& UIIDstr)
	{
		Canvas* newCanvas = mainCanvas->CreateCanvas(UIIDstr);
		newCanvas->SetScale({ mainCanvas->GetWidth() / static_cast<float>(nowWidth), mainCanvas->GetHeight() / static_cast<float>(nowHeight) });

		return newCanvas;
	}

	void UIManager::CreateUIText(const std::string& UIIDstr, const std::string& text, Vector color, Vector position, float rotation,
		Vector scale, Canvas* parentCanvas)
	{
		TextUI* tempText;
		if (parentCanvas == nullptr)
			tempText = mainCanvas->CreateTextUI(UIIDstr);
		else
			tempText = parentCanvas->CreateTextUI(UIIDstr);

		tempText->SetText(text);
		tempText->SetColor(color);
		tempText->SetPosition(position);
		tempText->SetRotation(VectorReplicate(rotation));
		tempText->SetScale(scale);
	}

	void UIManager::SetTextUIText(const std::string& UIIDstr, const std::string& text, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetText(text);
	}

	void UIManager::SetTextUIColor(const std::string& UIIDstr, Vector color, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetColor(color);
	}

	void UIManager::SetTextUIPosition(const std::string& UIIDstr, Vector position, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetPosition(position);
	}

	void UIManager::SetTextUIScale(const std::string& UIIDstr, Vector scale, Canvas* parentCanvas)
	{

		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetScale(scale);
	}

	void UIManager::SetTextUIRotation(const std::string& UIIDstr, float rotation, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetRotation(VectorReplicate(rotation));
	}

	void UIManager::SetTextUIIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		TextUI* result;
		if (parentCanvas == nullptr)
			result = mainCanvas->GetTextUI(UIIDstr);
		else
			result = parentCanvas->GetTextUI(UIIDstr);

		if (result != nullptr)
			result->SetEnable(isEnable);
	}

	void UIManager::SetTextUIAnchor(const std::string& UIIDstr, VerticalLocation vertical,
		HorizontalLocation horizontal, Canvas* parentCanvas)
	{
	}

	void UIManager::CreateUISprite(const std::string& UIIDstr, const std::string& textureName,
		const std::string& texturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		TextureUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateTextureUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateTextureUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		newTextureUI->SetTexture(texture);
		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	void UIManager::CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		ButtonUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateButtonUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateButtonUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(noneTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(noneTextureName, noneTexturePath);

		newTextureUI->SetDefaultTexture(texture);

		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	void UIManager::CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName,
		const std::string& noneTexturePath, const std::string& hoverTextureName, const std::string& hoverTexturePath,
		const std::string& downTextureName, const std::string& downTexturePath, long posX, long posY, long width,
		long height, float z, Canvas* parentCanvas)
	{

		ButtonUI* newTextureUI;

		if (parentCanvas == nullptr)
			newTextureUI = mainCanvas->CreateButtonUI(UIIDstr);
		else
			newTextureUI = parentCanvas->CreateButtonUI(UIIDstr);

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(noneTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(noneTextureName, noneTexturePath);

		newTextureUI->SetDefaultTexture(texture);

		// Hover Texture
		texture = resourceManager->GetTexture(hoverTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(hoverTextureName, hoverTexturePath);

		newTextureUI->SetHoverTexture(texture);

		// Press Texture
		texture = resourceManager->GetTexture(downTextureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(downTextureName, downTexturePath);

		newTextureUI->SetPressTexture(texture);

		newTextureUI->SetPosition(Vector{ static_cast<float>(posX), static_cast<float>(posY), z });
		newTextureUI->SetWidth(width);
		newTextureUI->SetHeight(height);
	}

	UIBase* UIManager::CreateProgressUI(const std::string& UIIDstr, const std::string& frontTextureName, const std::string& fontTexturePath, RECT frontSrc, const std::string& backTextureName, const std::string& backTexturePath, RECT backSrc, long posX, long posY, long width, long height, float z, Canvas* parentCanvas)
	{
		return nullptr;
	}

	void UIManager::SetSpriteTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{
		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetTexture(texture);
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetTexture(texture);
	}

	void UIManager::SetSpritePosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void UIManager::SetSpriteSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
		{
			mainCanvas->GetTextureUI(UIIDstr)->SetWidth(width);
			mainCanvas->GetTextureUI(UIIDstr)->SetHeight(height);
		}
		else
		{
			parentCanvas->GetTextureUI(UIIDstr)->SetWidth(width);
			parentCanvas->GetTextureUI(UIIDstr)->SetHeight(height);
		}
	}

	void GameEngineSpace::UIManager::SetSpriteIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetTextureUI(UIIDstr)->SetEnable(isEnable);
		else
			parentCanvas->GetTextureUI(UIIDstr)->SetEnable(isEnable);
	}

	void UIManager::SetSpriteNoneTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{
		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetDefaultTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetDefaultTexture(texture);
	}

	void UIManager::SetSpriteHoverTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetHoverTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetHoverTexture(texture);
	}

	void UIManager::SetSpriteDownTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas)
	{

		auto* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// Default Texture
		auto* texture = resourceManager->GetTexture(textureName);

		if (texture == nullptr)
			texture = graphicsInstance->GetRenderer()->CreateFactory()->CreateTexture(textureName, texturePath);

		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetPressTexture(texture);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetPressTexture(texture);
	}

	void UIManager::SetButtonIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetEnable(isEnable);
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetEnable(isEnable);
	}

	void UIManager::SetButtonPosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
			mainCanvas->GetButtonUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
		else
			parentCanvas->GetButtonUI(UIIDstr)->SetPosition({ static_cast<float>(posX), static_cast<float>(posY) });
	}

	void UIManager::SetButtonSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas)
	{
		if (parentCanvas == nullptr)
		{
			mainCanvas->GetButtonUI(UIIDstr)->SetWidth(width);
			mainCanvas->GetButtonUI(UIIDstr)->SetHeight(height);
		}
		else
		{
			parentCanvas->GetButtonUI(UIIDstr)->SetWidth(width);
			parentCanvas->GetButtonUI(UIIDstr)->SetHeight(height);
		}
	}


	ButtonState UIManager::GetButtonState(const std::string& UIIDstr, Canvas* parentCanvas)
	{
		ButtonUI* result;

		if (parentCanvas == nullptr)
			result = mainCanvas->GetButtonUI(UIIDstr);
		else
			result = parentCanvas->GetButtonUI(UIIDstr);

		if (result == nullptr)
			return ButtonState::DEFAULT;

		return result->GetButtonState();
	}

	void UIManager::OnResize(uint32 width, uint32 height)
	{
		mainCanvas->SetWidth(width);
		mainCanvas->SetHeight(height);

		// ���� ���
		float xScale = static_cast<float>(width) / nowWidth;
		float yScale = static_cast<float>(height) / nowHeight;

		// ��� ĵ������ �������� �����մϴ�.
		//mainCanvas->GetCanvas("StartAltar")->SetScale({xScale, yScale});
		mainCanvas->SetScaleAllCanvas({ xScale, yScale });
	}

	Canvas* UIManager::GetCanvas(const std::string& canvasIDStr)
	{
		return mainCanvas->GetCanvas(canvasIDStr);
	}

	void UIManager::ClearUI()
	{
		//mainCanvas->Release();
	}

	void UIManager::CreateInventory()
	{
		Canvas* inventory = mainCanvas->GetCanvas("Inventory");

		// ���
		CreateUISprite("InventoryWindow", "InventoryBG", "Resources/UI/BG.png"
			, -50, 0, 2153, 1199, 0.5f, inventory);

		// ���������� �����ֵ��� ����
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// ���� ����
		// ����
		CreateUISprite("InventoryWeaponSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, -230, 130, 176, 0.3f, inventory);

		TextureUI* slot = inventory->GetTextureUI("InventoryWeaponSlot");
		TextureUI* icon = inventory->CreateTextureUI("InventoryWeaponIcon");

		// �� �ؽ���
		TextureBase* weapon = resourceManager->GetTexture("ClubIcon");
		if (weapon == nullptr)
			weapon = factory->CreateTexture("ClubIcon", "Resources/UI/Weapon_500_500_2D_Image.png");

		icon->SetParent(slot);
		icon->SetTexture(weapon);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(80.f);

		CreateUIButton("InventoryWeaponButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-513, -235, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryWeaponButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryWeaponButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryWeaponScript")->SetEnable(true);
			});

		// ����

		// ����
		CreateUISprite("InventoryCursePainSlot", "InventorySlot", "Resources/UI/slot.png"
			, -620, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCursePainSlot");
		icon = inventory->CreateTextureUI("InventoryCursePainIcon");

		// �� �ؽ���
		TextureBase* curse = resourceManager->GetTexture("Pain");
		if (curse == nullptr)
			curse = factory->CreateTexture("Pain", "Resources/UI/CurseIcon_Pain.png");

		icon->SetParent(slot);
		icon->SetTexture(curse);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(80.f);

		CreateUIButton("InventoryCursePainButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-623, -35, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryCursePainButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryCursePainButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCursePainScript")->SetEnable(true);
			});

		// ����
		CreateUISprite("InventoryCurseMadnessSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseMadnessSlot");
		icon = inventory->CreateTextureUI("InventoryCurseMadnessIcon");

		// �� �ؽ���
		curse = resourceManager->GetTexture("Madness");
		if (curse == nullptr)
			curse = factory->CreateTexture("Madness", "Resources/UI/CurseIcon_Madness.png");

		icon->SetParent(slot);
		icon->SetTexture(curse);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(80.f);

		CreateUIButton("InventoryCurseMadnessButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-513, -35, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryCurseMadnessButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryCurseMadnessButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCurseMadnessScript")->SetEnable(true);
			});

		// �þ�
		CreateUISprite("InventoryCurseBlindSlot", "InventorySlot", "Resources/UI/slot.png"
			, -400, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseBlindSlot");
		icon = inventory->CreateTextureUI("InventoryCurseBlindIcon");

		// �� �ؽ���
		curse = resourceManager->GetTexture("Blind");
		if (curse == nullptr)
			curse = factory->CreateTexture("Blind", "Resources/UI/CurseIcon_Blind.png");

		icon->SetParent(slot);
		icon->SetTexture(curse);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(80.f);

		CreateUIButton("InventoryCurseBlindButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-403, -35, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryCurseBlindButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryCurseBlindButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCurseBlindScript")->SetEnable(true);
			});

		// �ʿ�

		// HP
		CreateUISprite("InventoryEnhanceHPSlot", "InventorySlot", "Resources/UI/slot.png"
			, -620, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceHPSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceHPIcon");

		// �� �ؽ���
		TextureBase* enhance = resourceManager->GetTexture("MaxHPUp");
		if (enhance == nullptr)
			enhance = factory->CreateTexture("MaxHPUp", "Resources/UI/patience_icon.png");

		icon->SetParent(slot);
		icon->SetTexture(enhance);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(75.f);

		CreateUIButton("InventoryEnhanceHPButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-623, 165, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryEnhanceHPButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryEnhanceHPButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceHPScript")->SetEnable(true);
			});

		// ����
		CreateUISprite("InventoryEnhanceArmorSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceArmorSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceArmorIcon");

		// �� �ؽ���
		enhance = resourceManager->GetTexture("ArmorUp");
		if (enhance == nullptr)
			enhance = factory->CreateTexture("ArmorUp", "Resources/UI/steelskin_icon.png");

		icon->SetParent(slot);
		icon->SetTexture(enhance);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(75.f);

		CreateUIButton("InventoryEnhanceArmorButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-513, 165, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryEnhanceArmorButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryEnhanceArmorButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceArmorScript")->SetEnable(true);
			});

		// ���ݾ�
		CreateUISprite("InventoryEnhanceAttackSlot", "InventorySlot", "Resources/UI/slot.png"
			, -400, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceAttackSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceAttackIcon");

		// �� �ؽ���
		enhance = resourceManager->GetTexture("AttackUp");
		if (enhance == nullptr)
			enhance = factory->CreateTexture("AttackUp", "Resources/UI/superpower_icon.png");

		icon->SetParent(slot);
		icon->SetTexture(enhance);
		icon->SetPosition({ 0.f, 0.f, -0.01f });
		icon->SetHeight(100.f);
		icon->SetWidth(75.f);

		CreateUIButton("InventoryEnhanceAttackButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-403, 165, 110, 210, 0.25f, inventory);
		inventory->GetButtonUI("InventoryCurseBlindButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		inventory->GetButtonUI("InventoryEnhanceAttackButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");

				// �ش�Ǵ� ���� �̹��� ���
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceAttackScript")->SetEnable(true);
			});

		// ���� �̹��� ��������Ʈ.
		// ����..
		CreateUISprite("InventoryWeaponScript", "InventoryWeaponScript", "Resources/UI/weapon.png",
			150, 50, 900, 460, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryWeaponScript");
		TextureUI* weaponScriptIcon = inventory->CreateTextureUI("InventoryWeaponScriptIcon");
		weapon = resourceManager->GetTexture("ClubIcon");
		weaponScriptIcon->SetParent(slot);
		weaponScriptIcon->SetTexture(weapon);
		weaponScriptIcon->SetPosition({ -320.f, -105.f, -0.01f });
		weaponScriptIcon->SetHeight(200.f);
		weaponScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryWeaponScript")->SetEnable(false);

		// ����
		CreateUISprite("InventoryCursePainScript", "InventoryCursePainScript", "Resources/UI/curse_pain.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryCursePainScript");
		TextureUI* curseScriptIcon = inventory->CreateTextureUI("InventoryCursePainScriptIcon");
		curse = resourceManager->GetTexture("Pain");
		curseScriptIcon->SetParent(slot);
		curseScriptIcon->SetTexture(curse);
		curseScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		curseScriptIcon->SetHeight(200.f);
		curseScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryCursePainScript")->SetEnable(false);

		// ����
		CreateUISprite("InventoryCurseMadnessScript", "InventoryCurseMadnessScript", "Resources/UI/curse_crazy.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseMadnessScript");
		curseScriptIcon = inventory->CreateTextureUI("InventoryCurseMadnessScriptIcon");
		curse = resourceManager->GetTexture("Madness");
		curseScriptIcon->SetParent(slot);
		curseScriptIcon->SetTexture(curse);
		curseScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		curseScriptIcon->SetHeight(200.f);
		curseScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryCurseMadnessScript")->SetEnable(false);

		// �þ�
		CreateUISprite("InventoryCurseBlindScript", "InventoryCurseBlindScript", "Resources/UI/curse_view.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseBlindScript");
		curseScriptIcon = inventory->CreateTextureUI("InventoryCurseBlindScriptIcon");
		curse = resourceManager->GetTexture("Blind");
		curseScriptIcon->SetParent(slot);
		curseScriptIcon->SetTexture(curse);
		curseScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		curseScriptIcon->SetHeight(200.f);
		curseScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryCurseBlindScript")->SetEnable(false);

		// hp
		CreateUISprite("InventoryEnhanceHPScript", "InventoryEnhanceHPScript", "Resources/UI/Ability_patience.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceHPScript");
		TextureUI* enhanceScriptIcon = inventory->CreateTextureUI("InventoryEnhanceHPScriptIcon");
		enhance = resourceManager->GetTexture("MaxHPUp");
		enhanceScriptIcon->SetParent(slot);
		enhanceScriptIcon->SetTexture(enhance);
		enhanceScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		enhanceScriptIcon->SetHeight(200.f);
		enhanceScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryEnhanceHPScript")->SetEnable(false);

		// ���
		CreateUISprite("InventoryEnhanceArmorScript", "InventoryEnhanceArmorScript", "Resources/UI/Ability_steelskin.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceArmorScript");
		enhanceScriptIcon = inventory->CreateTextureUI("InventoryEnhanceArmorScriptIcon");
		enhance = resourceManager->GetTexture("ArmorUp");
		enhanceScriptIcon->SetParent(slot);
		enhanceScriptIcon->SetTexture(enhance);
		enhanceScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		enhanceScriptIcon->SetHeight(200.f);
		enhanceScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryEnhanceArmorScript")->SetEnable(false);

		// ����
		CreateUISprite("InventoryEnhanceAttackScript", "InventoryEnhanceAttackScript", "Resources/UI/Ability_superpower.png",
			150, 0, 900, 450, 0.2f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceAttackScript");
		enhanceScriptIcon = inventory->CreateTextureUI("InventoryEnhanceAttackScriptIcon");
		enhance = resourceManager->GetTexture("AttackUp");
		enhanceScriptIcon->SetParent(slot);
		enhanceScriptIcon->SetTexture(enhance);
		enhanceScriptIcon->SetPosition({ -275.f, 0.f, -0.01f });
		enhanceScriptIcon->SetHeight(200.f);
		enhanceScriptIcon->SetWidth(160.f);
		inventory->GetTextureUI("InventoryEnhanceAttackScript")->SetEnable(false);

		inventory->SetEnable(false);
	}

	void UIManager::CreateCurseAltar()
	{
		Canvas* curseAltarCanvas = mainCanvas->GetCanvas("CurseAltar");

		/// ��� => ���Ŀ� ������ ����.
		CreateUISprite("CurseAltarWindow", "StartAltarWindow", "Resources/UI/altar_ui_BG.png",
			-50, 0, 2153, 1199, 0.5f, curseAltarCanvas);

#pragma region Title
		/// Ÿ��Ʋ
		// �ؽ�Ʈ ����
		TextUI* title = curseAltarCanvas->CreateTextUI("CurseAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"���� �ູ");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// SubTitle
			// ���� �ع�, �ɷ� ���׷��̵�.
		TextUI* subTitle = curseAltarCanvas->CreateTextUI("CurseAltarSubTitleCurse");
		subTitle->SetPosition({ -155.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"���� �ع�");
		subTitle->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });

		// ���� ��ư �߰�
		CreateUIButton("CurseAltarCurseSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-142, -315, 280, 80, 0.38f, curseAltarCanvas);
		// hover�̺�Ʈ �߰�.
		curseAltarCanvas->GetButtonUI("CurseAltarCurseSelectButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		curseAltarCanvas->GetButtonUI("CurseAltarCurseSelectButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");
			});

		TextUI* subTitle2 = curseAltarCanvas->CreateTextUI("CurseAltarSubTitleEnhance");
		subTitle2->SetPosition({ 135.0f, -320.0f, 0.4f });
		subTitle2->SetFont("MuseumMedium");
		subTitle2->SetFontSize(20.0f);
		subTitle2->SetText(L"�ʿ�");
		subTitle2->SetColor({ 0.2196f, 0.2510f, 0.2784f, 1.0f });

		// ���� ��ư �߰�
		CreateUIButton("CurseAltarEnhanceSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			145, -315, 280, 80, 0.38f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CurseAltarEnhanceSelectButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		curseAltarCanvas->GetButtonUI("CurseAltarEnhanceSelectButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("UIOpen");
			});
#pragma endregion

		// ���ҽ� �Ŵ����� ���丮�� �޾ƿ´�.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		/// ���� ����
#pragma region CurseOfPain
		// ������ ��� => �ִ� HP ��
		CreateUISprite("CursePainBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, curseAltarCanvas);
		// �� ����
		TextureUI* cursePainBG = curseAltarCanvas->GetTextureUI("CursePainBG");
		cursePainBG->SetPosition({ -220.f, -180.f, 0.3f });

		// ������
		TextureBase* curseIcon = resourceManager->GetTexture("Pain");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Pain", "Resources/UI/CurseIcon_Pain.png");

		TextureUI* PainTex = curseAltarCanvas->CreateTextureUI("CursePain");

		PainTex->SetParent(cursePainBG);
		PainTex->SetPosition({ 5.f, 0.f, -0.01f });
		PainTex->SetHeight(100.f);
		PainTex->SetWidth(100.f);
		PainTex->SetTexture(curseIcon);

		// ��ư
		CreateUIButton("CursePainButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CursePainButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* PainText = curseAltarCanvas->CreateTextUI("CursePainText");
		PainText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		PainText->SetPosition({ 50.f, -220.f, 0.2f });
		PainText->SetFont("MuseumMedium");
		PainText->SetFontSize(20.0f);
		PainText->SetText(L"�İ��� ����");
		PainText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* PainSubText = curseAltarCanvas->CreateTextUI("CursePainSubText");
		PainSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		PainSubText->SetPosition({ 170.f, -170.f, 0.2f });
		PainSubText->SetFont("MuseumMedium");
		PainSubText->SetFontSize(14.0f);
		PainSubText->SetText(L" �ؾ��� ��������\n\n\n\n ������ 10% �����մϴ�.");
		PainSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		TextUI* gemCount = curseAltarCanvas->CreateTextUI("CursePainGemCount");
		gemCount->SetPosition({ 270.f, -245.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("1");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("CursePainGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, -245, 22, 38, 0.2f, curseAltarCanvas);
#pragma endregion

#pragma region CurseOfMadness
		// ������ ��� => ���� ��
		CreateUISprite("CurseMadnessBG", "AltarSlot", "Resources/UI/altar_iconbox.png"
			, -40, 30, 150, 230, 0.3f, curseAltarCanvas);
		// �� ����
		TextureUI* curseMadnessBG = curseAltarCanvas->GetTextureUI("CurseMadnessBG");
		curseMadnessBG->SetPosition({ -220.f, 10.f, 0.3f });

		// ������
		curseIcon = resourceManager->GetTexture("Madness");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Madness", "Resources/UI/CurseIcon_Madness.png");

		TextureUI* madnessTex = curseAltarCanvas->CreateTextureUI("CurseMadness");

		madnessTex->SetParent(curseMadnessBG);
		madnessTex->SetPosition({ 5.f, 0.f, -0.01f });
		madnessTex->SetHeight(100.f);
		madnessTex->SetWidth(100.f);
		madnessTex->SetTexture(curseIcon);

		// ��ư
		CreateUIButton("CurseMadnessButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 10, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CurseMadnessButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* madnessText = curseAltarCanvas->CreateTextUI("CurseMadnessText");
		madnessText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		madnessText->SetPosition({ 50.f, -30.f, 0.2f });
		madnessText->SetFont("MuseumMedium");
		madnessText->SetFontSize(20.0f);
		madnessText->SetText(L"��������� ����");
		madnessText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* madnessSubText = curseAltarCanvas->CreateTextUI("CurseMadnessSubText");
		madnessSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		madnessSubText->SetPosition({ 170.f, 20.f, 0.2f });
		madnessSubText->SetFont("MuseumMedium");
		madnessSubText->SetFontSize(14.0f);
		madnessSubText->SetText(L" ���⿡ ���� ���� �̵��ӵ���\n\n\n\n 10% �����մϴ�.");
		madnessSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		gemCount = curseAltarCanvas->CreateTextUI("CurseMadnessGemCount");
		gemCount->SetPosition({ 270.f, -55.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("1");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("CurseMadnessGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, -55, 22, 38, 0.2f, curseAltarCanvas);
#pragma endregion

#pragma region CurseOfBlind
		// ������ ��� => ���ݷ� ��
		CreateUISprite("CurseBlindBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, curseAltarCanvas);
		// �� ����
		TextureUI* curseBlindBG = curseAltarCanvas->GetTextureUI("CurseBlindBG");
		curseBlindBG->SetPosition({ -220.f, 200.f, 0.3f });


		// ������
		curseIcon = resourceManager->GetTexture("Blind");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Blind", "Resources/UI/CurseIcon_Blind.png");

		TextureUI* blindTex = curseAltarCanvas->CreateTextureUI("CurseBlind");

		blindTex->SetParent(curseBlindBG);
		blindTex->SetPosition({ 5.f, 0.f, -0.01f });
		blindTex->SetHeight(100.f);
		blindTex->SetWidth(100.f);
		blindTex->SetTexture(curseIcon);

		// ��ư
		CreateUIButton("CurseBlindButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 200, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CurseBlindButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* blindText = curseAltarCanvas->CreateTextUI("CurseBlindText");
		blindText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		blindText->SetPosition({ 50.f, 160.f, 0.2f });
		blindText->SetFont("MuseumMedium");
		blindText->SetFontSize(20.0f);
		blindText->SetText(L"������ �ʴ� �þ�");
		blindText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* blindSubText = curseAltarCanvas->CreateTextUI("CurseBlindSubText");
		blindSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		blindSubText->SetPosition({ 170.f, 210.f, 0.2f });
		blindSubText->SetFont("MuseumMedium");
		blindSubText->SetFontSize(14.0f);
		blindSubText->SetText(L" �ð��� �������� ����\n\n\n\n �� �� �ִ� �þ߰� ���ѵ˴ϴ�.");
		blindSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		gemCount = curseAltarCanvas->CreateTextUI("CurseBlindGemCount");
		gemCount->SetPosition({ 270.f, 135.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("1");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("CurseBlindGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, 135, 22, 38, 0.2f, curseAltarCanvas);
#pragma endregion

#pragma region GemLack
		CreateUISprite("CurseNotice", "Notice", "Resources/UI/OnMouse_Title2.png",
			0, 320, 700, 200, 0.19f, curseAltarCanvas);
		TextureUI* notice = curseAltarCanvas->GetTextureUI("CurseNotice");

		TextUI* noticeText = curseAltarCanvas->CreateTextUI("CurseNoticeText");
		noticeText->SetParent(notice);
		noticeText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		noticeText->SetPosition({ -20.f, -15.f, -0.01f });
		noticeText->SetFont("MuseumMedium");
		noticeText->SetFontSize(35.f);
		noticeText->SetText(L"��ȭ�� �����մϴ�!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("CurseNotice", false, curseAltarCanvas);

#pragma endregion

#pragma region CloseButton
		/// ���� ��ư
		CreateUISprite("StartAltarQuit", "QuitButton", "Resources/UI/quit_button.png",
			0, 450, 313, 58, 0.18f, curseAltarCanvas);

		CreateUIButton("StartAltarClose", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"CloseButtonHover", "Resources/UI/OnMouse_Title2.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			0, 450, 313, 58, 0.17f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("StartAltarClose")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		curseAltarCanvas->GetButtonUI("StartAltarClose")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("AltarClose");
			});

		/*ButtonUI* buttonBG = curseAltarCanvas->GetButtonUI("StartAltarClose");
		TextureUI* closeButton = curseAltarCanvas->CreateTextureUI("ButtonX");

		TextureBase* buttonX = resourceManager->GetTexture("ButtonX");
		if (buttonX == nullptr)
			buttonX = factory->CreateTexture("ButtonX", "Resources/UI/Button1_X.png");

		closeButton->SetParent(buttonBG);
		closeButton->SetPosition({ 0.f, 0.f, -0.01f });
		closeButton->SetHeight(75.f);
		closeButton->SetWidth(75.f);
		closeButton->SetTexture(buttonX);*/
#pragma endregion

#pragma region PreTextureLoad
		// ���Ŀ� �� �ؽ��� �̸� �ε�
		TextureBase* preTex = resourceManager->GetTexture("FreePain");
		if (preTex == nullptr)
			factory->CreateTexture("FreePain", "Resources/UI/steelskin_icon.png");
		preTex = resourceManager->GetTexture("FreeMadness");
		if (preTex == nullptr)
			factory->CreateTexture("FreeMadness", "Resources/UI/free_madness.png");
		preTex = resourceManager->GetTexture("FreeBlind");
		if (preTex == nullptr)
			factory->CreateTexture("FreeBlind", "Resources/UI/free_blind.png");
#pragma endregion

		curseAltarCanvas->SetEnable(false);
	}

	void UIManager::CreateEnhanceAltar()
	{
		// ���� ĵ������ StartAltarĵ������ ������ݴϴ�.
		Canvas* enhanceAltarCanvas = mainCanvas->GetCanvas("EnhanceAltar");

		/// ��� => ���Ŀ� ������ ����.
		CreateUISprite("EnhanceAltarWindow", "StartAltarWindow", "Resources/UI/altar_ui_BG.png",
			-50, 0, 2153, 1199, 0.5f, enhanceAltarCanvas);

#pragma region Title
		/// Ÿ��Ʋ
		// �ؽ�Ʈ ����
		TextUI* title = enhanceAltarCanvas->CreateTextUI("EnhanceAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"���� �ູ");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// SubTitle
			// ���� �ع�, �ɷ� ���׷��̵�.
		TextUI* subTitle = enhanceAltarCanvas->CreateTextUI("EnhanceAltarSubTitleCurse");
		subTitle->SetPosition({ -155.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"���� �ع�");
		subTitle->SetColor({ 0.2196f, 0.2510f, 0.2784f, 1.0f });

		// ���� ��ư �߰�
		CreateUIButton("EnhanceAltarCurseSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-142, -315, 280, 80, 0.38f, enhanceAltarCanvas);

		TextUI* subTitle2 = enhanceAltarCanvas->CreateTextUI("EnhanceAltarSubTitleEnhance");
		subTitle2->SetPosition({ 135.0f, -320.0f, 0.4f });
		subTitle2->SetFont("MuseumMedium");
		subTitle2->SetFontSize(20.0f);
		subTitle2->SetText(L"�ʿ�");
		subTitle2->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });

		// ���� ��ư �߰�
		CreateUIButton("EnhanceAltarEnhanceSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			145, -315, 280, 80, 0.38f, enhanceAltarCanvas);
#pragma endregion

		// ���ҽ� �Ŵ����� ���丮�� �޾ƿ´�.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		/// ��ȭ ��ư
#pragma region HPButton
		// ������ ��� => �ִ� HP ��
		CreateUISprite("EnhanceHPBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, enhanceAltarCanvas);
		// �� ����
		TextureUI* enhanceHPBG = enhanceAltarCanvas->GetTextureUI("EnhanceHPBG");
		enhanceHPBG->SetPosition({ -220.f, -180.f, 0.3f });

		// ������
		TextureBase* enhanceIcon = resourceManager->GetTexture("MaxHPUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("MaxHPUp", "Resources/UI/patience_icon.png");

		TextureUI* HPTex = enhanceAltarCanvas->CreateTextureUI("EnhanceHP");

		HPTex->SetParent(enhanceHPBG);
		HPTex->SetPosition({ 5.f, 0.f, -0.01f });
		HPTex->SetHeight(100.f);
		HPTex->SetWidth(85.f);
		HPTex->SetTexture(enhanceIcon);

		// ��ư
		CreateUIButton("EnhanceHPButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceHPButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* HPText = enhanceAltarCanvas->CreateTextUI("EnhanceHPText");
		HPText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		HPText->SetPosition({ 50.f, -230.f, 0.2f });
		HPText->SetFont("MuseumMedium");
		HPText->SetFontSize(20.0f);
		HPText->SetText(L"�γ�");
		HPText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* HPSubText = enhanceAltarCanvas->CreateTextUI("EnhanceHPSubText");
		HPSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		HPSubText->SetPosition({ 270.f, -180.f, 0.2f });
		HPSubText->SetFont("MuseumMedium");
		HPSubText->SetFontSize(14.0f);
		HPSubText->SetText(L" ������ �һ��� ���Ӿ��� ��������\n\n\n\n �ʿ����� �γ��� ���� ����ϴ�.\n\n\n\n �ִ� HP�� 20 �����մϴ�.");
		HPSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		TextUI* gemCount = enhanceAltarCanvas->CreateTextUI("EnhanceHPGemCount");
		gemCount->SetPosition({ 270.f, -245.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("2");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("EnhanceHPGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, -245, 22, 38, 0.2f, enhanceAltarCanvas);
#pragma endregion

#pragma region Armor UP
		// ������ ��� => ���� ��
		CreateUISprite("EnhanceArmorBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, enhanceAltarCanvas);
		// �� ����
		TextureUI* enhanceArmorBG = enhanceAltarCanvas->GetTextureUI("EnhanceArmorBG");
		enhanceArmorBG->SetPosition({ -220.f, 10.f, 0.3f });

		// ������
		enhanceIcon = resourceManager->GetTexture("ArmorUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("ArmorUp", "Resources/UI/steelskin_icon.png");

		TextureUI* armorTex = enhanceAltarCanvas->CreateTextureUI("EnhanceArmor");

		armorTex->SetParent(enhanceArmorBG);
		armorTex->SetPosition({ 5.f, 0.f, -0.01f });
		armorTex->SetHeight(100.f);
		armorTex->SetWidth(85.f);
		armorTex->SetTexture(enhanceIcon);

		// ��ư
		CreateUIButton("EnhanceArmorButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 10, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceArmorButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* armorText = enhanceAltarCanvas->CreateTextUI("EnhanceArmorText");
		armorText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		armorText->SetPosition({ 50.f, -40.f, 0.2f });
		armorText->SetFont("MuseumMedium");
		armorText->SetFontSize(20.0f);
		armorText->SetText(L"��ö �Ǻ�");
		armorText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* armorSubText = enhanceAltarCanvas->CreateTextUI("EnhanceArmorSubText");
		armorSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		armorSubText->SetPosition({ 270.f, 10.f, 0.2f });
		armorSubText->SetFont("MuseumMedium");
		armorSubText->SetFontSize(14.0f);
		armorSubText->SetText(L" ���� ���ݿ��� ���ظ� ���� �ʴ�\n\n\n\n ���� ��ö �Ǻθ� ��� �˴ϴ�.\n\n\n\n ������ 10% �����մϴ�.");
		armorSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		gemCount = enhanceAltarCanvas->CreateTextUI("EnhanceArmorGemCount");
		gemCount->SetPosition({ 270.f, -55.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("2");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("EnhanceArmorGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, -55, 22, 38, 0.2f, enhanceAltarCanvas);
#pragma endregion

#pragma region Attack UP
		// ������ ��� => ���ݷ� ��
		CreateUISprite("EnhanceAttackBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, enhanceAltarCanvas);
		// �� ����
		TextureUI* enhanceAttackBG = enhanceAltarCanvas->GetTextureUI("EnhanceAttackBG");
		enhanceAttackBG->SetPosition({ -220.f, 200.f, 0.3f });


		// ������
		enhanceIcon = resourceManager->GetTexture("AttackUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("AttackUp", "Resources/UI/superpower_icon.png");

		TextureUI* attackTex = enhanceAltarCanvas->CreateTextureUI("EnhanceAttack");

		attackTex->SetParent(enhanceAttackBG);
		attackTex->SetPosition({ 5.f, 0.f, -0.01f });
		attackTex->SetHeight(100.f);
		attackTex->SetWidth(85.f);
		attackTex->SetTexture(enhanceIcon);

		// ��ư
		CreateUIButton("EnhanceAttackButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 200, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceAttackButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// �̸� Text
		TextUI* attackText = enhanceAltarCanvas->CreateTextUI("EnhanceAttackText");
		attackText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		attackText->SetPosition({ 50.f, 150.f, 0.2f });
		attackText->SetFont("MuseumMedium");
		attackText->SetFontSize(20.0f);
		attackText->SetText(L"����");
		attackText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// ���� Text
		TextUI* attackSubText = enhanceAltarCanvas->CreateTextUI("EnhanceAttackSubText");
		attackSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		attackSubText->SetPosition({ 270.f, 200.f, 0.2f });
		attackSubText->SetFont("MuseumMedium");
		attackSubText->SetFontSize(14.0f);
		attackSubText->SetText(L" ������ ���� �ϱ��� ����Ǿ��ִ�\n\n\n\n �ʿ����� �������� ����ϴ�.\n\n\n\n ���ݷ��� 10% �����մϴ�.");
		attackSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem ����.
		gemCount = enhanceAltarCanvas->CreateTextUI("EnhanceAttackGemCount");
		gemCount->SetPosition({ 270.f, 135.f, 0.2f });
		gemCount->SetFont("MuseumMedium");
		gemCount->SetFontSize(14.0f);
		gemCount->SetText("2");
		gemCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("EnhanceAttackGem", "MemoryIcon", "Resources/UI/MemoryGem_Icon.png",
			250, 135, 22, 38, 0.2f, enhanceAltarCanvas);
#pragma endregion

#pragma region GemLack
		CreateUISprite("EnhanceNotice", "Notice", "Resources/UI/OnMouse_Title2.png",
			0, 320, 700, 200, 0.19f, enhanceAltarCanvas);
		TextureUI* notice = enhanceAltarCanvas->GetTextureUI("EnhanceNotice");

		TextUI* noticeText = enhanceAltarCanvas->CreateTextUI("EnhanceNoticeText");
		noticeText->SetParent(notice);
		noticeText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		noticeText->SetPosition({ -20.f, -15.f, -0.01f });
		noticeText->SetFont("MuseumMedium");
		noticeText->SetFontSize(35.f);
		noticeText->SetText(L"��ȭ�� �����մϴ�!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("EnhanceNotice", false, enhanceAltarCanvas);

#pragma endregion

#pragma region CloseButton
		/// ���� ��ư
		CreateUISprite("StartAltarQuit", "QuitButton", "Resources/UI/quit_button.png",
			0, 450, 313, 58, 0.18f, enhanceAltarCanvas);

		CreateUIButton("StartAltarClose", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"CloseButtonHover", "Resources/UI/OnMouse_Title2.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			0, 450, 313, 58, 0.17f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("StartAltarClose")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		enhanceAltarCanvas->GetButtonUI("StartAltarClose")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("AltarClose");
			});

		/*ButtonUI* buttonBG = enhanceAltarCanvas->GetButtonUI("StartAltarClose");
		TextureUI* closeButton = enhanceAltarCanvas->CreateTextureUI("ButtonX");

		TextureBase* buttonX = resourceManager->GetTexture("ButtonX");
		if (buttonX == nullptr)
			buttonX = factory->CreateTexture("ButtonX", "Resources/UI/Button1_X.png");

		closeButton->SetParent(buttonBG);
		closeButton->SetPosition({ 0.f, 0.f, -0.01f });
		closeButton->SetHeight(75.f);
		closeButton->SetWidth(75.f);
		closeButton->SetTexture(buttonX);*/
#pragma endregion

		enhanceAltarCanvas->SetEnable(false);
	}

	void UIManager::CreateWeaponAltar()
	{
		// ���� ĵ������ WeaponAltarĵ������ ������ݴϴ�.
		Canvas* weaponAltarCanvas = mainCanvas->GetCanvas("WeaponAltar");

		// �⺻���� ���� ����
		// ���
		CreateUISprite("WeaponAltarWindow", "WeaponAltarWindow", "Resources/UI/weapon_altar_BG.png",
			40, 0, 2153, 1199, 0.5f, weaponAltarCanvas);
		//SetSpriteIsEnable("WeaponAltarWindow", false, weaponAltarCanvas);

		// Ÿ��Ʋ
		// �ؽ�Ʈ ����
		TextUI* title = weaponAltarCanvas->CreateTextUI("WeaponAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"���� ��ȭ");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// subtitle
		/*TextUI* subTitle = weaponAltarCanvas->CreateTextUI("WeaponAltarSubTitle");
		subTitle->SetPosition({ -8.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"���� ��ȭ");
		subTitle->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });*/

		// ���� �ϳ� ��ư �̹���.
		// ������, ��ư, �ؽ�Ʈ, ���׷��̵� ���� ����.
		// ������ ���
		CreateUISprite("Weapon1BG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, weaponAltarCanvas);
		// �� ����
		TextureUI* weapon1BG = weaponAltarCanvas->GetTextureUI("Weapon1BG");
		weapon1BG->SetPosition({ -220.f, -180.f, 0.3f });

		// ���ҽ� �Ŵ����� ���丮�� �޾ƿ´�.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// �⺻ ���� �ϳ� �־��.
			// �ΰ��� UI�� �̸��� ��������.
		TextureBase* weapon = resourceManager->GetTexture("ClubIcon");
		if (weapon == nullptr)
			weapon = factory->CreateTexture("ClubIcon", "Resources/UI/Weapon_500_500_2D_Image.png");

		// ���� �ϳ� ������ ���� �ڸ�
		TextureUI* weapon1Tex = weaponAltarCanvas->CreateTextureUI("Weapon1");

		weapon1Tex->SetParent(weapon1BG);
		weapon1Tex->SetPosition({ 0.0f, 0.0f, -0.01f });
		weapon1Tex->SetHeight(100.f);
		weapon1Tex->SetWidth(85.f);
		weapon1Tex->SetTexture(weapon);

		// ��ư �߰�.
		CreateUIButton("Weapon1Button", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, weaponAltarCanvas);
		weaponAltarCanvas->GetButtonUI("Weapon1Button")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// ���� �ؽ�Ʈ �߰�
		TextUI* weapon1Text = weaponAltarCanvas->CreateTextUI("Weapon1Text");
		weapon1Text->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon1Text->SetPosition({ -100.f, -220.f, 0.2f });
		weapon1Text->SetFont("MuseumMedium");
		weapon1Text->SetFontSize(20.0f);
		weapon1Text->SetText(L"ưư�� �������� ������");
		weapon1Text->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		TextUI* weapon1SubText = weaponAltarCanvas->CreateTextUI("Weapon1SubText");
		weapon1SubText->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon1SubText->SetPosition({ -110.f, -180.f, 0.2f });
		weapon1SubText->SetFont("MuseumMedium");
		weapon1SubText->SetFontSize(14.0f);
		weapon1SubText->SetText(L" ������ ������� ���� \n\n\n\n ���� ���� ��������\n\n\n\n �ı����� �ʰ� ��ȭ�˴ϴ�.");
		weapon1SubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		/*// ���׷��̵� ������
		CreateUISprite("Weapon1UpgradeIcon", "WeaponUpgradeIcon", "Resources/UI/Altar_UpgradeIcon.png",
			290, -5, 50, 50, 0.3f, weaponAltarCanvas);*/

			// ���� ��ȭ ����.
		TextUI* weaponCoinCount = weaponAltarCanvas->CreateTextUI("WeaponCoinCount");
		weaponCoinCount->SetPosition({ 270.f, -245.f, 0.2f });
		weaponCoinCount->SetFont("MuseumMedium");
		weaponCoinCount->SetFontSize(14.0f);
		weaponCoinCount->SetText("5");
		weaponCoinCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("WeaponCoin", "WeaponCoinIcon", "Resources/UI/Weapon_Coin.png",
			250, -245, 22, 38, 0.2f, weaponAltarCanvas);

		// �� ��° ���� ����.
			// �⺻������ ������ ���� Enable�� none����
				// �ΰ��� UI�� �̸��� ��������.
		/*weapon = resourceManager->GetTexture("SpikeClub");
		if (weapon == nullptr)
			weapon = factory->CreateTexture("SpikeClub", "Resources/UI/SpikeClub.png");

		// ������ ���
		CreateUISprite("Weapon2BG", "IconBG", "Resources/UI/Altar_WeaponIcon_BG.png", -40, 30, 110, 110, 0.3f, weaponAltarCanvas);
		// �� ����
		TextureUI* weapon2BG = weaponAltarCanvas->GetTextureUI("Weapon2BG");
		weapon2BG->SetPosition({ -180.f, 230.f, 0.3f });

		// ���� �ϳ� ������ ���� �ڸ�
		TextureUI* weapon2Tex = weaponAltarCanvas->CreateTextureUI("Weapon2");

		weapon2Tex->SetParent(weapon2BG);
		weapon2Tex->SetPosition({ 0.0f, 0.0f, -0.01f });
		weapon2Tex->SetHeight(85.f);
		weapon2Tex->SetWidth(85.f);
		weapon2Tex->SetTexture(weapon);
		weapon2Tex->SetEnable(false);

		// ��ư �߰�.
		CreateUIButton("Weapon2Button", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			0, 230, 610, 220, 0.35f, weaponAltarCanvas);
		SetButtonIsEnable("Weapon2Button", false, weaponAltarCanvas);

		// ���� �ؽ�Ʈ �߰�
		TextUI* weapon2Text = weaponAltarCanvas->CreateTextUI("Weapon2Text");
		weapon2Text->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon2Text->SetPosition({ -110.f, 210.f, 0.3f });
		weapon2Text->SetFont("HahmletBold");
		weapon2Text->SetFontSize(28.0f);
		weapon2Text->SetText("Master Sward");
		weapon2Text->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		weapon2Text->SetEnable(false);

		// ���׷��̵� ������
		CreateUISprite("Weapon2UpgradeIcon", "WeaponUpgradeIcon", "Resources/UI/Altar_UpgradeIcon.png",
			220, 225, 50, 50, 0.3f, weaponAltarCanvas);
		SetSpriteIsEnable("Weapon2UpgradeIcon", false, weaponAltarCanvas);*/

#pragma region GemLack
		CreateUISprite("WeaponNotice", "Notice", "Resources/UI/OnMouse_Title2.png",
			0, 320, 700, 200, 0.1f, weaponAltarCanvas);
		TextureUI* notice = weaponAltarCanvas->GetTextureUI("WeaponNotice");

		TextUI* noticeText = weaponAltarCanvas->CreateTextUI("WeaponNoticeText");
		noticeText->SetParent(notice);
		noticeText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		noticeText->SetPosition({ -20.f, -15.f, -0.01f });
		noticeText->SetFont("MuseumMedium");
		noticeText->SetFontSize(35.f);
		noticeText->SetText(L"��ȭ�� �����մϴ�!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("WeaponNotice", false, weaponAltarCanvas);

#pragma endregion

		/// ���� ��ư
		CreateUISprite("WeaponAltarQuit", "QuitButton", "Resources/UI/quit_button.png",
			0, 450, 313, 58, 0.08f, weaponAltarCanvas);

		CreateUIButton("WeaponAltarClose", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"CloseButtonHover", "Resources/UI/OnMouse_Title2.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			0, 450, 313, 58, 0.07f, weaponAltarCanvas);
		weaponAltarCanvas->GetButtonUI("WeaponAltarClose")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		weaponAltarCanvas->GetButtonUI("WeaponAltarClose")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("AltarClose");
			});

		/*
		ButtonUI* buttonBG = weaponAltarCanvas->GetButtonUI("WeaponAltarClose");
		TextureUI* closeButton = weaponAltarCanvas->CreateTextureUI("ButtonX");

		TextureBase* buttonX = resourceManager->GetTexture("ButtonX");
		if (buttonX == nullptr)
			buttonX = factory->CreateTexture("ButtonX", "Resources/UI/Button1_X.png");

		closeButton->SetParent(buttonBG);
		closeButton->SetPosition({ 0.f, 0.f, -0.01f });
		closeButton->SetHeight(75.f);
		closeButton->SetWidth(75.f);
		closeButton->SetTexture(buttonX);*/

		weaponAltarCanvas->SetEnable(false);
	}

	// ��� ȭ�鿡 ǥ�õǴ� ����� UI�� ��Ƶ� ĵ����.
	void UIManager::CreateDebugCanvas()
	{
		Canvas* debugCanvas = mainCanvas->GetCanvas("DebugCanvas");

		debugCanvas->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::TOP });
		debugCanvas->SetPosition({ -600.0f, -300.0f, 0.6f });

		/*// Altar Debug
		CreateUIText("AltarStateTitle", "AltarState : ", Vector(1.f, 1.f, 1.f), Vector(-800.f, -400.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("AltarState", "Altar", Vector(1.f, 1.f, 1.f), Vector(-400.f, -300.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		CreateUIText("AltarActivatedTitle", "AltarActivated : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 150.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Activate", "False", Vector(1.f, 1.f, 1.f), Vector(1220.f, 150.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		CreateUIText("CollideWithPlayer", "CollideWithPlayer : bool", Vector(1.f, 1.f, 1.f), Vector(1120.f, 175.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		CreateUIText("AltarPos", "AltarPos : ", Vector(1.f, 1.f, 1.f), Vector(100.f, 100.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);*/

		// �⺻���� �÷��̾� ����� �ڵ� ��ġ �̵�.
		CreateUIText("PlayerState : ", "PlayrState : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 50.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("State", "IdleState", Vector(1.f, 1.f, 1.f), Vector(1200.f, 50.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("PlayerPos", "PlayerPos : ", Vector(1.f, 1.f, 1.f), Vector(100.f, 120.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("PlayerCurrentHp", "PlayerCurrentHp : ", Vector(1.f, 1.f, 0.f), Vector(1120.f, 260.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		CreateUIText("PlayerRayCast", "PlayerRay : Empty", Vector(0.f, 1.f, 1.f), Vector(100.f, 460.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("PlayerRayCast", "PlayerRay : Empty", debugCanvas);

		CreateUIText("LookAt", "LookAt", Vector(1.f, 1.f, 1.f), Vector(100.f, 140.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Ray", "raychec ked", Vector(1.f, 1.f, 1.f), Vector(100.f, 600.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIIsEnable("Ray", false, debugCanvas);

		// ���� ����� �ڵ�
		CreateUIText("BossState", "BossState: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 320.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("BossHp", "BossHp: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 340.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("BossPhase", "BossPhase: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 360.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Distance-Player", "Distance-Player: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 400.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		// ����� ������Ʈ�� �����ϴ� ����� ����
		CreateUIText("MousePos", "MousePos", Vector(1.f, 1.f, 1.f), Vector(100.f, 270.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Inventory", "Inventory : OFF", Vector(1.f, 1.f, 1.f), Vector(1120.f, 240.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("IsInvincable", "Invincable : False", Vector(1.f, 1.f, 1.f), Vector(1120.f, 280.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);


		CreateUIText("HP", "HP : OFF", Vector(1.f, 1.f, 1.f), Vector(1120.f, 320.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("AttackPower", "AttackPower : OFF", Vector(1.f, 1.f, 1.f), Vector(1120.f, 340.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("RotateSpeed", "RotateSpeed : OFF", Vector(1.f, 1.f, 1.f), Vector(1120.f, 360.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("CursePain", "Curse : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 380.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("CurseMadness", "Curse : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 400.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("CurseSight", "Curse : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 420.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("WaeponUpgrade", "WaeponUpgrade : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 440.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		// ���� ����� �ڵ�.
		CreateUIText("PlayerDetected", "PlayerDetected : False", Vector(1.f, 1.f, 1.f), Vector(1120.f, 500.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("MonsterState", "MonsterState : Idle", Vector(1.f, 1.f, 1.f), Vector(1120.f, 520.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("MonsterState", "MonsterState : Idle", debugCanvas);
		CreateUIText("RayDirection", "RayDirection : Idle", Vector(1.f, 1.f, 1.f), Vector(1120.f, 540.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("RayDirection", "RayDirection : default", debugCanvas);
		CreateUIText("MonsterPosition", "MonsterPosition : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 560.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("MonsterPosition", "MonsterPosition : default", debugCanvas);
		CreateUIText("MonsterLook", "MonsterLook : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 580.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("MonsterLook", "MonsterLook : default", debugCanvas);
		CreateUIText("MonsterHp", "MonsterHP : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 800.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

#ifdef _DEBUG
		debugCanvas->SetEnable(true);
#else
		debugCanvas->SetEnable(false);
#endif
	}

	void UIManager::CreateMonologueCanvas()
	{
		Canvas* monologueCanvas = GetCanvas("MonologueCanvas");

		// ��ȭ�� �� ��Ҹ� �̸� ����� �дٴ� ������ ������.
		// ������ ��ȭ ��ü�� �� ������Ʈ���� ���ڿ��� ����־ ����� ���� ������ ����.

		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		GraphicsEngineSpace::TextureUI* monologuePointer = monologueCanvas->CreateTextureUI("MonologuePointer");

		GraphicsEngineSpace::TextureBase* pointerTex = resourceManager->GetTexture("Cursor");
		if (pointerTex == nullptr)
			pointerTex = factory->CreateTexture("Cursor", "Resources/UI/cursor-08.png");

		monologuePointer->SetTexture(pointerTex);
		monologuePointer->SetWidth(30.f);
		monologuePointer->SetHeight(30.f);
		//monologuePointer->SetPosition({ 60.0f, 90.f, 0.2f });

		monologuePointer->SetEnable(false);

		CreateUIText("Monologue0", "", { 1.0f, 1.0f, 1.0f }, { -800.f, 0.f, 0.2f }, 0.0f,
			{ 1.0f, 1.0f, 1.0f }, monologueCanvas);
		TextUI* monologue0 = monologueCanvas->GetTextUI("Monologue0");
		monologue0->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue0->SetFont("MuseumBold");
		monologue0->SetFontSize(40.0f);
		monologue0->SetText(L"Episode VIII : ����޵����� �߻��� ����");
		monologue0->SetEnable(false);

		//monologueCanvas->SetEnable(false);
		TextUI* monologue1 = monologueCanvas->CreateTextUI("Monologue1");
		monologue1->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue1->SetPosition({ -800.f, 0.f, 0.2f });
		monologue1->SetFont("MuseumMedium");
		monologue1->SetFontSize(20.f);
		monologue1->SetText(L" ...\n\n\n �ð��� �󸶳� �帥����.\n\n\n �ƴ�, ��� �ð��� �󸶳� �帥������ �߿�ġ �ʴ�.");
		monologue1->SetEnable(false);

		TextUI* monologue2 = monologueCanvas->CreateTextUI("Monologue2");
		monologue2->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue2->SetPosition({ -800.f, 0.f, 0.2f });
		monologue2->SetFont("MuseumMedium");
		monologue2->SetFontSize(20.f);
		monologue2->SetText(L" ���� ���� �Ƴ��� ���̵��� ���� �� ��.\n\n\n �װ͸��� �߿��� ���� ��.");
		monologue2->SetEnable(false);

		TextUI* monologue3 = monologueCanvas->CreateTextUI("Monologue3");
		monologue3->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue3->SetPosition({ -800.f, 0.f, 0.2f });
		monologue3->SetFont("MuseumMedium");
		monologue3->SetFontSize(20.f);
		monologue3->SetText(L" �����ϰ� ��ѷ��� �߰ſ� �� ������ ��.\n\n\n ������ �ľ���� �׵��� ���� �기 ������ ���� �̷길ŭ ���\n\n\n �ٽô� ������ �긮�� ���� ���̶� ����������...\n\n\n ǥ���� �Ҿ���� �� ���� �� �ٽ� �߰ſ� ������ �귯���ȴ�.");
		monologue3->SetEnable(false);

		TextUI* monologue4 = monologueCanvas->CreateTextUI("Monologue4");
		monologue4->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue4->SetPosition({ -800.f, 0.f, 0.2f });
		monologue4->SetFont("MuseumMedium");
		monologue4->SetFontSize(20.f);
		monologue4->SetText(L" ���� ������ �Ҿ���� ä\n\n\n ���ž��� �̷ﳽ ������ ����� 7��.");
		monologue4->SetEnable(false);

		TextUI* monologue5 = monologueCanvas->CreateTextUI("Monologue5");
		monologue5->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue5->SetPosition({ -800.f, 0.f, 0.2f });
		monologue5->SetFont("MuseumMedium");
		monologue5->SetFontSize(20.f);
		monologue5->SetText(L" �׵鿡�� �����̳��� �����ϱ� ����.\n\n\n 8��° ������ �����Ѵ�.\n\n\n �ε�, � ���� ����� ���ֿ��� ��� �׵��� ������ �� �� �ֱ�.");
		monologue5->SetEnable(false);

		monologueCanvas->SetEnable(false);
	}

	void UIManager::CreateDialogCanvas()
	{
		Canvas* dialogCanvas = GetCanvas("DialogCanvas");
		dialogCanvas->SetEnable(false);

		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// ��ȭ ���
		TextureUI* dialogBG = dialogCanvas->CreateTextureUI("DialogBG");

		TextureBase* BGTex = resourceManager->GetTexture("DialogBG");
		if (BGTex == nullptr)
			BGTex = factory->CreateTexture("DialogBG", "Resources/UI/talk.png");

		dialogBG->SetTexture(BGTex);
		dialogBG->SetWidth(1000.f);
		dialogBG->SetHeight(300.f);
		dialogBG->SetPosition({ 150.0f, 350.f, 0.15f });

		// �⺻���� ������. => ��ȭâ������ ��ġ ����.
		TextureUI* dialogPointer = dialogCanvas->CreateTextureUI("DialogPointer");

		TextureBase* pointerTex = resourceManager->GetTexture("Cursor");
		if (pointerTex == nullptr)
			pointerTex = factory->CreateTexture("Cursor", "Resources/UI/cursor-08.png");

		dialogPointer->SetTexture(pointerTex);
		dialogPointer->SetWidth(50.f);
		dialogPointer->SetHeight(50.f);
		dialogPointer->SetPosition({ 550.0f, 430.f, 0.1f });

		dialogPointer->SetEnable(false);

		// ��ȭ �̹���
		TextureUI* dialogImage = dialogCanvas->CreateTextureUI("DialogHeracles");

		TextureBase* heraclesTex = resourceManager->GetTexture("DialogHeracles");
		if (heraclesTex == nullptr)
			heraclesTex = factory->CreateTexture("DialogHeracles", "Resources/UI/Dialog_Heracles.png");

		dialogImage->SetTexture(heraclesTex);
		dialogImage->SetWidth(450.f);
		dialogImage->SetHeight(450.f);
		dialogImage->SetPosition({ -450.f, 300.f, 0.05f });

		// �ϳ��� ���� �� ��ȭ �ؽ�Ʈ
		TextUI* dialogText = dialogCanvas->CreateTextUI("DialogText");
		dialogText->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		dialogText->SetPosition({ -200.f, 300.f, 0.13f });
		dialogText->SetFont("MuseumMedium");
		dialogText->SetFontSize(22.f);
		dialogText->SetText(L" ���� ���� �ܴ�������. �������� ���̷������ ���ݿ��� ���� ���ڱ�");

		dialogCanvas->SetEnable(false);
	}

	void UIManager::CreateTutorialCanvas()
	{
		Canvas* tutorial = mainCanvas->GetCanvas("TutorialCanvas");

		CreateUISprite("WeaponTutorial1", "WeaponTutorial1", "Resources/UI/Weapons_Mounted1.png",
		-10, 0, 2153, 1199, 0.01f, tutorial);
		tutorial->GetTextureUI("WeaponTutorial1")->SetEnable(false);

		CreateUISprite("WeaponTutorial2", "WeaponTutorial2", "Resources/UI/Weapons_Mounted2.png",
		30, 10, 2153, 1199, 0.01f, tutorial);
		tutorial->GetTextureUI("WeaponTutorial2")->SetEnable(false);

		CreateUISprite("AltarTutorial", "AltarTutorial", "Resources/UI/altar_tutorial.png",
		0, -10, 2153, 1199, 0.01f, tutorial);
		tutorial->GetTextureUI("AltarTutorial")->SetEnable(false);

		CreateUISprite("ControlTutorial", "Controls", "Resources/UI/controls_tab.png",
			0, 0, 956, 904, 0.01f, tutorial);
		tutorial->GetTextureUI("ControlTutorial")->SetEnable(false);

		tutorial->SetEnable(false);
	}

	void UIManager::CreateEndingCanvas()
	{
		Canvas* ending = mainCanvas->GetCanvas("EndingCanvas");

		CreateUISprite("End", "EndBG", "Resources/UI/win_BG.png",
		50, 0, 2153, 1199, 0.1f, ending);

		TextUI* endText = ending->CreateTextUI("EndText");
		endText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		endText->SetPosition({ 30.f, 20.f, 0.08f });
		endText->SetFont("MuseumMedium");
		endText->SetFontSize(30.f);
		endText->SetText(L" 00 : 00");


		TextUI* restartText = ending->CreateTextUI("EndRestartText");
		restartText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		restartText->SetPosition({ 20.f, 480.f, 0.08f });
		restartText->SetFont("MuseumMedium");
		restartText->SetFontSize(20.f);
		restartText->SetText(L"�����");

		// �ؽ��� ��Ȱ��.
		CreateUIButton("EndRestartButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"CloseButtonHover", "Resources/UI/OnMouse_Title2.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			32, 490, 150, 58, 0.07f, ending);

		ending->GetButtonUI("EndRestartButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});
		ending->GetButtonUI("EndRestartButton")->SetClickEvent([&]()
			{
				soundInstance->PlayEffect("AltarClose");

				// ���⼭ ���� �޴� �ε� ��..
				SceneManager::GetInstance()->LoadScene(TEXT("MainMenu"));
			});

		ending->SetEnable(false);
	}
}
