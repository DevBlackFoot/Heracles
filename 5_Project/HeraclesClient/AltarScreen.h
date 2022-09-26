#pragma once

#include "GameState.h"
#include "UIManager.h"
#include "PlayerStateMachine.h"
#include "SoundManager.h"

namespace Heracles
{
	namespace Altar
	{
		class AltarState;
		class WeaponAltarState;

		// �� ���¿� ���� ui�ؽ��� ����
		enum class ScreenState
		{
			// ���� ���� ���� �ϳ� ����
			START,
			REMOVE_CURSE,
			ENHANCE,
			DISABLE, // ���� �Ž��� ���̴� ���� , �������� ui �ؽ��İ� ���δ�
			WEAPON,
			NONE
		};

		class ScreenStateBase abstract
		{
		protected:
			ScreenState tag = ScreenState::DISABLE;
			bool running = false;

			// UI�� ������ ĵ����
			Canvas* curseAltar;
			Canvas* enhanceAltar;
			Canvas* debug;

			// UI Manager��ü�� ĳ��
			std::shared_ptr<UIManager> ui;

			// Sound Manager ��ü�� ĳ��
			std::shared_ptr<SoundManager> sound;

			// �÷��̾� ���� ��ü�� ĳ��.
			std::shared_ptr<Player::PlayerStateMachine> playerState;

			// ƽ�� ��ô�.
			bool isNotice;
			float noticeTickCount;

		public:
			ScreenStateBase(ScreenState state)
			{
				this->tag = state;
				curseAltar = UIManager::GetInstance()->GetCanvas("CurseAltar");
				enhanceAltar = UIManager::GetInstance()->GetCanvas("EnhanceAltar");
				debug = UIManager::GetInstance()->GetCanvas("DebugCanvas");
				ui = UIManager::GetInstance();
				sound = SoundManager::GetInstance();
				playerState = GameState::GetInstance()->GetPlayer()->GetComponent<Player::PlayerStateMachine>();
				isNotice = false;
				noticeTickCount = 0.0f;
			}
			virtual void EventCheck(AltarState* altar) = 0;
			virtual void EventCheck(WeaponAltarState* weaponAltar) = 0;

		private:
			virtual void Enter(AltarState* altar) = 0;
			virtual void Enter(WeaponAltarState* weaponAltar) = 0;
			virtual void Exit(AltarState* altar, ScreenState nextState) = 0;
			virtual void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) = 0;
		};

		class DisableState : public ScreenStateBase
		{
		public:
			DisableState();
			virtual void EventCheck(AltarState* altar) override;
			void EventCheck(WeaponAltarState* weaponAltar) override;

		private:
			virtual void Enter(AltarState* altar) override;
			void Enter(WeaponAltarState* weaponAltar) override;

			virtual void Exit(AltarState* altar, ScreenState nextState) override;
			void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) override;
		};

		class EnhanceScreen : public ScreenStateBase
		{
		public:
			EnhanceScreen();
			virtual void EventCheck(AltarState* altar) override;
			void EventCheck(WeaponAltarState* weaponAltar) override;

		private:
			virtual void Enter(AltarState* altar) override;
			void Enter(WeaponAltarState* weaponAltar) override;
			virtual void Exit(AltarState* altar, ScreenState nextState) override;
			void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) override;

			void SetAllButtonEnable(bool enable);
			
			// �� ��ư ��Ʈ��
			std::string buttonName[6];

			// �� ���׷��̵� �� ����
			int isEnhanced[3];
		};

		class RemoveCurseScreen : public ScreenStateBase
		{
		public:
			RemoveCurseScreen();
			virtual void EventCheck(AltarState* altar) override;
			void EventCheck(WeaponAltarState* weaponAltar) override;

		private:
			virtual void Enter(AltarState* altar) override;
			void Enter(WeaponAltarState* weaponAltar) override;
			virtual void Exit(AltarState* altar, ScreenState nextState) override;
			void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) override;

			void SetAllButtonEnable(bool enable);
		};

		class NoneState : public ScreenStateBase
		{
		public:
			NoneState();
			virtual void EventCheck(AltarState* altar) override;
			void EventCheck(WeaponAltarState* weaponAltar) override;

		private:
			virtual void Enter(AltarState* altar) override;
			void Enter(WeaponAltarState* weaponAltar) override;
			virtual void Exit(AltarState* altar, ScreenState nextState) override;
			void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) override;
		};

		class WeaponScreen : public ScreenStateBase
		{
		private:
			// ���⼭�� ������ ���� ĵ����
			Canvas* weaponAltarCanvas;

		public:
			WeaponScreen();
			virtual void EventCheck(AltarState* altar) override;
			void EventCheck(WeaponAltarState* weaponAltar) override;

		private:
			virtual void Enter(AltarState* altar) override;
			void Enter(WeaponAltarState* weaponAltar) override;
			virtual void Exit(AltarState* altar, ScreenState nextState) override;
			void Exit(WeaponAltarState* weaponAltar, ScreenState nextState) override;

			void SetAllButtonEnable(bool enable);

			bool isUpgrade;
		};

	}
}
