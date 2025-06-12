#include "Triangle2.hpp"
#include "Engine/Math/MathUtils.hpp"

Triangle2::Triangle2()
{
	m_pointsCounterClockwise[0] = Vec2(0, 0);
	m_pointsCounterClockwise[1] = Vec2(8, 0);
	m_pointsCounterClockwise[2] = Vec2(0, 6);
}

Triangle2::Triangle2(Vec2 const& a, Vec2 const& b, Vec2 const& c)
{
	m_pointsCounterClockwise[0] = a;
	m_pointsCounterClockwise[1] = b;
	m_pointsCounterClockwise[2] = c;
}

Triangle2::~Triangle2()
{
}
