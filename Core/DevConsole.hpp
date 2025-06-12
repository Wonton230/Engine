#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/EventSystem.hpp"
#include <string>

struct AABB2;
class Renderer;
class BitmapFont;
class Camera;
class Timer;

extern DevConsole* g_theDevConsole;

struct DevConsoleConfig
{
	Renderer* m_renderer = nullptr;
	Camera* m_camera = nullptr;
	std::string m_fontName = "Data/Fonts/OctoFont.png";
	float m_fontAspect = 0.7f;
	int maxLines = 40;
	int circularPointer = 0;
	int maxCommandHistory = 128;
	bool startopen = false;
	BitmapFont* font = nullptr;
};

struct DevConsoleLine
{
	Rgba8 m_color;
	std::string m_text;
	float m_aspectRatio = -1.f;
};

enum class DevConsoleMode
{
	HIDDEN,
	FULL
};

class DevConsole
{
public:
	DevConsole(DevConsoleConfig const& config);
	~DevConsole();
	void Startup(); 
	void Shutdown(); 
	void BeginFrame();
	void EndFrame();

	void Execute(std::string const& consoleCommandText, bool echoCommand = true);
	void AddText(Rgba8 const& color, std::string const& text);
	void AddText(Rgba8 const& color, std::string const& text, float aspect);
	void Render(AABB2 const& bounds, Renderer* rendererOverride = nullptr) const;
	void AddValidCommand(std::string command);

	DevConsoleMode GetMode() const;
	void SetMode(DevConsoleMode mode);
	void ToggleMode(DevConsoleMode mode);
	bool IsOpen();

	static const Rgba8 ERROR;
	static const Rgba8 WARNING;
	static const Rgba8 INFO_MAJOR;
	static const Rgba8 INFO_MINOR;
	static const Rgba8 INPUT_TEXT;
	static const Rgba8 INPUT_INSERTION_POINT;

	//Event functions
	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_CharInput(EventArgs& args);

	//Commands
	static bool Command_Clear(EventArgs& args);
	static bool Command_Help(EventArgs& args);

protected:
	void Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect = 1.f) const;

protected:
	DevConsoleConfig				m_config;
	DevConsoleMode					m_mode = DevConsoleMode::HIDDEN; 
	bool							m_isOpen = false;
	std::vector<DevConsoleLine>		m_lines;
	int								m_maxLines;
	int								m_frameNumber = 0;
	std::vector<std::string>		m_registeredCommands = {"help","clear","quit","debug_clear","debug_toggle"};

	//Typing and insertion point tracking
	int m_insertionPointPosition = 0;
	bool m_insertionPointVisible = true;
	Timer* m_insertionPointBlinkTimer;
	std::vector<std::string> m_commandHistory;
	int m_historyIndex = -1;
};