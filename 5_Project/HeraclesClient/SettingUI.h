#pragma once

#include "SoundManager.h"

namespace Heracles
{
	namespace UI
	{
		class Canvas;
		using GraphicsEngineSpace::TextureBase;
		using GraphicsEngineSpace::ProgressBar;
		using GameEngineSpace::PlayMode;

		class SettingUI : public ComponentBase
		{
			static GraphicsEngineSpace::Canvas* canvas;
			GraphicsEngineSpace::ProgressBar*	bgmVolumeBar;
			GraphicsEngineSpace::ProgressBar*	effectVolumeBar;
			GraphicsEngineSpace::ButtonUI*		backButton;
			GraphicsEngineSpace::ButtonUI*		closeButton;

		public:
			SettingUI(std::weak_ptr<GameObject> obj);
			~SettingUI();

			void SetBackButtonEnable(bool isEnable);

			void Awake() override;
			void Start() override;
			void PreUpdate(float tick) override;
			void Update(float tick) override;
			void LateUpdate(float tick) override;

		private:
			void CreateBoard(TextureBase* boardTexture);
			void CreateVolumeBar(ProgressBar* bar, TextureBase* icon, TextureBase* frontBar, TextureBase* backBar, TextureBase* cursor, PlayMode mode);
			void CreateBackToMainButton(TextureBase* hoverTexture);
			void CreateCloseButton(TextureBase* backGround, TextureBase* closeTexture);
		};
	}
}