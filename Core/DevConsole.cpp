#include "DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/VertexUtils.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Timer.hpp"

const Rgba8 DevConsole::ERROR = Rgba8(255, 0, 0, 255);     // Red
const Rgba8 DevConsole::WARNING = Rgba8(255, 255, 0, 255); // Yellow
const Rgba8 DevConsole::INFO_MAJOR = Rgba8(0, 255, 0, 255); // Green
const Rgba8 DevConsole::INFO_MINOR = Rgba8(0, 0, 255, 255); // Blue
const Rgba8 DevConsole::INPUT_TEXT = Rgba8(150, 150, 150, 255);
const Rgba8 DevConsole::INPUT_INSERTION_POINT = Rgba8(255,255,255,255); //White

DevConsole::DevConsole(DevConsoleConfig const& config)
	:m_config(config),
	 m_lines(config.maxLines),
	 m_maxLines(config.maxLines)
{
	m_insertionPointBlinkTimer = new Timer(0.5, g_theSystemClock);
	m_insertionPointVisible = true;
}

DevConsole::~DevConsole()
{
	m_lines.clear();
}

void DevConsole::Startup()
{
	m_lines.reserve(m_config.maxLines);
	g_theEventSystem->SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	g_theEventSystem->SubscribeEventCallbackFunction("CharPressed", Event_CharInput);
	AddText(INFO_MAJOR, "DevConsole Start:");
	g_theEventSystem->SubscribeEventCallbackFunction("help", Command_Help);
	g_theEventSystem->SubscribeEventCallbackFunction("clear", Command_Clear);

	m_insertionPointBlinkTimer->Start();
	//FireEvent("help");
}

void DevConsole::Shutdown()
{

}

void DevConsole::BeginFrame()
{
	m_frameNumber += 1;
	if (m_insertionPointBlinkTimer->HasPeriodElapsed())
	{
		m_insertionPointVisible = !m_insertionPointVisible;
		m_insertionPointBlinkTimer->DecrementPeriodIfElapsed();
	}
}

void DevConsole::EndFrame()
{
}

void DevConsole::Execute(std::string const& consoleCommandText, bool echoCommand)
{
	Strings command = SplitStringOnDelimiter(consoleCommandText, ' ');
	EventArgs args;
	bool isValidCommand = false;

	if (command.empty())
	{
		return;
	}
	for (int i = 0; i < static_cast<int>(m_registeredCommands.size()); i++)
	{
		if (m_registeredCommands[i] == command[0])
		{
			isValidCommand = true;
			break;
		}
	}
	if (!isValidCommand)
	{
		AddText(ERROR, "Unrecognized command: " + command[0]);
		return;
	}

	if (echoCommand)
	{
		AddText(INFO_MAJOR, consoleCommandText);
	}
	for (int argIndex = 1; argIndex < command.size(); argIndex++)
	{
		Strings argPair = SplitStringOnDelimiter(command[argIndex], '=');
		if (argPair.size() >= 2)
		{
			args.SetValue(argPair[0], argPair[1]);
		}
	}
	FireEvent(command[0], args);

	m_commandHistory.push_back(consoleCommandText);
}

void DevConsole::AddText(Rgba8 const& color, std::string const& text)
{
	Strings lines = SplitStringOnDelimiter(text, '\n');

	for (int i = 0; i < lines.size(); i++)
	{
		m_lines[0].m_text = lines[i];
		m_lines[0].m_color = color;
		for (int j = static_cast<int>(m_lines.size()) - 1; j >= 0; j--)
		{
			if (j + 1 < m_config.maxLines)
			{
				m_lines[j + 1].m_text = m_lines[j].m_text;
				m_lines[j + 1].m_color = m_lines[j].m_color;
				m_lines[j + 1].m_aspectRatio = m_lines[j].m_aspectRatio;
			}
		}
		m_lines[0].m_text = "";
		m_lines[0].m_color = INPUT_TEXT;
		m_lines[0].m_aspectRatio = -1.f;
	}
}

void DevConsole::AddText(Rgba8 const& color, std::string const& text, float aspect)
{
	Strings lines = SplitStringOnDelimiter(text, '\n');

	for (int i = 0; i < lines.size(); i++)
	{
		m_lines[0].m_text = lines[i];
		m_lines[0].m_color = color;
		m_lines[0].m_aspectRatio = aspect;
		for (int j = static_cast<int>(m_lines.size()) - 1; j >= 0; j--)
		{
			if (j + 1 < m_config.maxLines)
			{
				m_lines[j + 1].m_text = m_lines[j].m_text;
				m_lines[j + 1].m_color = m_lines[j].m_color;
				m_lines[j + 1].m_aspectRatio = m_lines[j].m_aspectRatio;
			}
		}
		m_lines[0].m_text = "";
		m_lines[0].m_color = INPUT_TEXT;
		m_lines[0].m_aspectRatio = -1.f;
	}
}

