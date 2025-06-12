#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include "Window.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/EventSystem.hpp"

Window* Window::s_mainWindow = nullptr;

Window::Window()
{
	m_windowHandle = nullptr;
	m_displayContext = nullptr;
}

Window::Window(WindowConfig windowConfig)
{
	s_mainWindow = this;
	m_config = windowConfig;
	m_windowHandle = nullptr;
	m_displayContext = nullptr;
}

Window::~Window()
{
}

void Window::Startup()
{
	CreateOSWindow();
}

void Window::Shutdown()
{
}

void Window::BeginFrame()
{
	m_scrollWheelUsedThisFrame = false;
	RunMessagePump();
}

void Window::EndFrame()
{
	if (!m_scrollWheelUsedThisFrame)
	{
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_MOUSEWHEEL_UP));
		FireEvent("KeyReleased", args);
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_MOUSEWHEEL_DOWN));
		FireEvent("KeyReleased", args);
	}
}

WindowConfig const& Window::GetConfig() const
{
	return m_config;
}

void* Window::GetDisplayContext() const
{
	return m_displayContext;
}

void* Window::GetHwnd() const
{
	return m_windowHandle;
}

IntVec2 Window::GetClientDimensions() const
{
	RECT clientRect;
	::GetClientRect(static_cast<HWND>(GetHwnd()), &clientRect);
	return IntVec2(static_cast<int>(clientRect.right), static_cast<int>(clientRect.bottom));
}

Vec2 Window::GetNormalizedMouseUV() const
{
	HWND windowHandle = static_cast<HWND>(m_windowHandle);
	POINT cursorCoords;
	RECT clientRect;
	::GetCursorPos(&cursorCoords);
	::ScreenToClient(windowHandle, &cursorCoords);
	::GetClientRect(windowHandle, &clientRect);
	float cursorX = static_cast<float>(cursorCoords.x) / static_cast<float>(clientRect.right);
	float cursorY = static_cast<float>(cursorCoords.y) / static_cast<float>(clientRect.bottom);
	return Vec2(cursorX, 1.f - cursorY);
}

bool Window::IsActiveWindow() const
{
	return (HWND)m_windowHandle == GetActiveWindow();
}

void Window::OnScrollWheelUsed()
{
	m_scrollWheelUsedThisFrame = true;
}

LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	InputSystem* input = nullptr;
	if (Window::s_mainWindow)
	{
		WindowConfig const& config = Window::s_mainWindow->GetConfig();
		input = config.m_inputSystem;
	}

	switch (wmMessageCode)
	{
	case WM_CLOSE:
	{
		FireEvent("quit");
		return 0;
		break;
	}

	case WM_CHAR:
	{
		unsigned char asKey = (unsigned char)wParam;
		EventArgs args;
		args.SetValue("CharCode", Stringf("%d", asKey));
		FireEvent("CharPressed", args);
		return 0;
		break;
	}

	case WM_KEYDOWN:
	{
		unsigned char asKey = (unsigned char)wParam;
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", asKey));
		FireEvent("KeyPressed", args);
		return 0;
		break;
	}

	case WM_KEYUP:
	{
		unsigned char asKey = (unsigned char)wParam;
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", asKey));
		FireEvent("KeyReleased", args);
		return 0;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_LMB));
		FireEvent("KeyPressed", args);
		return 0;
		break;
	}
	case WM_LBUTTONUP:
	{
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_LMB));
		FireEvent("KeyReleased", args);
		return 0;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_RMB));
		FireEvent("KeyPressed", args);
		return 0;
		break;
	}
	case WM_RBUTTONUP:
	{
		EventArgs args;
		args.SetValue("KeyCode", Stringf("%d", KEYCODE_RMB));
		FireEvent("KeyReleased", args);
		return 0;
		break;
	}
	case WM_MOUSEWHEEL:
	{
		int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		EventArgs args;

		if (wheelDelta > 0)
		{
			args.SetValue("KeyCode", Stringf("%d", KEYCODE_MOUSEWHEEL_UP));
			FireEvent("KeyPressed", args);
			Window::s_mainWindow->OnScrollWheelUsed();
		}
		else if (wheelDelta < 0)
		{
			args.SetValue("KeyCode", Stringf("%d", KEYCODE_MOUSEWHEEL_DOWN));
			FireEvent("KeyPressed", args);
			Window::s_mainWindow->OnScrollWheelUsed();
		}
		return 0;
		break;
	}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}

void Window::CreateOSWindow()
{
	HMODULE applicationInstanceHandle = GetModuleHandle(NULL);
	float clientAspect = m_config.m_aspectRatio;

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	// #SD1ToDo: Add support for fullscreen mode (requires different window style flags than windowed mode)
	DWORD const windowStyleFlags = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_OVERLAPPED;
	DWORD const windowStyleExFlags = WS_EX_APPWINDOW;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	// Calculate maximum client size (as some % of desktop size)
	constexpr float maxClientFractionOfDesktop = 0.90f;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if (clientAspect > desktopAspect)
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * clientAspect;
	}

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;
	clientRect.left = (int)clientMarginX;
	clientRect.right = clientRect.left + (int)clientWidth;
	clientRect.top = (int)clientMarginY;
	clientRect.bottom = clientRect.top + (int)clientHeight;

	// Calculate the outer dimensions of the physical window, including frame et. al.
	RECT windowRect = clientRect;
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	m_windowHandle = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		(HINSTANCE)applicationInstanceHandle,
		NULL);

	ShowWindow(static_cast<HWND>(m_windowHandle), SW_SHOW);
	SetForegroundWindow(static_cast<HWND>(m_windowHandle));
	SetFocus(static_cast<HWND>(m_windowHandle));

	m_displayContext = GetDC(static_cast<HWND>(m_windowHandle));

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);
}

void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		BOOL const wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}