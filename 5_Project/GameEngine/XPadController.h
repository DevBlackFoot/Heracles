#pragma once
#include <XInput.h>

using namespace std;

namespace GameEngineSpace
{
	enum class XPadInput
	{
		Up,
		Down,
		Left,
		Right,
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY,
		Start,
		Back,
		XInputKeyMax
	};

	enum class XPadCommand
	{
		Release = 1,
		KeepPress = 2,
		Push = 3
	};

	/// <summary>
	/// XInput 패드 입력 핸들 클래스
	/// 2022. 08. 18. 작성자 Almond
	/// </summary>
	class XPadController
	{
	private:

		// 컨트롤러 체크용 동적배열변수. index 0, 1, 2.. = 1p, 2p, 3p...
		vector<vector<unsigned char>> m_curKeyStates;
		vector<vector<unsigned char>> m_oldKeyStates;
		vector<vector<unsigned char>> m_commandStates;

		// 컨트롤러 스틱 이동 좌표
		vector<SHORT> m_curStickXPoint;
		vector<SHORT> m_curStickYPoint;
		vector<float> m_curnormalMagnitude;

		// 상수
		int							  m_userMaxCount;
		float						  m_tickChecker;
		float						  m_updateTime;

	public:
		XPadController();
		~XPadController();
		void UpdateXPadStates(float tick);
		bool IsPadStickMove(XPadInput input, int player = 0);
		bool IsPadCommand(XPadInput input, XPadCommand inputState, int player = 0);
		bool IsAnyButtonCommand(XPadCommand inputState, int player = 0);
		void StartVibration(WORD leftMotorSpeed = 32000, WORD rightMotorSpeed = 32000, int player = 0);
		void StopVibration(int player = 0);

		Vector GetStickDirection(int player = 0);
		float GetStickMagnitude(int player = 0) { return m_curnormalMagnitude[player]; }


	private:
		static float CreateNormalizedMagnitude(XINPUT_STATE& state);
		static void CheckMovement(XINPUT_STATE& state, float& normalMagnitude, vector<unsigned char>& curkeyState, SHORT& stickX, SHORT& stickY);
		static void CheckButton(XINPUT_STATE& state, vector<unsigned char>& curkeyState);
		static void CheckCommand(XINPUT_STATE& state,
			vector<unsigned char>& curkeyState, vector<unsigned char>& oldkeyState, vector<unsigned char>& commandState);
		static void SetZeroPadState(vector<unsigned char>& state);
		static void CopyPadState(vector<unsigned char>& source, vector<unsigned char>& target);


	};
}
