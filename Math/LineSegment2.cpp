#include "LineSegment2.hpp"

LineSegment2::LineSegment2()
{
	m_start = Vec2(0, 0);
	m_end = Vec2(0, 0);
}

LineSegment2::~LineSegment2()
{
	
}

LineSegment2::LineSegment2(Vec2 start, Vec2 end)
{
	m_start = start;
	m_end = end;
}

void LineSegment2::Translate(Vec2 translation)
{
	m_start += translation;
	m_end += translation;
}

void LineSegment2::SetCenter(Vec2 center)
{
	Vec2 currentCenter = (m_end + m_start) / 2;
	Vec2 currentToNewCenter = center - currentCenter;
	Translate(currentToNewCenter);
}

void LineSegment2::RotateAroundCenterDeg(float rotationDeltaDeg)
{
	Vec2 center = (m_end + m_start) / 2;
	Vec2 centerToEnd = m_end - center;
	Vec2 centerToStart = m_start - center;
	m_end = centerToEnd.GetRotatedDegrees(rotationDeltaDeg);
	m_start = centerToStart.GetRotatedDegrees(-rotationDeltaDeg);
}