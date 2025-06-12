#pragma once
#include "XboxController.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/EventSystem.hpp"

constexpr int NUM_KEYS = 256;
constexpr int NUM_CONTROLLERS = 4;
extern unsigned char const KEYCODE_F1;
extern unsigned char const KEYCODE_F2;
extern unsigned char const KEYCODE_F3;
extern unsigned char const KEYCODE_F4;
extern unsigned char const KEYCODE_F5;
extern unsigned char const KEYCODE_F6;
extern unsigned char const KEYCODE_F7;
extern unsigned char const KEYCODE_F8;
extern unsigned char const KEYCODE_F9;
extern unsigned char const KEYCODE_F10;
extern unsigned char const KEYCODE_F11;
extern unsigned char const KEYCODE_F12;
extern unsigned char const KEYCODE_ESC;
extern unsigned char const KEYCODE_UPARROW;
extern unsigned char const KEYCODE_DOWNARROW;
extern unsigned char const KEYCODE_LEFTARROW;
extern unsigned char const KEYCODE_RIGHTARROW;
extern unsigned char const KEYCODE_SPACE;
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_SHIFT;
extern unsigned char const KEYCODE_UPARROW;
extern unsigned char const KEYCODE_DOWNARROW;
extern unsigned char const KEYCODE_LEFTARROW;
extern unsigned char const KEYCODE_RIGHTARROW;
extern unsigned char const KEYCODE_SPACE;
extern unsigned char const KEYCODE_ENTER;
extern unsigned char const KEYCODE_BACKSPACE;
extern unsigned char const KEYCODE_INSERT;
extern unsigned char const KEYCODE_DELETE;
extern unsigned char const KEYCODE_HOME;
extern unsigned char const KEYCODE_END;
extern unsigned char const KEYCODE_TILDE;
extern unsigned char const KEYCODE_LEFTBRACKET;
extern unsigned char const KEYCODE_RIGHTBRACKET;

extern unsigned char const KEYCODE_LMB;
extern unsigned char const KEYCODE_RMB;
extern unsigned char const KEYCODE_MOUSEWHEEL_UP;
extern unsigned char const KEYCODE_MOUSEWHEEL_DOWN;

struct InputConfig
{

};

enum class CursorMode
{
	POINTER,
	FPS,
	COUNT
};

struct CursorState
{
	IntVec2 m_cursorClientDelta;
	IntVec2 m_cursorClientPosition;

	CursorMode m_cursorMode = CursorMode::POINTER;
};

class InputSystem
{
protected:
	KeyButtonState m_KeyStates[NUM_KEYS] = {};
	XboxController m_XboxControllers[NUM_CONTROLLERS] = {};
public:
	InputSystem();
	InputSystem(InputConfig inputConfig);
	~InputSystem();

	void Startup();
	void Shutdown();

	XboxController const& GetController(int id);

	bool IsKeyDown(unsigned char keyCode);
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	void HandleKeyPressed(unsigned char keyCode);
	void HandleKeyReleased(unsigned char keyCode);

	void SetCursorMode(CursorMode cursorMode);
	Vec2 GetCursorClientDelta() const;
	Vec2 GetCursorClientPosition() const;
	//Vec2 GetCursorNormalizedPosition() const;

	void BeginFrame();
	void EndFrame();

	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_KeyReleased(EventArgs& args);

private:
	InputConfig m_inputConfig;
	CursorState m_cursorState;
	bool		m_cursorShowing = true;
};