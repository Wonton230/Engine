#pragma once
#include "Engine/Math/IntVec2.hpp"
#include <string>

class InputSystem;
struct Vec2;
struct IntVec2;

struct WindowConfig
{
	InputSystem*	m_inputSystem	= nullptr;
	float			m_aspectRatio	= 16.f/9.f;
	std::string		m_windowTitle	= "PLACEHOLDER_NAME";
};

class Window
{
public:
	Window();
	Window(WindowConfig windowConfig);
	~Window();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	WindowConfig const& GetConfig() const;
	void*				GetDisplayContext() const;
	void*				GetHwnd() const;
	IntVec2				GetClientDimensions() const;
	Vec2				GetNormalizedMouseUV() const;
	bool				IsActiveWindow() const;
	void				OnScrollWheelUsed();

	static Window* s_mainWindow;
	bool  m_scrollWheelUsedThisFrame = false;

private:
	void CreateOSWindow();
	void RunMessagePump();

	WindowConfig m_config;
	void* m_displayContext;
	void* m_windowHandle;
};