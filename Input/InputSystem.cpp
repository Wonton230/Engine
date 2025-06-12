#include "InputSystem.hpp"
#include "KeyButtonState.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Window/Window.hpp"
#include <Windows.h>

constexpr unsigned char const KEYCODE_F1 = VK_F1;
constexpr unsigned char const KEYCODE_F2 = VK_F2;
constexpr unsigned char const KEYCODE_F3 = VK_F3;
constexpr unsigned char const KEYCODE_F4 = VK_F4;
constexpr unsigned char const KEYCODE_F5 = VK_F5;
constexpr unsigned char const KEYCODE_F6 = VK_F6;
constexpr unsigned char const KEYCODE_F7 = VK_F7;
constexpr unsigned char const KEYCODE_F8 = VK_F8;
constexpr unsigned char const KEYCODE_F9 = VK_F9;
constexpr unsigned char const KEYCODE_F10 = VK_F10;
constexpr unsigned char const KEYCODE_F11 = VK_F11;
constexpr unsigned char const KEYCODE_F12 = VK_F12;
constexpr unsigned char const KEYCODE_ESC = VK_ESCAPE;
constexpr unsigned char const KEYCODE_UPARROW = VK_UP;
constexpr unsigned char const KEYCODE_DOWNARROW = VK_DOWN;
constexpr unsigned char const KEYCODE_LEFTARROW = VK_LEFT;
constexpr unsigned char const KEYCODE_RIGHTARROW = VK_RIGHT;
constexpr unsigned char const KEYCODE_SPACE = VK_SPACE;
constexpr unsigned char const KEYCODE_ENTER = VK_RETURN;
constexpr unsigned char const KEYCODE_SHIFT = VK_SHIFT;
constexpr unsigned char const KEYCODE_BACKSPACE = VK_BACK;
constexpr unsigned char const KEYCODE_INSERT = VK_INSERT;
constexpr unsigned char const KEYCODE_DELETE = VK_DELETE;
constexpr unsigned char const KEYCODE_HOME = VK_HOME;
constexpr unsigned char const KEYCODE_END = VK_END;
constexpr unsigned char const KEYCODE_TILDE = 0xC0;
constexpr unsigned char const KEYCODE_LMB = VK_LBUTTON;
constexpr unsigned char const KEYCODE_RMB = VK_RBUTTON;
constexpr unsigned char const KEYCODE_LEFTBRACKET = 219;
constexpr unsigned char const KEYCODE_RIGHTBRACKET = 221;

constexpr unsigned char const KEYCODE_MOUSEWHEEL_UP = 200;
constexpr unsigned char const KEYCODE_MOUSEWHEEL_DOWN = 201;

extern Window* g_theWindow;

InputSystem::InputSystem()
{
}

InputSystem::InputSystem(InputConfig inputConfig)
{
	m_inputConfig = inputConfig;
}

InputSystem::~InputSystem()
{
}

void InputSystem::Startup()
{
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyReleased", Event_KeyReleased);
}

void InputSystem::Shutdown()
{
}

XboxController const& InputSystem::GetController(int id)
{
	return m_XboxControllers[id];
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_KeyStates[keyCode].m_isPressed;
}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return (!m_KeyStates[keyCode].m_wasPressedLastFrame && m_KeyStates[keyCode].m_isPressed);
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return (m_KeyStates[keyCode].m_wasPressedLastFrame && !m_KeyStates[keyCode].m_isPressed);
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_KeyStates[keyCode].m_isPressed = true;
}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_KeyStates[keyCode].m_isPressed = false;
}

void InputSystem::SetCursorMode(CursorMode cursorMode)
{
	m_cursorState.m_cursorMode = cursorMode;
}

Vec2 InputSystem::GetCursorClientDelta() const
{
	return Vec2((float)m_cursorState.m_cursorClientDelta.x, (float)m_cursorState.m_cursorClientDelta.y);
}

Vec2 InputSystem::GetCursorClientPosition() const
{
	return Vec2((float)m_cursorState.m_cursorClientPosition.x, (float)m_cursorState.m_cursorClientPosition.y);
}

void InputSystem::BeginFrame()
{
	for (int controllerIndex = 0; controllerIndex < NUM_CONTROLLERS; controllerIndex++)
	{
		m_XboxControllers[controllerIndex].Update();
	}

	//Cursor Handling (Windows)
	POINT p;
	while (!GetCursorPos(&p))
	{
	}
	ScreenToClient((HWND)g_theWindow->GetHwnd(), &p);
	IntVec2 previousCursorPosition = m_cursorState.m_cursorClientPosition;
	m_cursorState.m_cursorClientPosition = IntVec2((int)p.x, (int)p.y);

	if (m_cursorState.m_cursorMode == CursorMode::FPS)
	{
		int cursorReturn = 1;
		while (cursorReturn >= 0 && m_cursorShowing)
		{
			cursorReturn = ShowCursor(false);
		}
		m_cursorShowing = false;
		m_cursorState.m_cursorClientDelta = m_cursorState.m_cursorClientPosition - previousCursorPosition;
		SetCursorPos(g_theWindow->GetClientDimensions().x / 2, g_theWindow->GetClientDimensions().y / 2);
		while (!GetCursorPos(&p))
		{
		}
		ScreenToClient((HWND)g_theWindow->GetHwnd(), &p);
		m_cursorState.m_cursorClientPosition = IntVec2((int)p.x, (int)p.y);
	}
	else if (m_cursorState.m_cursorMode == CursorMode::POINTER)
	{
		int cursorReturn = -1;
		while (cursorReturn < 0 && !m_cursorShowing)
		{
			cursorReturn = ShowCursor(true);
		}
		m_cursorShowing = true;
		m_cursorState.m_cursorClientDelta = IntVec2(0, 0);
	}

}

void InputSystem::EndFrame()
{
	for (int keyIndex = 0; keyIndex < NUM_KEYS; keyIndex++)
	{
		m_KeyStates[keyIndex].m_wasPressedLastFrame = m_KeyStates[keyIndex].m_isPressed;
	}
}

bool InputSystem::Event_KeyPressed(EventArgs& args)
{
	if (!g_theInputSystem)
	{
		return false;
	}
	unsigned char keyCode = static_cast<unsigned char>(args.GetValue("KeyCode", -1));
	g_theInputSystem->HandleKeyPressed(keyCode);
	return true;
}

bool InputSystem::Event_KeyReleased(EventArgs& args)
{
	if (!g_theInputSystem)
	{
		return false;
	}
	unsigned char keyCode = static_cast<unsigned char>(args.GetValue("KeyCode", -1));
	g_theInputSystem->HandleKeyReleased(keyCode);
	return false;
}
