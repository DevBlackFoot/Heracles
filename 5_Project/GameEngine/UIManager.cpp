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
		// 그래픽스 매니저 캐싱
		graphicsInstance = GraphicsManager::GetInstance();
		inputInstance = InputManager::GetInstance();
		soundInstance = SoundManager::GetInstance();

		// 캔버스 생성i
		auto renderer = graphicsInstance->GetRenderer();
		auto width = WindowManager::GetInstance()->GetClientWidth();
		auto height = WindowManager::GetInstance()->GetClientHeight();
		nowWidth = width;
		nowHeight = height;

		mainCanvas = renderer->CreateFactory()->CreateCanvas("Canvas", width, height);

		/// 게임에서 사용할 모든 캔버스들의 목록
		// 시작 위치 제단 저주 - CurseAltar
		mainCanvas->CreateCanvas("CurseAltar");
		// 시작 위치 제단 초월 - EnhanceAltar
		mainCanvas->CreateCanvas("EnhanceAltar");
		// 무기 제단 - WeaponAltar
		mainCanvas->CreateCanvas("WeaponAltar");
		// 인벤토리 - Inventory
		mainCanvas->CreateCanvas("Inventory");
		// 디버그 캔버스 -> DebugCanvas;
		mainCanvas->CreateCanvas("DebugCanvas");

		// 모놀로그 캔버스
		mainCanvas->CreateCanvas("MonologueCanvas");
		// 대화창 캔버스 -> DialogCanvas;
		mainCanvas->CreateCanvas("DialogCanvas");

		// 튜토리얼 들을 모아둔 튜토리얼 캔버스
		mainCanvas->CreateCanvas("TutorialCanvas");

		// 엔딩 씬
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

		// 캔버스로 통합
		mainCanvas->Render(graphicsInstance->GetRenderer().get(), Timer::GetInstance()->DeltaTime());
	}

	// 해당 함수는 Input 업데이트 이후에 있어야한다.
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

		// 비율 계산
		float xScale = static_cast<float>(width) / nowWidth;
		float yScale = static_cast<float>(height) / nowHeight;

		// 모든 캔버스의 스케일을 변경합니다.
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

		// 배경
		CreateUISprite("InventoryWindow", "InventoryBG", "Resources/UI/BG.png"
			, -50, 0, 2153, 1199, 0.5f, inventory);

		// 아이콘으로 넣을애들을 위해
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// 각종 슬롯
		// 무기
		CreateUISprite("InventoryWeaponSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, -230, 130, 176, 0.3f, inventory);

		TextureUI* slot = inventory->GetTextureUI("InventoryWeaponSlot");
		TextureUI* icon = inventory->CreateTextureUI("InventoryWeaponIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryWeaponScript")->SetEnable(true);
			});

		// 저주

		// 고통
		CreateUISprite("InventoryCursePainSlot", "InventorySlot", "Resources/UI/slot.png"
			, -620, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCursePainSlot");
		icon = inventory->CreateTextureUI("InventoryCursePainIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCursePainScript")->SetEnable(true);
			});

		// 광기
		CreateUISprite("InventoryCurseMadnessSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseMadnessSlot");
		icon = inventory->CreateTextureUI("InventoryCurseMadnessIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCurseMadnessScript")->SetEnable(true);
			});

		// 시야
		CreateUISprite("InventoryCurseBlindSlot", "InventorySlot", "Resources/UI/slot.png"
			, -400, -30, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryCurseBlindSlot");
		icon = inventory->CreateTextureUI("InventoryCurseBlindIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryCurseBlindScript")->SetEnable(true);
			});

		// 초월

		// HP
		CreateUISprite("InventoryEnhanceHPSlot", "InventorySlot", "Resources/UI/slot.png"
			, -620, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceHPSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceHPIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceHPScript")->SetEnable(true);
			});

		// 방어력
		CreateUISprite("InventoryEnhanceArmorSlot", "InventorySlot", "Resources/UI/slot.png"
			, -510, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceArmorSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceArmorIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceArmorScript")->SetEnable(true);
			});

		// 공격업
		CreateUISprite("InventoryEnhanceAttackSlot", "InventorySlot", "Resources/UI/slot.png"
			, -400, 170, 130, 176, 0.3f, inventory);

		slot = inventory->GetTextureUI("InventoryEnhanceAttackSlot");
		icon = inventory->CreateTextureUI("InventoryEnhanceAttackIcon");

		// 들어갈 텍스쳐
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

				// 해당되는 설명 이미지 출력
				mainCanvas->GetCanvas("Inventory")->GetTextureUI("InventoryEnhanceAttackScript")->SetEnable(true);
			});

		// 각종 이미지 스프라이트.
		// 무기..
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

		// 저주
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

		// 광기
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

		// 시야
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

		// 방어
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

		// 공격
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

		/// 배경 => 이후에 받으면 변경.
		CreateUISprite("CurseAltarWindow", "StartAltarWindow", "Resources/UI/altar_ui_BG.png",
			-50, 0, 2153, 1199, 0.5f, curseAltarCanvas);

