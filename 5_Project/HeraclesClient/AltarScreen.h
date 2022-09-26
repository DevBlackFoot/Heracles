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

		// 이 상태에 따라 ui텍스쳐 변경
		enum class ScreenState
		{
			// 시작 지점 제단 하나 선택
			START,
			REMOVE_CURSE,
			ENHANCE,
			DISABLE, // 제단 매쉬만 보이는 상태 , 나머지는 ui 텍스쳐가 보인다
			WEAPON,
			NONE
		};

		class ScreenStateBase abstract
		{
		protected:
			ScreenState tag = ScreenState::DISABLE;
			bool running = false;

			// UI를 관리할 캔버스
			Canvas* curseAltar;
			Canvas* enhanceAltar;
			Canvas* debug;

			// UI Manager자체를 캐싱
			std::shared_ptr<UIManager> ui;

			// Sound Manager 자체를 캐싱
			std::shared_ptr<SoundManager> sound;

			// 플레이어 정보 자체를 캐싱.
			std::shared_ptr<Player::PlayerStateMachine> playerState;

			// 틱을 잽시다.
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
			
			// 각 버튼 스트링
			std::string buttonName[6];

			// 각 업그레이드 불 변수
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
			// 여기서만 관리할 웨폰 캔버스
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
