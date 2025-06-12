#pragma once
#include "Engine/Math/Vec2.hpp"

struct Triangle2
{
public:
	Vec2 m_pointsCounterClockwise[3];

	Triangle2();
	explicit Triangle2(Vec2 const& a, Vec2 const& b, Vec2 const& c);
	~Triangle2();
};