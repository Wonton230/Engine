#pragma once
#include "Engine/Math/LineSegment2.hpp"

struct Capsule2
{
	LineSegment2 m_bone;
	float m_radius;

	Capsule2();
	~Capsule2();
	explicit Capsule2(LineSegment2 line, float radius);

	void Translate(Vec2 translation);
	void SetCenter(Vec2 center);
	void RotateAroundCenterDeg(float rotationDeltaDeg);
};