#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"

struct LineSegment2
{
public:
	Vec2 m_start;
	Vec2 m_end;

	LineSegment2();
	~LineSegment2();
	explicit LineSegment2(Vec2 start, Vec2 end);

	void Translate(Vec2 translation);
	void SetCenter(Vec2 center);
	void RotateAroundCenterDeg(float rotationDeltaDeg);
};