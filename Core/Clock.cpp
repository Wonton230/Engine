#include "Clock.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Time.hpp"

extern Clock* g_theSystemClock;

Clock::Clock()
{
	m_parent = g_theSystemClock;
}

Clock::~Clock()
{
	m_children.clear();
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}
	m_parent = nullptr;
}

Clock::Clock(Clock& parent)
{
	m_parent = &parent;
	m_parent->AddChild(this);
}

void Clock::Reset()
{
	m_totalSeconds = 0.0;
	m_deltaSeconds = 0.0;
	m_frameCount = 0;
	m_timeScale = 1.0;
	m_isPaused = false;
	m_stepSingleFrame = false;
	m_maxDeltaSeconds = 0.1;
	m_lastupdatedTimeInSeconds = g_theSystemClock->m_lastupdatedTimeInSeconds;
}

bool Clock::IsPaused() const
{
	return m_isPaused;
}

void Clock::Pause()
{
	m_isPaused = true;
	m_timeScale = 0.0f;
}

void Clock::Unpause()
{
	m_isPaused = false;
	m_timeScale = 1.0f;
}

void Clock::TogglePause()
{
	if (m_isPaused)
	{
		Unpause();
	}
	else
	{
		Pause();
	}
}

void Clock::StepSingleFrame()
{
	if (!m_stepSingleFrame)
	{
		Unpause();
		m_stepSingleFrame = true;
	}
}

void Clock::SetTimeScale(double timeScale)
{
	m_timeScale = timeScale;
}

double Clock::GetTimeScale() const
{
	return m_timeScale;
}

double Clock::GetDeltaSeconds() const
{
	return m_deltaSeconds;
}

double Clock::GetTotalSeconds() const
{
	return m_totalSeconds;
}

double Clock::GetFrameRate() const
{
	return m_frameCount / m_totalSeconds;
}

int Clock::GetFrameCount() const
{
	return m_frameCount;
}

Clock* Clock::GetChildAtIndex(int index)
{
	if (index >= 0 && index < static_cast<int>(m_children.size()))
	{
		return m_children[index];
	}
	return nullptr;
}

Clock& Clock::GetSystemClock()
{
	return *g_theSystemClock;
}

void Clock::TickSystemClock()
{
	g_theSystemClock->Tick();
}

void Clock::Tick()
{
	double currentTime = GetCurrentTimeSeconds();
	m_deltaSeconds = currentTime - m_lastupdatedTimeInSeconds;
	m_deltaSeconds = GetClamped(static_cast<float>(m_deltaSeconds), 0.0f, static_cast<float>(m_maxDeltaSeconds));

	m_lastupdatedTimeInSeconds += m_deltaSeconds;

	for (int clockIndex = 0; clockIndex < static_cast<int>(m_children.size()); clockIndex++)
	{
		if (m_children[clockIndex] != nullptr)
		{
			m_children[clockIndex]->Advance(m_deltaSeconds);
		}
	}

	m_totalSeconds += m_deltaSeconds;
}

void Clock::Advance(double deltaTimeSeconds)
{
	m_deltaSeconds = deltaTimeSeconds * m_timeScale;
	m_totalSeconds += m_deltaSeconds;
	m_lastupdatedTimeInSeconds += m_deltaSeconds;
	m_frameCount++;

	for (int clockIndex = 0; clockIndex < static_cast<int>(m_children.size()); clockIndex++)
	{
		if (m_children[clockIndex] != nullptr)
		{
			m_children[clockIndex]->Advance(m_deltaSeconds);
		}
	}

	if (m_stepSingleFrame)
	{
		Pause();
		m_stepSingleFrame = false;
	}
}

void Clock::AddChild(Clock* childClock)
{
	for (int clockIndex = 0; clockIndex < static_cast<int>(m_children.size()); clockIndex++)
	{
		if (m_children[clockIndex] == nullptr)
		{
			m_children[clockIndex] = childClock;
			return;
		}
	}
	m_children.push_back(childClock);
}

void Clock::RemoveChild(Clock* childClock)
{
	for (int clockIndex = 0; clockIndex < static_cast<int>(m_children.size()); clockIndex++)
	{
		if (m_children[clockIndex] == childClock)
		{
			m_children[clockIndex] = nullptr;
		}
	}
}