#pragma once
#include <vector>
#include "Engine/Math/Vec2.hpp"

class CubicBezierCurve2D;

class CubicHermiteCurve2D
{
public:
	CubicHermiteCurve2D(Vec2 start, Vec2 velStart, Vec2 end, Vec2 velEnd);
	explicit CubicHermiteCurve2D(CubicBezierCurve2D const& curve);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float length, int numSubDivisions = 64) const;

	Vec2 m_start;
	Vec2 m_startVel;
	Vec2 m_end;
	Vec2 m_endVel;
};