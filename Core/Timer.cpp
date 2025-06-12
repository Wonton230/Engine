#include "Timer.hpp"
#include "Engine/Core/Clock.hpp"

Timer::Timer(double period, const Clock* clock)
{
	m_period = period;
	m_clock = clock;
}

void Timer::Start()
{
	m_startTime = m_clock->GetTotalSeconds();
}

void Timer::Stop()
{
	m_startTime = -1.0;
}

double Timer::GetElapsedTime() const
{
	if (m_startTime == -1.0)
	{
		return 0.0;
	}
	return m_clock->GetTotalSeconds() - m_startTime;
}

double Timer::GetElapsedFraction() const
{
	if (m_startTime == -1.0)
	{
		return 0.0;
	}
	return GetElapsedTime() / m_period;
}

bool Timer::IsStopped() const
{
	if (m_startTime == -1.0)
	{
		return true;
	}
	return false;
}

bool Timer::HasPeriodElapsed() const
{
	if (m_startTime == -1.0 || GetElapsedTime() > m_period)
	{
		return true;
	}
	return false;
}

bool Timer::DecrementPeriodIfElapsed()
{
	if (m_startTime == -1.0)
	{
		return false;
	}

	float elapsed = (float)GetElapsedTime();
	if (elapsed > m_period)
	{
		// Subtract full periods that have elapsed
		float periodsElapsed = (float)(elapsed / m_period);
		m_startTime += m_period * periodsElapsed;
		return true;
	}
	return false;
}
