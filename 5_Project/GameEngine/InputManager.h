#pragma once
#include "XPadController.h"

#include "FootEngineTypedef.h"

namespace GameEngineSpace
{
	using namespace HeraclesMath;

	enum class KeyState
	{
		UP,
		DOWN,
		STAY,
		TOGGLE
	};

	/**
	 * \brief 싱글톤으로 모든 인풋을 관리하는 인풋 매니저.
	 *
	 * 2022.08.01 Dev.BlackFoot
	 */
	class InputManager final
	{
	private:
		// Key
		std::map<int, bool> prevKeyState;
		std::map<int, bool> currKeyState;

		// Cursor Coordinate
		Vector prevCursorXY;
		Vector currCursorXY;

		// XboxPad 컨트롤러
		std::shared_ptr<XPadController> gamePad;

		// inputManger 스태틱 변수
		static std::shared_ptr<InputManager> instance;

	public:
		// 윈도우 활성화 관련 Input 거름망
		bool isMouseInWindow;
		bool isWindowActivated;

	public:
		InputManager();
		~InputManager();
		// Key Input
		void Update();
		void LateUpdate();
		virtual bool GetInputState(int key, KeyState state);

		// Pad Input
		void PadUpdate(float tick);

		// Pad Get
		inline std::shared_ptr<XPadController> GetGamePad() { return gamePad; }

		// Mouse Pos
		void SetMousePos(float x, float y);
		const Vector& GetMousePos();
		Vector GetMouseMoveVector();

		static std::shared_ptr<InputManager> GetInstance();
	};
}
