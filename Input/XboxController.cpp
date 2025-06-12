#include "XboxController.hpp"
#include <Windows.h> 
#include <Xinput.h>
#pragma comment( lib, "xinput" ) 

class XboxButtonID;

XboxController::XboxController()
{
}

XboxController::~XboxController()
{
}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

AnalogJoystick const& XboxController::GetLeftStick() const
{
	return m_LeftJoystick;
}

AnalogJoystick const& XboxController::GetRightStick() const
{
	return m_RightJoystick;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

KeyButtonState const& XboxController::GetButton(int buttonID) const
{
	return m_Buttons[buttonID];
}

bool XboxController::IsButtonDown(int buttonID) const
{
	return m_Buttons[buttonID].m_isPressed;
}

bool XboxController::WasButtonJustPressed(int buttonID) const
{
	return (!m_Buttons[buttonID].m_wasPressedLastFrame && m_Buttons[buttonID].m_isPressed);
}

bool XboxController::WasButtonJustReleased(int buttonID) const
{
	return (m_Buttons[buttonID].m_wasPressedLastFrame && !m_Buttons[buttonID].m_isPressed);
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_id, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		Reset();
		m_isConnected = false;
		return;
	}

	m_isConnected = true;

	XINPUT_GAMEPAD const& state = xboxControllerState.Gamepad;
	UpdateJoystick(m_LeftJoystick, state.sThumbLX, state.sThumbLY);
	UpdateJoystick(m_RightJoystick, state.sThumbRX, state.sThumbRY);

	UpdateTrigger(m_leftTrigger, state.bLeftTrigger);
	UpdateTrigger(m_rightTrigger, state.bRightTrigger);

	UpdateButton(XBOX_BUTTON_A,				state.wButtons, XINPUT_GAMEPAD_A);
	UpdateButton(XBOX_BUTTON_B,				state.wButtons, XINPUT_GAMEPAD_B);
	UpdateButton(XBOX_BUTTON_X,				state.wButtons, XINPUT_GAMEPAD_X);
	UpdateButton(XBOX_BUTTON_Y,				state.wButtons, XINPUT_GAMEPAD_Y);
	UpdateButton(XBOX_BUTTON_BACK,			state.wButtons, XINPUT_GAMEPAD_BACK);
	UpdateButton(XBOX_BUTTON_START,			state.wButtons, XINPUT_GAMEPAD_START);
	UpdateButton(XBOX_BUTTON_RIGHT_BUMPER,	state.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
	UpdateButton(XBOX_BUTTON_LEFT_BUMPER,	state.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
	UpdateButton(XBOX_BUTTON_DPAD_DOWN,		state.wButtons, XBOX_BUTTON_DPAD_DOWN);
	UpdateButton(XBOX_BUTTON_DPAD_LEFT,		state.wButtons, XBOX_BUTTON_DPAD_LEFT);
	UpdateButton(XBOX_BUTTON_DPAD_UP,		state.wButtons, XBOX_BUTTON_DPAD_UP);
	UpdateButton(XBOX_BUTTON_DPAD_RIGHT,	state.wButtons, XBOX_BUTTON_DPAD_RIGHT);
	UpdateButton(XBOX_BUTTON_LSTICK,		state.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
	UpdateButton(XBOX_BUTTON_RSTICK,		state.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
}

void XboxController::Reset()
{
	for (int buttonIndex = 0; buttonIndex < NUM_BUTTONS; buttonIndex++)
	{
		m_Buttons[buttonIndex].m_isPressed = false;
		m_Buttons[buttonIndex].m_wasPressedLastFrame = false;
	}

	m_leftTrigger = 0;
	m_rightTrigger = 0;

	m_LeftJoystick.Reset();
	m_RightJoystick.Reset();
}

void XboxController::UpdateJoystick(AnalogJoystick& out_Joystick, short rawX, short rawY)
{
	out_Joystick.UpdatePosition(rawX, rawY);
}

void XboxController::UpdateTrigger(float& out_TriggerValue, unsigned char rawValue)
{
	out_TriggerValue = (float)rawValue;
}

void XboxController::UpdateButton(int buttonID, unsigned short buttonFlags, unsigned short buttonFlag)
{
	m_Buttons[buttonID].m_wasPressedLastFrame = m_Buttons[buttonID].m_isPressed;
	m_Buttons[buttonID].m_isPressed = ((buttonFlags & buttonFlag) == buttonFlag);
}
