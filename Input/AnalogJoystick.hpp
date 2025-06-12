#pragma once
#include "Engine/Math/Vec2.hpp"

constexpr float MIN_RANGE = -32768;
constexpr float MAX_RANGE = 32767;

class AnalogJoystick
{
public:
	AnalogJoystick();
	~AnalogJoystick();

	Vec2 GetPosition() const;
	float GetMagnitude() const;
	float GetOrientationDegrees() const;

	Vec2 GetUncorrectedPosition() const;
	float GetInnerDeadzoneFraction() const;
	float GetOuterDeadzoneFraction() const;

	void Reset();
	void SetDeadzoneThresholds(float normalizedInnerthreshold, float NormalizedOuterThreshold);
	void UpdatePosition(float rawNormalizedX, float rawNormalizedY);

protected:
	Vec2 m_rawPosition;
	Vec2 m_correctedPosition;
	float m_innerDeadzoneFraction = 0.30f;
	float m_outerDeadzoneFraction = 0.95f;
};