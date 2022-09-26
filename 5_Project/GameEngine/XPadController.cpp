#include "pch.h"
#include "XPadController.h"

using namespace GameEngineSpace;

XPadController::XPadController()
{	
	m_userMaxCount = 1;	// 일단 1인용임 바꾸고싶으면 생성자 매개변수로 빼시던지..	
	m_oldKeyStates.resize(m_userMaxCount);
	m_curKeyStates.resize(m_userMaxCount);
	m_commandStates.resize(m_userMaxCount);
	m_curStickXPoint.resize(m_userMaxCount);
	m_curStickYPoint.resize(m_userMaxCount);
	m_curnormalMagnitude.resize(m_userMaxCount);

	// XInput 컨트롤러 키 입력 관련 중간변수 초기화
	vector<unsigned char> inputkey_ZeroContainer;
	int inputkeyNumber = (int)XPadInput::XInputKeyMax;
	inputkey_ZeroContainer.resize(inputkeyNumber);
	for (int key = 0; key < inputkeyNumber; key++)
	{
		inputkey_ZeroContainer[key] = 0;
	}
	for (int pl = 0; pl < m_userMaxCount; pl++)
	{
		m_oldKeyStates[pl] = inputkey_ZeroContainer;
		m_curKeyStates[pl] = inputkey_ZeroContainer;
		m_commandStates[pl] = inputkey_ZeroContainer;
		m_curStickXPoint[pl] = 0;
		m_curStickYPoint[pl] = 0;
		m_curnormalMagnitude[pl] = 0.0f;
	}
	m_tickChecker = 0.0f;
	m_updateTime = 0.0f;
}

XPadController::~XPadController()
{
	for (auto& keyStates : m_curKeyStates)
	{
		keyStates.clear();
	}

	for (auto& keyStates : m_oldKeyStates)
	{
		keyStates.clear();
	}

	for (auto& keyStates : m_commandStates)
	{
		keyStates.clear();
	}
	
	m_curKeyStates.clear();
	m_oldKeyStates.clear();
	m_commandStates.clear();

}

void XPadController::UpdateXPadStates(float tick)
{
	//m_tickChecker += tick;
	//if (m_tickChecker < m_updateTime)
	//	return;

	for (int pl = 0; pl < m_userMaxCount; pl++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		if (XInputGetState(pl, &state) != ERROR_SUCCESS)
			return;

		CopyPadState(m_curKeyStates[pl], m_oldKeyStates[pl]);
		SetZeroPadState(m_curKeyStates[pl]);
		SetZeroPadState(m_commandStates[pl]);

		m_curnormalMagnitude[pl] = CreateNormalizedMagnitude(state);

		// 패드 입력 체크
		CheckMovement(state, m_curnormalMagnitude[pl], m_curKeyStates[pl], m_curStickXPoint[pl], m_curStickYPoint[pl]);
		CheckButton(state, m_curKeyStates[pl]);
		CheckCommand(state, m_curKeyStates[pl], m_oldKeyStates[pl], m_commandStates[pl]);
	}
	//m_tickChecker = 0.0f;
	//m_updateTime = tick;
}

// padstate를 사이즈를 변경하지 않고, 사이즈 내 변수들을 0으로 초기화해준다.
void XPadController::SetZeroPadState(vector<unsigned char>& state)
{
	fill(state.begin(), state.end(), 0);
}

// padstate를 사이즈를 변경하지 않고, 소스 백터 내 변수들을 target에 복사해준다.
void XPadController::CopyPadState(vector<unsigned char>& source, vector<unsigned char>& target)
{
	SetZeroPadState(target);
	copy(source.begin(), source.end(), target.begin());
}

// 아날로그 스틱 데드존에 따른 normalized magnitude 반환
float XPadController::CreateNormalizedMagnitude(XINPUT_STATE& state)
{
	float LX = state.Gamepad.sThumbLX;
	float LY = state.Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float magnitude = sqrt(LX * LX + LY * LY);

	//determine the direction the controller is pushed
	float normalizedLX = LX / magnitude;
	float normalizedLY = LY / magnitude;

	float normalizedMagnitude = 0;

	float inputDeadZone = 25000;

	//check if the controller is outside a circular dead zone
	if (magnitude > inputDeadZone)
	{
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= inputDeadZone;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - inputDeadZone);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}
	return normalizedMagnitude;
}

