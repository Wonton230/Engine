#pragma once

class KeyButtonState
{
public:
	KeyButtonState();
	~KeyButtonState();

	bool m_isPressed;
	bool m_wasPressedLastFrame;
};