#pragma region Title
		/// 타이틀
		// 텍스트 세팅
		TextUI* title = curseAltarCanvas->CreateTextUI("CurseAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"신의 축복");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// SubTitle
			// 저주 해방, 능력 업그레이드.
		TextUI* subTitle = curseAltarCanvas->CreateTextUI("CurseAltarSubTitleCurse");
		subTitle->SetPosition({ -155.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"저주 해방");
		subTitle->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });

		// 선택 버튼 추가
		CreateUIButton("CurseAltarCurseSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-142, -315, 280, 80, 0.38f, curseAltarCanvas);
		// hover이벤트 추가.
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
		subTitle2->SetText(L"초월");
		subTitle2->SetColor({ 0.2196f, 0.2510f, 0.2784f, 1.0f });

		// 선택 버튼 추가
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

		// 리소스 매니저와 팩토리를 받아온다.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		/// 저주 해제
#pragma region CurseOfPain
		// 아이콘 배경 => 최대 HP 업
		CreateUISprite("CursePainBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, curseAltarCanvas);
		// 상세 세팅
		TextureUI* cursePainBG = curseAltarCanvas->GetTextureUI("CursePainBG");
		cursePainBG->SetPosition({ -220.f, -180.f, 0.3f });

		// 아이콘
		TextureBase* curseIcon = resourceManager->GetTexture("Pain");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Pain", "Resources/UI/CurseIcon_Pain.png");

		TextureUI* PainTex = curseAltarCanvas->CreateTextureUI("CursePain");

		PainTex->SetParent(cursePainBG);
		PainTex->SetPosition({ 5.f, 0.f, -0.01f });
		PainTex->SetHeight(100.f);
		PainTex->SetWidth(100.f);
		PainTex->SetTexture(curseIcon);

		// 버튼
		CreateUIButton("CursePainButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CursePainButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* PainText = curseAltarCanvas->CreateTextUI("CursePainText");
		PainText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		PainText->SetPosition({ 50.f, -220.f, 0.2f });
		PainText->SetFont("MuseumMedium");
		PainText->SetFontSize(20.0f);
		PainText->SetText(L"파고드는 고통");
		PainText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* PainSubText = curseAltarCanvas->CreateTextUI("CursePainSubText");
		PainSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		PainSubText->SetPosition({ 170.f, -170.f, 0.2f });
		PainSubText->SetFont("MuseumMedium");
		PainSubText->SetFontSize(14.0f);
		PainSubText->SetText(L" 극악의 고통으로\n\n\n\n 방어력이 10% 감소합니다.");
		PainSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		// 아이콘 배경 => 방어력 업
		CreateUISprite("CurseMadnessBG", "AltarSlot", "Resources/UI/altar_iconbox.png"
			, -40, 30, 150, 230, 0.3f, curseAltarCanvas);
		// 상세 세팅
		TextureUI* curseMadnessBG = curseAltarCanvas->GetTextureUI("CurseMadnessBG");
		curseMadnessBG->SetPosition({ -220.f, 10.f, 0.3f });

		// 아이콘
		curseIcon = resourceManager->GetTexture("Madness");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Madness", "Resources/UI/CurseIcon_Madness.png");

		TextureUI* madnessTex = curseAltarCanvas->CreateTextureUI("CurseMadness");

		madnessTex->SetParent(curseMadnessBG);
		madnessTex->SetPosition({ 5.f, 0.f, -0.01f });
		madnessTex->SetHeight(100.f);
		madnessTex->SetWidth(100.f);
		madnessTex->SetTexture(curseIcon);

		// 버튼
		CreateUIButton("CurseMadnessButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 10, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CurseMadnessButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* madnessText = curseAltarCanvas->CreateTextUI("CurseMadnessText");
		madnessText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		madnessText->SetPosition({ 50.f, -30.f, 0.2f });
		madnessText->SetFont("MuseumMedium");
		madnessText->SetFontSize(20.0f);
		madnessText->SetText(L"끓어오르는 광기");
		madnessText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* madnessSubText = curseAltarCanvas->CreateTextUI("CurseMadnessSubText");
		madnessSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		madnessSubText->SetPosition({ 170.f, 20.f, 0.2f });
		madnessSubText->SetFont("MuseumMedium");
		madnessSubText->SetFontSize(14.0f);
		madnessSubText->SetText(L" 광기에 젖은 적의 이동속도가\n\n\n\n 10% 증가합니다.");
		madnessSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		// 아이콘 배경 => 공격력 업
		CreateUISprite("CurseBlindBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, curseAltarCanvas);
		// 상세 세팅
		TextureUI* curseBlindBG = curseAltarCanvas->GetTextureUI("CurseBlindBG");
		curseBlindBG->SetPosition({ -220.f, 200.f, 0.3f });


		// 아이콘
		curseIcon = resourceManager->GetTexture("Blind");
		if (curseIcon == nullptr)
			curseIcon = factory->CreateTexture("Blind", "Resources/UI/CurseIcon_Blind.png");

		TextureUI* blindTex = curseAltarCanvas->CreateTextureUI("CurseBlind");

		blindTex->SetParent(curseBlindBG);
		blindTex->SetPosition({ 5.f, 0.f, -0.01f });
		blindTex->SetHeight(100.f);
		blindTex->SetWidth(100.f);
		blindTex->SetTexture(curseIcon);

		// 버튼
		CreateUIButton("CurseBlindButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 200, 660, 250, 0.25f, curseAltarCanvas);
		curseAltarCanvas->GetButtonUI("CurseBlindButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* blindText = curseAltarCanvas->CreateTextUI("CurseBlindText");
		blindText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		blindText->SetPosition({ 50.f, 160.f, 0.2f });
		blindText->SetFont("MuseumMedium");
		blindText->SetFontSize(20.0f);
		blindText->SetText(L"보이지 않는 시야");
		blindText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* blindSubText = curseAltarCanvas->CreateTextUI("CurseBlindSubText");
		blindSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		blindSubText->SetPosition({ 170.f, 210.f, 0.2f });
		blindSubText->SetFont("MuseumMedium");
		blindSubText->SetFontSize(14.0f);
		blindSubText->SetText(L" 시각적 착란으로 인해\n\n\n\n 볼 수 있는 시야가 제한됩니다.");
		blindSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		noticeText->SetText(L"재화가 부족합니다!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("CurseNotice", false, curseAltarCanvas);

#pragma endregion

#pragma region CloseButton
		/// 종료 버튼
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
		// 이후에 쓸 텍스쳐 미리 로드
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
		// 메인 캔버스에 StartAltar캔버스를 만들어줍니다.
		Canvas* enhanceAltarCanvas = mainCanvas->GetCanvas("EnhanceAltar");

		/// 배경 => 이후에 받으면 변경.
		CreateUISprite("EnhanceAltarWindow", "StartAltarWindow", "Resources/UI/altar_ui_BG.png",
			-50, 0, 2153, 1199, 0.5f, enhanceAltarCanvas);

#pragma region Title
		/// 타이틀
		// 텍스트 세팅
		TextUI* title = enhanceAltarCanvas->CreateTextUI("EnhanceAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"신의 축복");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// SubTitle
			// 저주 해방, 능력 업그레이드.
		TextUI* subTitle = enhanceAltarCanvas->CreateTextUI("EnhanceAltarSubTitleCurse");
		subTitle->SetPosition({ -155.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"저주 해방");
		subTitle->SetColor({ 0.2196f, 0.2510f, 0.2784f, 1.0f });

		// 선택 버튼 추가
		CreateUIButton("EnhanceAltarCurseSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-142, -315, 280, 80, 0.38f, enhanceAltarCanvas);

		TextUI* subTitle2 = enhanceAltarCanvas->CreateTextUI("EnhanceAltarSubTitleEnhance");
		subTitle2->SetPosition({ 135.0f, -320.0f, 0.4f });
		subTitle2->SetFont("MuseumMedium");
		subTitle2->SetFontSize(20.0f);
		subTitle2->SetText(L"초월");
		subTitle2->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });

		// 선택 버튼 추가
		CreateUIButton("EnhanceAltarEnhanceSelectButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"MenuButtonHover", "Resources/UI/OnMouse_Title.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			145, -315, 280, 80, 0.38f, enhanceAltarCanvas);
#pragma endregion

		// 리소스 매니저와 팩토리를 받아온다.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		/// 강화 버튼
#pragma region HPButton
		// 아이콘 배경 => 최대 HP 업
		CreateUISprite("EnhanceHPBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, enhanceAltarCanvas);
		// 상세 세팅
		TextureUI* enhanceHPBG = enhanceAltarCanvas->GetTextureUI("EnhanceHPBG");
		enhanceHPBG->SetPosition({ -220.f, -180.f, 0.3f });

		// 아이콘
		TextureBase* enhanceIcon = resourceManager->GetTexture("MaxHPUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("MaxHPUp", "Resources/UI/patience_icon.png");

		TextureUI* HPTex = enhanceAltarCanvas->CreateTextureUI("EnhanceHP");

		HPTex->SetParent(enhanceHPBG);
		HPTex->SetPosition({ 5.f, 0.f, -0.01f });
		HPTex->SetHeight(100.f);
		HPTex->SetWidth(85.f);
		HPTex->SetTexture(enhanceIcon);

		// 버튼
		CreateUIButton("EnhanceHPButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceHPButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* HPText = enhanceAltarCanvas->CreateTextUI("EnhanceHPText");
		HPText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		HPText->SetPosition({ 50.f, -230.f, 0.2f });
		HPText->SetFont("MuseumMedium");
		HPText->SetFontSize(20.0f);
		HPText->SetText(L"인내");
		HPText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* HPSubText = enhanceAltarCanvas->CreateTextUI("EnhanceHPSubText");
		HPSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		HPSubText->SetPosition({ 270.f, -180.f, 0.2f });
		HPSubText->SetFont("MuseumMedium");
		HPSubText->SetFontSize(14.0f);
		HPSubText->SetText(L" 죽음을 불사한 끊임없는 도전으로\n\n\n\n 초월적인 인내의 힘을 얻습니다.\n\n\n\n 최대 HP가 20 증가합니다.");
		HPSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		// 아이콘 배경 => 방어력 업
		CreateUISprite("EnhanceArmorBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, enhanceAltarCanvas);
		// 상세 세팅
		TextureUI* enhanceArmorBG = enhanceAltarCanvas->GetTextureUI("EnhanceArmorBG");
		enhanceArmorBG->SetPosition({ -220.f, 10.f, 0.3f });

		// 아이콘
		enhanceIcon = resourceManager->GetTexture("ArmorUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("ArmorUp", "Resources/UI/steelskin_icon.png");

		TextureUI* armorTex = enhanceAltarCanvas->CreateTextureUI("EnhanceArmor");

		armorTex->SetParent(enhanceArmorBG);
		armorTex->SetPosition({ 5.f, 0.f, -0.01f });
		armorTex->SetHeight(100.f);
		armorTex->SetWidth(85.f);
		armorTex->SetTexture(enhanceIcon);

		// 버튼
		CreateUIButton("EnhanceArmorButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 10, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceArmorButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* armorText = enhanceAltarCanvas->CreateTextUI("EnhanceArmorText");
		armorText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		armorText->SetPosition({ 50.f, -40.f, 0.2f });
		armorText->SetFont("MuseumMedium");
		armorText->SetFontSize(20.0f);
		armorText->SetText(L"강철 피부");
		armorText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* armorSubText = enhanceAltarCanvas->CreateTextUI("EnhanceArmorSubText");
		armorSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		armorSubText->SetPosition({ 270.f, 10.f, 0.2f });
		armorSubText->SetFont("MuseumMedium");
		armorSubText->SetFontSize(14.0f);
		armorSubText->SetText(L" 적의 공격에도 피해를 입지 않는\n\n\n\n 신의 강철 피부를 얻게 됩니다.\n\n\n\n 방어력이 10% 증가합니다.");
		armorSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		// 아이콘 배경 => 공격력 업
		CreateUISprite("EnhanceAttackBG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-40, 30, 150, 230, 0.3f, enhanceAltarCanvas);
		// 상세 세팅
		TextureUI* enhanceAttackBG = enhanceAltarCanvas->GetTextureUI("EnhanceAttackBG");
		enhanceAttackBG->SetPosition({ -220.f, 200.f, 0.3f });


		// 아이콘
		enhanceIcon = resourceManager->GetTexture("AttackUp");
		if (enhanceIcon == nullptr)
			enhanceIcon = factory->CreateTexture("AttackUp", "Resources/UI/superpower_icon.png");

		TextureUI* attackTex = enhanceAltarCanvas->CreateTextureUI("EnhanceAttack");

		attackTex->SetParent(enhanceAttackBG);
		attackTex->SetPosition({ 5.f, 0.f, -0.01f });
		attackTex->SetHeight(100.f);
		attackTex->SetWidth(85.f);
		attackTex->SetTexture(enhanceIcon);

		// 버튼
		CreateUIButton("EnhanceAttackButton", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, 200, 660, 250, 0.25f, enhanceAltarCanvas);
		enhanceAltarCanvas->GetButtonUI("EnhanceAttackButton")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 이름 Text
		TextUI* attackText = enhanceAltarCanvas->CreateTextUI("EnhanceAttackText");
		attackText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		attackText->SetPosition({ 50.f, 150.f, 0.2f });
		attackText->SetFont("MuseumMedium");
		attackText->SetFontSize(20.0f);
		attackText->SetText(L"괴력");
		attackText->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 설명 Text
		TextUI* attackSubText = enhanceAltarCanvas->CreateTextUI("EnhanceAttackSubText");
		attackSubText->SetPivot({ HorizontalLocation::CENTER, VerticalLocation::MIDDLE });
		attackSubText->SetPosition({ 270.f, 200.f, 0.2f });
		attackSubText->SetFont("MuseumMedium");
		attackSubText->SetFontSize(14.0f);
		attackSubText->SetText(L" 내면의 힘을 일깨워 잠재되어있던\n\n\n\n 초월적인 강인함을 얻습니다.\n\n\n\n 공격력이 10% 증가합니다.");
		attackSubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		// Gem 개수.
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
		noticeText->SetText(L"재화가 부족합니다!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("EnhanceNotice", false, enhanceAltarCanvas);

#pragma endregion

#pragma region CloseButton
		/// 종료 버튼
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
		// 메인 캔버스에 WeaponAltar캔버스를 만들어줍니다.
		Canvas* weaponAltarCanvas = mainCanvas->GetCanvas("WeaponAltar");

		// 기본적인 제단 세팅
		// 배경
		CreateUISprite("WeaponAltarWindow", "WeaponAltarWindow", "Resources/UI/weapon_altar_BG.png",
			40, 0, 2153, 1199, 0.5f, weaponAltarCanvas);
		//SetSpriteIsEnable("WeaponAltarWindow", false, weaponAltarCanvas);

		// 타이틀
		// 텍스트 세팅
		TextUI* title = weaponAltarCanvas->CreateTextUI("WeaponAltarTitle");
		title->SetPosition({ -18.0f, -435.0f, 0.4f });
		title->SetFont("MuseumMedium");
		title->SetFontSize(40.0f);
		title->SetText(L"무기 강화");
		title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		// subtitle
		/*TextUI* subTitle = weaponAltarCanvas->CreateTextUI("WeaponAltarSubTitle");
		subTitle->SetPosition({ -8.0f, -320.0f, 0.4f });
		subTitle->SetFont("MuseumMedium");
		subTitle->SetFontSize(20.0f);
		subTitle->SetText(L"무기 강화");
		subTitle->SetColor({ 0.9373f, 0.7608f, 0.7608f, 1.0f });*/

		// 무기 하나 버튼 이미지.
		// 아이콘, 버튼, 텍스트, 업그레이드 가능 여부.
		// 아이콘 배경
		CreateUISprite("Weapon1BG", "AltarSlot", "Resources/UI/altar_iconbox.png",
			-100, -100, 150, 230, 0.3f, weaponAltarCanvas);
		// 상세 세팅
		TextureUI* weapon1BG = weaponAltarCanvas->GetTextureUI("Weapon1BG");
		weapon1BG->SetPosition({ -220.f, -180.f, 0.3f });

		// 리소스 매니저와 팩토리를 받아온다.
		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// 기본 무기 하나 넣어보기.
			// 인게임 UI와 이름을 맞춰주자.
		TextureBase* weapon = resourceManager->GetTexture("ClubIcon");
		if (weapon == nullptr)
			weapon = factory->CreateTexture("ClubIcon", "Resources/UI/Weapon_500_500_2D_Image.png");

		// 무기 하나 아이콘 넣을 자리
		TextureUI* weapon1Tex = weaponAltarCanvas->CreateTextureUI("Weapon1");

		weapon1Tex->SetParent(weapon1BG);
		weapon1Tex->SetPosition({ 0.0f, 0.0f, -0.01f });
		weapon1Tex->SetHeight(100.f);
		weapon1Tex->SetWidth(85.f);
		weapon1Tex->SetTexture(weapon);

		// 버튼 추가.
		CreateUIButton("Weapon1Button", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Activation_box.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			-5, -180, 660, 250, 0.25f, weaponAltarCanvas);
		weaponAltarCanvas->GetButtonUI("Weapon1Button")->SetEnterEvent([&]()
			{
				soundInstance->PlayEffect("UIHover");
			});

		// 무기 텍스트 추가
		TextUI* weapon1Text = weaponAltarCanvas->CreateTextUI("Weapon1Text");
		weapon1Text->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon1Text->SetPosition({ -100.f, -220.f, 0.2f });
		weapon1Text->SetFont("MuseumMedium");
		weapon1Text->SetFontSize(20.0f);
		weapon1Text->SetText(L"튼튼한 떡갈나무 몽둥이");
		weapon1Text->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

		TextUI* weapon1SubText = weaponAltarCanvas->CreateTextUI("Weapon1SubText");
		weapon1SubText->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon1SubText->SetPosition({ -110.f, -180.f, 0.2f });
		weapon1SubText->SetFont("MuseumMedium");
		weapon1SubText->SetFontSize(14.0f);
		weapon1SubText->SetText(L" 영험한 기운으로 인해 \n\n\n\n 신의 힘이 더해져도\n\n\n\n 파괴되지 않고 강화됩니다.");
		weapon1SubText->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

		/*// 업그레이드 아이콘
		CreateUISprite("Weapon1UpgradeIcon", "WeaponUpgradeIcon", "Resources/UI/Altar_UpgradeIcon.png",
			290, -5, 50, 50, 0.3f, weaponAltarCanvas);*/

			// 무기 재화 개수.
		TextUI* weaponCoinCount = weaponAltarCanvas->CreateTextUI("WeaponCoinCount");
		weaponCoinCount->SetPosition({ 270.f, -245.f, 0.2f });
		weaponCoinCount->SetFont("MuseumMedium");
		weaponCoinCount->SetFontSize(14.0f);
		weaponCoinCount->SetText("5");
		weaponCoinCount->SetColor({ 1.0f,  1.0f, 1.0f, 1.0f });

		// MemoryGem
		CreateUISprite("WeaponCoin", "WeaponCoinIcon", "Resources/UI/Weapon_Coin.png",
			250, -245, 22, 38, 0.2f, weaponAltarCanvas);

		// 두 번째 무기 슬롯.
			// 기본적으로 시작할 때는 Enable을 none으로
				// 인게임 UI와 이름을 맞춰주자.
		/*weapon = resourceManager->GetTexture("SpikeClub");
		if (weapon == nullptr)
			weapon = factory->CreateTexture("SpikeClub", "Resources/UI/SpikeClub.png");

		// 아이콘 배경
		CreateUISprite("Weapon2BG", "IconBG", "Resources/UI/Altar_WeaponIcon_BG.png", -40, 30, 110, 110, 0.3f, weaponAltarCanvas);
		// 상세 세팅
		TextureUI* weapon2BG = weaponAltarCanvas->GetTextureUI("Weapon2BG");
		weapon2BG->SetPosition({ -180.f, 230.f, 0.3f });

		// 무기 하나 아이콘 넣을 자리
		TextureUI* weapon2Tex = weaponAltarCanvas->CreateTextureUI("Weapon2");

		weapon2Tex->SetParent(weapon2BG);
		weapon2Tex->SetPosition({ 0.0f, 0.0f, -0.01f });
		weapon2Tex->SetHeight(85.f);
		weapon2Tex->SetWidth(85.f);
		weapon2Tex->SetTexture(weapon);
		weapon2Tex->SetEnable(false);

		// 버튼 추가.
		CreateUIButton("Weapon2Button", "AltarNone", "Resources/UI/Altar_MouseNone.png",
			"AltarHover", "Resources/UI/Altar_OnMouseFrame.png", "AltarDown", "Resources/UI/Altar_MouseDown.png",
			0, 230, 610, 220, 0.35f, weaponAltarCanvas);
		SetButtonIsEnable("Weapon2Button", false, weaponAltarCanvas);

		// 무기 텍스트 추가
		TextUI* weapon2Text = weaponAltarCanvas->CreateTextUI("Weapon2Text");
		weapon2Text->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		weapon2Text->SetPosition({ -110.f, 210.f, 0.3f });
		weapon2Text->SetFont("HahmletBold");
		weapon2Text->SetFontSize(28.0f);
		weapon2Text->SetText("Master Sward");
		weapon2Text->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		weapon2Text->SetEnable(false);

		// 업그레이드 아이콘
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
		noticeText->SetText(L"재화가 부족합니다!");
		noticeText->SetColor({ 1.0f, 0.2f, 0.2f, 1.0f });

		SetSpriteIsEnable("WeaponNotice", false, weaponAltarCanvas);

#pragma endregion

		/// 종료 버튼
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

	// 모든 화면에 표시되는 디버그 UI를 모아둔 캔버스.
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

		// 기본적인 플레이어 디버그 코드 위치 이동.
		CreateUIText("PlayerState : ", "PlayrState : ", Vector(1.f, 1.f, 1.f), Vector(1120.f, 50.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("State", "IdleState", Vector(1.f, 1.f, 1.f), Vector(1200.f, 50.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("PlayerPos", "PlayerPos : ", Vector(1.f, 1.f, 1.f), Vector(100.f, 120.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("PlayerCurrentHp", "PlayerCurrentHp : ", Vector(1.f, 1.f, 0.f), Vector(1120.f, 260.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		CreateUIText("PlayerRayCast", "PlayerRay : Empty", Vector(0.f, 1.f, 1.f), Vector(100.f, 460.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIText("PlayerRayCast", "PlayerRay : Empty", debugCanvas);

		CreateUIText("LookAt", "LookAt", Vector(1.f, 1.f, 1.f), Vector(100.f, 140.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Ray", "raychec ked", Vector(1.f, 1.f, 1.f), Vector(100.f, 600.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		SetTextUIIsEnable("Ray", false, debugCanvas);

		// 보스 디버그 코드
		CreateUIText("BossState", "BossState: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 320.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("BossHp", "BossHp: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 340.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("BossPhase", "BossPhase: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 360.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);
		CreateUIText("Distance-Player", "Distance-Player: ", Vector(1.f, 1.f, 1.f), Vector(100.f, 400.f, 0.f), 0.f, Vector(1.f, 1.f, 1.f), debugCanvas);

		// 디버그 컴포넌트에 존재하던 디버그 정보
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

		// 몬스터 디버그 코드.
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

		// 대화가 뜰 장소를 미리 만들어 둔다는 느낌에 가깝다.
		// 실제로 대화 자체는 각 컴포넌트에서 문자열을 집어넣어서 만들어 지는 식으로 구현.

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
		monologue0->SetText(L"Episode VIII : 디오메데스의 야생마 생포");
		monologue0->SetEnable(false);

		//monologueCanvas->SetEnable(false);
		TextUI* monologue1 = monologueCanvas->CreateTextUI("Monologue1");
		monologue1->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue1->SetPosition({ -800.f, 0.f, 0.2f });
		monologue1->SetFont("MuseumMedium");
		monologue1->SetFontSize(20.f);
		monologue1->SetText(L" ...\n\n\n 시간이 얼마나 흐른거지.\n\n\n 아니, 사실 시간이 얼마나 흐른건지는 중요치 않다.");
		monologue1->SetEnable(false);

		TextUI* monologue2 = monologueCanvas->CreateTextUI("Monologue2");
		monologue2->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue2->SetPosition({ -800.f, 0.f, 0.2f });
		monologue2->SetFont("MuseumMedium");
		monologue2->SetFontSize(20.f);
		monologue2->SetText(L" 내가 나의 아내와 아이들을 죽인 그 죄.\n\n\n 그것만이 중요한 것일 뿐.");
		monologue2->SetEnable(false);

		TextUI* monologue3 = monologueCanvas->CreateTextUI("Monologue3");
		monologue3->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue3->SetPosition({ -800.f, 0.f, 0.2f });
		monologue3->SetFont("MuseumMedium");
		monologue3->SetFontSize(20.f);
		monologue3->SetText(L" 낭자하게 흩뿌려진 뜨거운 피 웅덩이 속.\n\n\n 차갑게 식어버린 그들을 보며 흘린 눈물이 강을 이룰만큼 깊어\n\n\n 다시는 눈물을 흘리지 못할 것이라 생각했지만...\n\n\n 표정을 잃어버린 볼 위로 또 다시 뜨거운 눈물이 흘러내렸다.");
		monologue3->SetEnable(false);

		TextUI* monologue4 = monologueCanvas->CreateTextUI("Monologue4");
		monologue4->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue4->SetPosition({ -800.f, 0.f, 0.2f });
		monologue4->SetFont("MuseumMedium");
		monologue4->SetFontSize(20.f);
		monologue4->SetText(L" 삶의 이유를 잃어버린 채\n\n\n 정신없이 이뤄낸 과업은 어느덧 7개.");
		monologue4->SetEnable(false);

		TextUI* monologue5 = monologueCanvas->CreateTextUI("Monologue5");
		monologue5->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		monologue5->SetPosition({ -800.f, 0.f, 0.2f });
		monologue5->SetFont("MuseumMedium");
		monologue5->SetFontSize(20.f);
		monologue5->SetText(L" 그들에게 조금이나마 속죄하기 위해.\n\n\n 8번째 과업을 시작한다.\n\n\n 부디, 어서 빨리 헤라의 저주에서 벗어나 그들의 곁으로 갈 수 있길.");
		monologue5->SetEnable(false);

		monologueCanvas->SetEnable(false);
	}

	void UIManager::CreateDialogCanvas()
	{
		Canvas* dialogCanvas = GetCanvas("DialogCanvas");
		dialogCanvas->SetEnable(false);

		Factory* factory = graphicsInstance->GetRenderer()->CreateFactory();
		ResourceManager* resourceManager = graphicsInstance->GetRenderer()->GetResourceManager();

		// 대화 배경
		TextureUI* dialogBG = dialogCanvas->CreateTextureUI("DialogBG");

		TextureBase* BGTex = resourceManager->GetTexture("DialogBG");
		if (BGTex == nullptr)
			BGTex = factory->CreateTexture("DialogBG", "Resources/UI/talk.png");

		dialogBG->SetTexture(BGTex);
		dialogBG->SetWidth(1000.f);
		dialogBG->SetHeight(300.f);
		dialogBG->SetPosition({ 150.0f, 350.f, 0.15f });

		// 기본적인 포인터. => 대화창에서는 위치 고정.
		TextureUI* dialogPointer = dialogCanvas->CreateTextureUI("DialogPointer");

		TextureBase* pointerTex = resourceManager->GetTexture("Cursor");
		if (pointerTex == nullptr)
			pointerTex = factory->CreateTexture("Cursor", "Resources/UI/cursor-08.png");

		dialogPointer->SetTexture(pointerTex);
		dialogPointer->SetWidth(50.f);
		dialogPointer->SetHeight(50.f);
		dialogPointer->SetPosition({ 550.0f, 430.f, 0.1f });

		dialogPointer->SetEnable(false);

		// 대화 이미지
		TextureUI* dialogImage = dialogCanvas->CreateTextureUI("DialogHeracles");

		TextureBase* heraclesTex = resourceManager->GetTexture("DialogHeracles");
		if (heraclesTex == nullptr)
			heraclesTex = factory->CreateTexture("DialogHeracles", "Resources/UI/Dialog_Heracles.png");

		dialogImage->SetTexture(heraclesTex);
		dialogImage->SetWidth(450.f);
		dialogImage->SetHeight(450.f);
		dialogImage->SetPosition({ -450.f, 300.f, 0.05f });

		// 하나로 돌려 쓸 대화 텍스트
		TextUI* dialogText = dialogCanvas->CreateTextUI("DialogText");
		dialogText->SetPivot({ HorizontalLocation::LEFT, VerticalLocation::MIDDLE });
		dialogText->SetPosition({ -200.f, 300.f, 0.13f });
		dialogText->SetFont("MuseumMedium");
		dialogText->SetFontSize(22.f);
		dialogText->SetText(L" 몸이 아주 단단해졌어. 이정도면 스켈레톤들의 공격에도 끄떡 없겠군");

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
		restartText->SetText(L"재시작");

		// 텍스쳐 재활용.
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

				// 여기서 메인 메뉴 로드 씬..
				SceneManager::GetInstance()->LoadScene(TEXT("MainMenu"));
			});

		ending->SetEnable(false);
	}
}
