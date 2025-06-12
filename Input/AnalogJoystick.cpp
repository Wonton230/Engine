#include "AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"

AnalogJoystick::AnalogJoystick()
{
}

AnalogJoystick::~AnalogJoystick()
{
}

Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetUncorrectedPosition() const
{
	return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadzoneFraction() const
{
	return m_innerDeadzoneFraction;
}

float AnalogJoystick::GetOuterDeadzoneFraction() const
{
	return m_outerDeadzoneFraction;
}

void AnalogJoystick::Reset()
{
	m_rawPosition = Vec2(0, 0);
	m_correctedPosition = Vec2(0, 0);
}

void AnalogJoystick::SetDeadzoneThresholds(float normalizedInnerthreshold, float NormalizedOuterThreshold)
{
	m_innerDeadzoneFraction = normalizedInnerthreshold;
	m_outerDeadzoneFraction = NormalizedOuterThreshold;
}

void AnalogJoystick::UpdatePosition(float rawNormalizedX, float rawNormalizedY)
{
	m_rawPosition.x = rawNormalizedX;
	m_rawPosition.y = rawNormalizedY;

	if (m_rawPosition.GetLength() <= MAX_RANGE * m_innerDeadzoneFraction)
	{
		m_correctedPosition.ClampLength(0);
	}
	else
	{
		m_correctedPosition = Vec2::MakeFromPolarDegrees(m_rawPosition.GetOrientationDegrees(), RangeMapClamped(m_rawPosition.GetLength(), MAX_RANGE * m_innerDeadzoneFraction, MAX_RANGE * m_outerDeadzoneFraction, 0.f, 1.f));
	}
}