void DevConsole::Render(AABB2 const& bounds, Renderer* rendererOverride) const
{
	switch (m_mode)
	{
	case DevConsoleMode::HIDDEN:
		break;

	case DevConsoleMode::FULL:
		Render_OpenFull(bounds, *rendererOverride,*m_config.font, .7f);
		break;

	default:
		ERROR_AND_DIE("invalid console mode");
		break;
	}
}

void DevConsole::AddValidCommand(std::string command)
{
	m_registeredCommands.push_back(command);
}

DevConsoleMode DevConsole::GetMode() const
{
	return DevConsoleMode();
}

void DevConsole::SetMode(DevConsoleMode mode)
{
	m_mode = mode;
}

void DevConsole::ToggleMode(DevConsoleMode mode)
{
	if (m_mode != DevConsoleMode::HIDDEN)
	{
		SetMode(DevConsoleMode::HIDDEN);
		m_isOpen = false;
	}
	else
	{
		SetMode(mode);
		m_isOpen = true;
	}
}

bool DevConsole::IsOpen()
{
	return m_isOpen;
}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	g_theDevConsole->m_insertionPointBlinkTimer->Stop();
	g_theDevConsole->m_insertionPointVisible = true;
	g_theDevConsole->m_insertionPointBlinkTimer->Start();

	if (!g_theInputSystem)
	{
		return false;
	}
	if (g_theDevConsole->IsOpen())
	{
		unsigned char keyCode = static_cast<unsigned char>(args.GetValue("KeyCode", -1));
		if (keyCode == KEYCODE_RIGHTARROW)
		{
			std::string temp = g_theDevConsole->m_lines[0].m_text;
			if (g_theDevConsole->m_insertionPointPosition < temp.length()) 
			{
				g_theDevConsole->m_insertionPointPosition++;
			}
			return true;
		}
		if (keyCode == KEYCODE_LEFTARROW)
		{
			std::string temp = g_theDevConsole->m_lines[0].m_text;
			if (g_theDevConsole->m_insertionPointPosition > 0)
			{
				g_theDevConsole->m_insertionPointPosition--;
			}
			return true;
		}
		if (keyCode == KEYCODE_ENTER)
		{
			std::string temp = g_theDevConsole->m_lines[0].m_text;
			g_theDevConsole->m_insertionPointPosition = 0;
			g_theDevConsole->AddText(g_theDevConsole->INPUT_INSERTION_POINT, temp);
			g_theDevConsole->Execute(temp);
			return true;
		}
		if (keyCode == KEYCODE_ESC)
		{
			if (g_theDevConsole->m_lines[0].m_text.empty())
			{
				g_theDevConsole->ToggleMode(g_theDevConsole->m_mode);
			}
			else
			{
				g_theDevConsole->m_lines[0].m_text = "";
				g_theDevConsole->m_insertionPointPosition = 0;
			}
			return true;
		}
		if (keyCode == KEYCODE_HOME)
		{
			g_theDevConsole->m_insertionPointPosition = 0;
			return true;
		}
		if (keyCode == KEYCODE_END)
		{
			g_theDevConsole->m_insertionPointPosition = static_cast<int>(g_theDevConsole->m_lines[0].m_text.length());
			return true;
		}
		if (keyCode == KEYCODE_DELETE)
		{
			std::string temp = g_theDevConsole->m_lines[0].m_text;
			if (g_theDevConsole->m_insertionPointPosition < temp.length()) {
				temp.erase(g_theDevConsole->m_insertionPointPosition, 1);
				g_theDevConsole->m_lines[0].m_text = temp;
			}
			return true;
		}
		if (keyCode == KEYCODE_UPARROW)
		{
			if (g_theDevConsole->m_historyIndex >= -1 && (g_theDevConsole->m_historyIndex < static_cast<int>(g_theDevConsole->m_commandHistory.size()-1) && g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex+1] != ""))
			{
				g_theDevConsole->m_historyIndex++;
				g_theDevConsole->m_lines[0].m_text = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
			}
			return true;
		}
		if (keyCode == KEYCODE_DOWNARROW)
		{
			if (g_theDevConsole->m_historyIndex == 0)
			{
				g_theDevConsole->m_lines[0].m_text = "";
			}
			else if (g_theDevConsole->m_historyIndex > 0)
			{
				g_theDevConsole->m_historyIndex--;
				g_theDevConsole->m_lines[0].m_text = g_theDevConsole->m_commandHistory[g_theDevConsole->m_historyIndex];
			}
			return true;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	g_theDevConsole->m_insertionPointBlinkTimer->Stop();
	g_theDevConsole->m_insertionPointVisible = true;
	g_theDevConsole->m_insertionPointBlinkTimer->Start();

	if (!g_theInputSystem)
	{
		return false;
	}
	if (g_theDevConsole->IsOpen())
	{
		unsigned char charCode = static_cast<unsigned char>(args.GetValue("CharCode", -1));
		if (charCode == '\b')
		{
			std::string temp = g_theDevConsole->m_lines[0].m_text;
			if (g_theDevConsole->m_insertionPointPosition > 0 && g_theDevConsole->m_insertionPointPosition <= temp.length()) {
				temp.erase(g_theDevConsole->m_insertionPointPosition - 1, 1);
				g_theDevConsole->m_insertionPointPosition--;
				g_theDevConsole->m_lines[0].m_text = temp;
			}
			return true;
		}
		if (charCode == '`' || charCode == '~')
		{
			g_theDevConsole->ToggleMode(g_theDevConsole->m_mode);
			return true;
		}
		if (charCode < 32 || charCode > 126)
		{
			return true;
		}
		std::string temp = g_theDevConsole->m_lines[0].m_text;
		temp = temp.c_str();
		temp.insert(g_theDevConsole->m_insertionPointPosition, 1, charCode);
		g_theDevConsole->m_insertionPointPosition++;
		g_theDevConsole->m_lines[0].m_text = temp;
		return true;
	}
	else
	{
		return false;
	}
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);
	g_theDevConsole->m_lines.clear();
	g_theDevConsole->m_lines.resize(g_theDevConsole->m_config.maxLines);
	return true;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	UNUSED(args);
	g_theDevConsole->AddText(INFO_MINOR, "Supported Commands: \n help \n clear \n quit \n debug_clear \n debug_toggle");
	return true;
}

