#pragma once
#include "AnalogJoystick.hpp"
#include "KeyButtonState.hpp"

constexpr int NUM_BUTTONS = 14;
constexpr int NUM_STICKS = 2;

constexpr int XBOX_BUTTON_A				= 0;
constexpr int XBOX_BUTTON_B				= 1;
constexpr int XBOX_BUTTON_X				= 2;
constexpr int XBOX_BUTTON_Y				= 3;
constexpr int XBOX_BUTTON_BACK			= 4;
constexpr int XBOX_BUTTON_START			= 5;
constexpr int XBOX_BUTTON_RIGHT_BUMPER	= 6;
constexpr int XBOX_BUTTON_LEFT_BUMPER	= 7;
constexpr int XBOX_BUTTON_DPAD_DOWN		= 8;
constexpr int XBOX_BUTTON_DPAD_LEFT		= 9;
constexpr int XBOX_BUTTON_DPAD_UP		= 10;
constexpr int XBOX_BUTTON_DPAD_RIGHT	= 11;
constexpr int XBOX_BUTTON_LSTICK		= 12;
constexpr int XBOX_BUTTON_RSTICK		= 13;

class XboxController
{
	friend class InputSystem;

public:
	XboxController();
	~XboxController();

	bool					IsConnected() const;
	int						GetControllerID() const;
	AnalogJoystick const&	GetLeftStick() const;
	AnalogJoystick const&	GetRightStick() const;
	float					GetLeftTrigger() const;
	float					GetRightTrigger() const;
	KeyButtonState const&	GetButton(int buttonID) const;
	bool					IsButtonDown(int buttonID) const;
	bool					WasButtonJustPressed(int buttonID) const;
	bool					WasButtonJustReleased(int buttonID) const;

private:
	void Update();
	void Reset();
	void UpdateJoystick(AnalogJoystick& out_Joystick, short rawX, short rawY);
	void UpdateTrigger(float& out_TriggerValue, unsigned char rawValue);
	void UpdateButton(int buttonID, unsigned short buttonFlags, unsigned short buttonFlag);

	int m_id = 0;
	bool m_isConnected = false;
	float m_leftTrigger = 0.f;
	float m_rightTrigger = 0.f;
	KeyButtonState m_Buttons[NUM_BUTTONS] = {};
	AnalogJoystick m_LeftJoystick;
	AnalogJoystick m_RightJoystick;
};