#pragma once
#include "Engine/Math/Vec2.hpp"

class CubicHermiteCurve2D;

class CubicBezierCurve2D
{
public:
	CubicBezierCurve2D(Vec2 start, Vec2 guide1, Vec2 guide2, Vec2 end);
	explicit CubicBezierCurve2D(CubicHermiteCurve2D const& curve);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float length, int numSubDivisions = 64) const;

	Vec2 m_start;
	Vec2 m_guideStart;
	Vec2 m_guideEnd;
	Vec2 m_end;
};