void DevConsole::Render_OpenFull(AABB2 const& bounds, Renderer& renderer, BitmapFont& font, float fontAspect) const
{
	std::vector<Vertex_PCU> consoleBGVerts;
	std::vector<Vertex_PCU> consoleVerts;
	std::vector<Vertex_PCU> consoleInsertionMarker;
	AddVertsForAABB2D(consoleBGVerts, bounds, Rgba8(0, 0, 0, 155));

	float lineHeight = bounds.m_maxs.y - bounds.m_mins.y;
	lineHeight /= m_config.maxLines;
	AABB2 lineBox;

	for (int i = 0; i < m_lines.size(); i++)
	{
		lineBox = AABB2(bounds.m_mins.x, (lineHeight * i), bounds.m_maxs.x, (lineHeight * (i+1)));
		if (!m_lines[i].m_text.empty())
		{
			if (m_lines[i].m_aspectRatio > -1.f)
			{
				font.AddVertsForTextInBox2D(consoleVerts, m_lines[i].m_text, lineBox, lineHeight, m_lines[i].m_color, m_lines[i].m_aspectRatio, Vec2(0.f, 0.5f), SHRINK_TO_FIT);
			}
			else
			{
				font.AddVertsForTextInBox2D(consoleVerts, m_lines[i].m_text, lineBox, lineHeight, m_lines[i].m_color, fontAspect, Vec2(0.f, 0.5f), SHRINK_TO_FIT);
			}
		}
	}

	renderer.SetRasterizerMode(RasterizerMode::SOLID_CULL_BACK);
	renderer.BindShader(nullptr);
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray(consoleBGVerts);

	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray(consoleVerts);

	renderer.SetStatesIfChanged();

	//Render marker

	AABB2 markerBox;

	markerBox = AABB2(m_insertionPointPosition * fontAspect * lineHeight - (fontAspect * lineHeight)/10, 0.0f, m_insertionPointPosition * fontAspect * lineHeight + (fontAspect * lineHeight) / 10, lineHeight);
	AddVertsForAABB2D(consoleInsertionMarker, markerBox, INPUT_INSERTION_POINT);
	if (m_insertionPointVisible)
	{
		renderer.BindTexture(nullptr);
		renderer.DrawVertexArray(consoleInsertionMarker);
	}
}