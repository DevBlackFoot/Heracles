#include "pch.h"
#include "XPadController.h"

using namespace GameEngineSpace;

XPadController::XPadController()
{	
	m_userMaxCount = 1;	// �ϴ� 1�ο��� �ٲٰ������ ������ �Ű������� ���ô���..	
	m_oldKeyStates.resize(m_userMaxCount);
	m_curKeyStates.resize(m_userMaxCount);
	m_commandStates.resize(m_userMaxCount);
	m_curStickXPoint.resize(m_userMaxCount);
	m_curStickYPoint.resize(m_userMaxCount);
	m_curnormalMagnitude.resize(m_userMaxCount);

	// XInput ��Ʈ�ѷ� Ű �Է� ���� �߰����� �ʱ�ȭ
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

		// �е� �Է� üũ
		CheckMovement(state, m_curnormalMagnitude[pl], m_curKeyStates[pl], m_curStickXPoint[pl], m_curStickYPoint[pl]);
		CheckButton(state, m_curKeyStates[pl]);
		CheckCommand(state, m_curKeyStates[pl], m_oldKeyStates[pl], m_commandStates[pl]);
	}
	//m_tickChecker = 0.0f;
	//m_updateTime = tick;
}

// padstate�� ����� �������� �ʰ�, ������ �� �������� 0���� �ʱ�ȭ���ش�.
void XPadController::SetZeroPadState(vector<unsigned char>& state)
{
	fill(state.begin(), state.end(), 0);
}

// padstate�� ����� �������� �ʰ�, �ҽ� ���� �� �������� target�� �������ش�.
void XPadController::CopyPadState(vector<unsigned char>& source, vector<unsigned char>& target)
{
	SetZeroPadState(target);
	copy(source.begin(), source.end(), target.begin());
}

// �Ƴ��α� ��ƽ �������� ���� normalized magnitude ��ȯ
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

// �̵�üũ, �̵��� �Է¿� ������ �߰������� üũ����.
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

// ��ưüũ, ��ư�� �Է��� ������ �߰������� üũ����. LR�� ������. �ʿ��ϸ� �߰�.
void XPadController::CheckButton(XINPUT_STATE& state, vector<unsigned char>& curkeyState)
{
	// A��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
		curkeyState[(int)XPadInput::ButtonA] = 1;
	else
		curkeyState[(int)XPadInput::ButtonA] = 0;
	// B��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
		curkeyState[(int)XPadInput::ButtonB] = 1;
	else
		curkeyState[(int)XPadInput::ButtonB] = 0;
	// X��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
		curkeyState[(int)XPadInput::ButtonX] = 1;
	else
		curkeyState[(int)XPadInput::ButtonX] = 0;
	// Y��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		curkeyState[(int)XPadInput::ButtonY] = 1;
	else
		curkeyState[(int)XPadInput::ButtonY] = 0;
	// Start��ư
	if (state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
		curkeyState[(int)XPadInput::Start] = 1;
	else
		curkeyState[(int)XPadInput::Start] = 0;
	// Back��ư
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
		if (curkeyState[key] && oldkeyState[key])			// ���� �����̳� Ű�� ��������.
			commandState[key] = (int)XPadCommand::KeepPress;
		else if (curkeyState[key] && !(oldkeyState[key]))	// ������ �ȴ��Ⱦ��µ�, ���� ��������.
			commandState[key] = (int)XPadCommand::Push;
		else if (!(curkeyState[key]) && oldkeyState[key])	// ������ �����µ�, ���� �ȴ�������.
			commandState[key] = (int)XPadCommand::Release;
		else
			commandState[key] = 0;
	}
}

// �е忡 �Է��� �ִ��� üũ.
// XPadInput = ��ƽ, �е�, ��ư�� ������� ����
// XpadCommand = � �Է��ൿ����.. Push�� ������, KeepPress�� ������ �ִ°�, Release�� ����
bool XPadController::IsPadCommand(XPadInput input, XPadCommand inputState, int player)
{
	return (m_commandStates[player][(int)input] == (int)inputState) ? true : false;
}

bool XPadController::IsPadStickMove(XPadInput input, int player)
{
	return (bool)m_curKeyStates[player][(int)input];
}

// ��ü��ư�� �Է� üũ. � ��ư�̶�, XpadCommand�� �ൿ�� ������ true ����
bool XPadController::IsAnyButtonCommand(XPadCommand inputState, int player)
{
	for (int button = (int)XPadInput::ButtonA; button < (int)XPadInput::XInputKeyMax; button++)
	{
		if (m_commandStates[player][button] == (int)inputState)
			return true;
	}
	return false;
}

// ���� ����
void XPadController::StartVibration(WORD leftMotorSpeed, WORD rightMotorSpeed, int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorSpeed; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = rightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}

// ���� ��
void XPadController::StopVibration(int player)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = 0; // use any value between 0-65535 here
	vibration.wRightMotorSpeed = 0; // use any value between 0-65535 here
	XInputSetState(player, &vibration);
}