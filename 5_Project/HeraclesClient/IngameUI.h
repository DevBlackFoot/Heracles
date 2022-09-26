#pragma once

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		using GraphicsEngineSpace::TextureBase;

		constexpr int CurseCount = 3;

		class InGameUI : public ComponentBase
		{
			static GraphicsEngineSpace::Canvas*	canvas;
			GraphicsEngineSpace::ProgressBar*	hpBar;
			GraphicsEngineSpace::ButtonUI*		curse[CurseCount];
			GraphicsEngineSpace::TextureUI*		weaponSlot;
			GraphicsEngineSpace::TextUI*		memoryFrags;
			GraphicsEngineSpace::ProgressBar*	bossHPBar;
			GraphicsEngineSpace::TextureUI*		bossBackUI;

			int currentHP;
			int maxHP;

			int currentBossHP;
			int bossMaxHP;
			
		public:
			InGameUI(std::weak_ptr<GameObject> obj);
			~InGameUI();

			void SetHP(int HP);
			void SetMaxHP(int maxHP);

			void SetBossHP(int HP);
			void SetBossMaxHP(int maxHP);

			void SetCurse(const std::string& curseName);
			void SetOffCurse(const std::string& curseName);
			void SetMemories(int memories);
			void SetEquipment(const std::string& weaponName);
			void SetSwitching(const std::string& weaponName);
			void Swap();
			void SetBossHPBarEnable(bool isEnable);

			void Awake() override;
			void Start() override;
			void PreUpdate(float tick) override;
			void Update(float tick) override;
			void LateUpdate(float tick) override;

		private:
			void CreateHpBar(TextureBase* frontBar, TextureBase* backBar, TextureBase* bossBack);
			void CreateCurse(TextureBase* curseIconSlot);
			void CreateWeaponUI(TextureBase* weaponSlotTexture);
			void CreateMemoryFragment(TextureBase* memoryIcon);
		};
	}
}