/********************************
*								*
*	Button UI.h					*
*								*
*	Created : 2022/08/10		*
*	Updated : 2022/08/20		*
*********************************/

#pragma once

#include <functional>
#include "Bases/UIBase.h"
#include "Bases/TextureBase.h"
#include "Bases/GraphicsEngineBase.h"

namespace GraphicsEngineSpace
{
	class Canvas;

	enum class ButtonState
	{
		DEFAULT,
		DOWN,
		UP,
		PRESS,
		HOVER,
		ENTER,		// ó�� hover ���� ��.
	};

	class ButtonUI : public UIBase
	{
		TextureBase* defaultTexture;
		TextureBase* hoverTexture;
		TextureBase* pressTexture;
		ButtonState buttonState;

		std::function<void(void)> clickEvent;
		std::function<void(void)> pressEvent;
		std::function<void(void)> enterEvent;
		std::function<void(void)> hoverEvent;
		std::function<void(void)> hoverOutEvent;

	protected:
		ButtonUI();
		~ButtonUI() override;

	public:
		virtual void SetDefaultTexture(TextureBase* texture);
		virtual void SetHoverTexture(TextureBase* texture);
		virtual void SetPressTexture(TextureBase* texture);
		virtual void SetButtonState(ButtonState state);
		virtual void SetClickEvent(std::function<void(void)> clickEvent);
		virtual void SetEnterEvent(std::function<void(void)> enterEvent);
		virtual void SetHoverEvent(std::function<void(void)> hoverEvent);
		virtual void SetHoverOutEvent(std::function<void(void)> hoverOutEvent);
		virtual void SetPressEvent(std::function<void(void)> pressEvent);

		virtual ButtonState GetButtonState();

		void Render(GraphicsEngineBase* engine, float tick) override;

		friend Canvas;
	};
}