// 이동체크, 이동계 입력에 들어오면 중간변수에 체크해줌.
void XPadController::CheckMovement(XINPUT_STATE& state, float& normalMagnitude, vector<unsigned char>& curkeyState, SHORT& stickX, SHORT& stickY)
{
	stickX = state.Gamepad.sThumbLX;
	stickY = state.Gamepad.sThumbLY;
	double radian = atan2((double)stickY,(double)stickX);
	float stickdegree = (float)radian * 57.32f;
	if (stickdegree > 360.0f)
		stickdegree -= 360.0f;

	// Up
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP
		|| (state.Gamepad.sThumbLY > 0 && normalMagnitude > 0.99f && 60.0f < stickdegree < 150.0f ))
		curkeyState[(int)XPadInput::Up] = 1;
	else
		curkeyState[(int)XPadInput::Up] = 0;
	// Down
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN
		|| (state.Gamepad.sThumbLY < 0 && normalMagnitude > 0.99f && 200.0f < stickdegree < 320.0f))
		curkeyState[(int)XPadInput::Down] = 1;
	else
		curkeyState[(int)XPadInput::Down] = 0;
	// Left
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT
		|| (state.Gamepad.sThumbLX < 0 && normalMagnitude > 0.99f && 120.0f < stickdegree < 230.0f))
		curkeyState[(int)XPadInput::Left] = 1;
	else
		curkeyState[(int)XPadInput::Left] = 0;
	// Right
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT
		|| (state.Gamepad.sThumbLX > 0 && normalMagnitude > 0.99f) && (stickdegree < 30.0f || stickdegree > 320.0f))
		curkeyState[(int)XPadInput::Right] = 1;
	else
		curkeyState[(int)XPadInput::Right] = 0;
}

Vector XPadController::GetStickDirection(int player)
{
	return m_curStickXPoint[player] == 0 && m_curStickYPoint[player] == 0 ?
		Vector::Zero : Vector3Normalize(Vector((float)m_curStickXPoint[player], (float)m_curStickYPoint[player]));

}

// 버튼체크, 버튼계 입력이 들어오면 중간변수에 체크해줌. LR은 안했음. 필요하면 추가.
void XPadController::CheckButton(XINPUT_STATE& state, vector<unsigned char>& curkeyState)
{
	// A버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		curkeyState[(int)XPadInput::ButtonA] = 1;
	else
		curkeyState[(int)XPadInput::ButtonA] = 0;
	// B버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		curkeyState[(int)XPadInput::ButtonB] = 1;
	else
		curkeyState[(int)XPadInput::ButtonB] = 0;
	// X버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		curkeyState[(int)XPadInput::ButtonX] = 1;
	else
		curkeyState[(int)XPadInput::ButtonX] = 0;
	// Y버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		curkeyState[(int)XPadInput::ButtonY] = 1;
	else
		curkeyState[(int)XPadInput::ButtonY] = 0;
	// Start버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		curkeyState[(int)XPadInput::Start] = 1;
	else
		curkeyState[(int)XPadInput::Start] = 0;
	// Back버튼
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
		curkeyState[(int)XPadInput::Back] = 1;
	else
		curkeyState[(int)XPadInput::Back] = 0;
}

void XPadController::CheckCommand(XINPUT_STATE& state,
	vector<unsigned char>& curkeyState, vector<unsigned char>& oldkeyState, vector<unsigned char>& commandState)
{
	for (int key = 0; key < (int)XPadInput::XInputKeyMax; key++)
	{
		if (curkeyState[key] && oldkeyState[key])			// 예나 지금이나 키가 눌렸으면.
			commandState[key] = (int)XPadCommand::KeepPress;
		else if (curkeyState[key] && !(oldkeyState[key]))	// 예전엔 안눌렸었는데, 현재 눌렸으면.
			commandState[key] = (int)XPadCommand::Push;
		else if (!(curkeyState[key]) && oldkeyState[key])	// 예전엔 눌렀는데, 지금 안눌렀으면.
			commandState[key] = (int)XPadCommand::Release;
		else
			commandState[key] = 0;
	}
}

// 패드에 입력이 있는지 체크.
// XPadInput = 스틱, 패드, 버튼이 어떤것인지 지정
// XpadCommand = 어떤 입력행동인지.. Push는 누른거, KeepPress는 누르고 있는거, Release는 뗀거
bool XPadController::IsPadCommand(XPadInput input, XPadCommand inputState, int player)
{
	return (m_commandStates[player][(int)input] == (int)inputState) ? true : false;
}

bool XPadController::IsPadStickMove(XPadInput input, int player)
{
	return (bool)m_curKeyStates[player][(int)input];
}

// 전체버튼의 입력 체크. 어떤 버튼이라도, XpadCommand의 행동을 했으면 true 리턴
bool XPadController::IsAnyButtonCommand(XPadCommand inputState, int player)
{
	for (int button = (int)XPadInput::ButtonA; button < (int)XPadInput::XInputKeyMax; button++)
	{
		if (m_commandStates[player][button] == (int)inputState)
			return true;
	}
	return false;
}

// 진동 시작
void XPadController::StartVibration(WORD leftMotorSpeed, WORD rightMotorSpeed, int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorSpeed; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = rightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}

// 진동 끝
void XPadController::StopVibration(int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 0; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}