#pragma once

namespace Heracles
{
	class InventoryUI : public ComponentBase
	{
	private:

		std::string currStateId = "InventoryBase";
		std::string triggeredButton = "None";

		bool isOpen = false;

		std::vector<std::string> stateKeyList;
		std::vector<std::string> buttonList;

		// �κ��丮 ĵ���� ĳ��
		GraphicsEngineSpace::Canvas* inventory;
		// ��ư �ϳ� ĳ��
		GraphicsEngineSpace::TextureUI* nowClickedSprite;

	public:
		InventoryUI(std::weak_ptr<GameObject> obj);

		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update(float tick) override;

		inline bool CheckIsActivated() { return this->isOpen; }

	private:
		void OnOffInventory(bool on);
		void ActivateButtons(bool b);
		void CheckState();
	};
}
