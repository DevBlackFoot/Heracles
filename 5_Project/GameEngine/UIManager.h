#pragma once

#include "../GraphicsEngine/GraphicsEngineFramework.h"

namespace GameEngineSpace
{
	class GraphicsManager;
	class InputManager;
	class SoundManager;

	using GraphicsEngineSpace::ButtonState;
	using GraphicsEngineSpace::Canvas;
	using GraphicsEngineSpace::UIBase;
	using GraphicsEngineSpace::VerticalLocation;
	using GraphicsEngineSpace::HorizontalLocation;
	
	/**
	 * \brief UI, �ؽ�Ʈ �� ȭ�鿡 2�������� �����ϴ� UI �鸸�� ������ �ϴ� UI Manager Ŭ����
	 *
	 * 2022.08.08(��) Deb.BlackFoot
	 */
	class UIManager final
	{
	private:
		// UI Manager�� �ν��Ͻ�
		static std::shared_ptr<UIManager> instance;

		// ���������� �׷��Ƚ� �Ŵ��� ĳ��
		std::shared_ptr<GraphicsManager> graphicsInstance;
		// ���������� ��ǲ �Ŵ��� ĳ��
		std::shared_ptr<InputManager> inputInstance;
		// ���������� soundManager ĳ��
		std::shared_ptr<SoundManager> soundInstance;

		//// ȭ�鿡 �׷����� ��� Text������ ��� �ִ� ��(����� ������ ����)
		//std::map<std::string, std::shared_ptr<UIText>> UITextInScene;

		//// ȭ�鿡 �׷����� ���
		//std::map<std::string, std::shared_ptr<UISprite>> UISpriteInScene;

		// ĵ����
			// ���ӿ� �ִ� ��� UI ĵ�������� init�� ���� �� ���� ������.
			// UI�� �����Ǵ� ��� �༮���� ó�� Init �� �� ����� enable�� false�� �������ְ�
			// Ư�� ��Ȳ�� �ʿ��� �� �ش� UI ���̵� �����ͼ� ������ش�.
		Canvas* mainCanvas;

		// ȭ�� ������ �ľ��ϰ�. �������� �÷�����..
		uint32 nowWidth;
		uint32 nowHeight;

	public:
		UIManager() = default;
		~UIManager() = default;
		UIManager(const UIManager& other) = delete;
		UIManager(const UIManager&& other) = delete;
		UIManager& operator= (const UIManager& other) = delete;
		UIManager& operator= (const UIManager&& other) = delete;

		static std::shared_ptr<UIManager> GetInstance();

		void Init();

		// WIC �̽��� ���ϱ� ���� Canvas ����
		void CreateCanvasAll();

		// ����
		void Render();

		// ��ư �Է��� ���� ������Ʈ�� LateUpdate
		void Update();
		void LateUpdate();

		void Release();

		Canvas* CreateCanvas(const std::string& UIIDstr);

		// UIText
		void CreateUIText(const std::string& UIIDstr, const std::string& text, Vector color, Vector position, float rotation, Vector scale, Canvas* parentCanvas = nullptr);

		void SetTextUIText(const std::string& UIIDstr, const std::string& text, Canvas* parentCanvas = nullptr);
		void SetTextUIColor(const std::string& UIIDstr, Vector color, Canvas* parentCanvas = nullptr);
		void SetTextUIPosition(const std::string& UIIDstr, Vector position, Canvas* parentCanvas = nullptr);
		void SetTextUIScale(const std::string& UIIDstr, Vector scale, Canvas* parentCanvas = nullptr);
		void SetTextUIRotation(const std::string& UIIDstr, float rotation, Canvas* parentCanvas = nullptr);
		void SetTextUIIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);

		void SetTextUIAnchor(const std::string& UIIDstr, VerticalLocation vertical, HorizontalLocation horizontal, Canvas* parentCanvas = nullptr);

		// UISprite
		// �׸� �ϳ�.
		void CreateUISprite(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);
		// ��ư �� �׸� ���� �ѹ��� �ޱ�..
		void CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);
		void CreateUIButton(const std::string& UIIDstr, const std::string& noneTextureName, const std::string& noneTexturePath,
			const std::string& hoverTextureName, const std::string& hoverTexturePath, const std::string& downTextureName, const std::string& downTexturePath,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);

		UIBase* CreateProgressUI(const std::string& UIIDstr, const std::string& frontTextureName, const std::string& fontTexturePath,
			RECT frontSrc, const std::string& backTextureName, const std::string& backTexturePath, RECT backSrc,
			long posX, long posY, long width, long height, float z, Canvas* parentCanvas = nullptr);

		// ���� ����.
		void SetSpriteTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpritePosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas = nullptr);
		void SetSpriteSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas = nullptr);
		void SetSpriteIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);

		void SetSpriteNoneTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpriteHoverTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetSpriteDownTexture(const std::string& UIIDstr, const std::string& textureName, const std::string& texturePath, Canvas* parentCanvas = nullptr);
		void SetButtonIsEnable(const std::string& UIIDstr, bool isEnable, Canvas* parentCanvas = nullptr);
		void SetButtonPosition(const std::string& UIIDstr, long posX, long posY, Canvas* parentCanvas = nullptr);
		void SetButtonSize(const std::string& UIIDstr, long width, long height, Canvas* parentCanvas = nullptr);
	
		// Ư�� ��ư�� ���¸� �˰� ���� ��..
		ButtonState GetButtonState(const std::string& UIIDstr, Canvas* parentCanvas = nullptr);

		// Ư�� ĵ������ ��� ���� ��
		Canvas* GetCanvas(const std::string& canvasIDStr);

		// ĵ���� ��������
		void OnResize(uint32 width, uint32 height);

		void ClearUI();

	private:
		
		// ���� ĵ���� ����
		void CreateInventory();
		//void CreateStartAltar();		=> ������ �ɰ����ϴ�.
		void CreateCurseAltar();
		void CreateEnhanceAltar();

		void CreateWeaponAltar();
		void CreateDebugCanvas();
		void CreateMonologueCanvas();
		void CreateDialogCanvas();
		void CreateTutorialCanvas();
		void CreateEndingCanvas();
	};

}
