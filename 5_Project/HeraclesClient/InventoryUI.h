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

		// 인벤토리 캔버스 캐싱
		GraphicsEngineSpace::Canvas* inventory;
		// 버튼 하나 캐싱
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
