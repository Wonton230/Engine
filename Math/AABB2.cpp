#include "AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"

const AABB2 AABB2::ZERO_TO_ONE = AABB2(Vec2(0.f, 0.f), Vec2(1.f, 1.f));

AABB2::AABB2(AABB2 const& copyFrom)
{
	m_mins = copyFrom.m_mins;
	m_maxs = copyFrom.m_maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_mins = Vec2(minX, minY);
	m_maxs = Vec2(maxX, maxY);
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
{
	m_mins = mins;
	m_maxs = maxs;
}

bool AABB2::IsPointInside(Vec2 const& point) const
{
	if (point.x >= m_mins.x && point.x <= m_maxs.x)
	{
		if (point.y >= m_mins.y && point.y <= m_maxs.y)
		{
			return true;
		}
	}
	return false;
}

Vec2 const AABB2::GetCenter() const
{
	return (m_mins + m_maxs) / 2;
}

Vec2 const AABB2::GetDimensions() const
{
	return Vec2(m_maxs.x - m_mins.x, m_maxs.y - m_mins.y);
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& reference) const
{
	if (IsPointInside(reference))
	{
		return reference;
	}
	else
	{
		return Vec2(GetClamped(reference.x, m_mins.x, m_maxs.x), GetClamped(reference.y, m_mins.y, m_maxs.y));
	}
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& UV) const
{

	float x = RangeMap(UV.x, 0.f, 1.f, m_mins.x, m_maxs.x);
	float y = RangeMap(UV.y, 0.f, 1.f, m_mins.y, m_maxs.y);
	return Vec2(x,y);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	float x = GetFractionWithinRange(point.x, m_mins.x, m_maxs.x);
	float y = GetFractionWithinRange(point.y, m_mins.y, m_maxs.y);
	return Vec2(x, y);
}

void AABB2::Translate(Vec2 const& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 offset = newCenter - GetCenter();
	Translate(offset);
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2 oldCenter = GetCenter();
	Vec2 newMax = Vec2(m_mins.x + newDimensions.x, m_mins.y + newDimensions.y);
	m_maxs = newMax;
	SetCenter(oldCenter);
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (IsPointInside(point))
	{
		return;
	}
	else
	{
		if (point.x < m_mins.x)
			m_mins.x = point.x;
		else if (point.x > m_maxs.x)
			m_maxs.x = point.x;

		if (point.y < m_mins.y)
			m_mins.y = point.y;
		else if (point.y > m_maxs.y)
			m_maxs.y = point.y;
	}
}

void AABB2::AddPadding(float padding)
{
	m_mins.x -= padding;
	m_mins.y -= padding;
	m_maxs.x += padding;
	m_maxs.y += padding;
}

void AABB2::HorizontalCut(float topPercentage, AABB2& outBottom)
{
	float splitY = RangeMap(topPercentage, 0.0f, 1.0f, m_maxs.y, m_mins.y);
	outBottom = AABB2(m_mins, Vec2(m_maxs.x, splitY));
	m_mins.y = splitY;
}

void AABB2::VerticalCut(float leftPercentage, AABB2& outRight)
{
	float splitX = RangeMap(leftPercentage, 0.0f, 1.0f, m_mins.x, m_maxs.x);
	outRight = AABB2(Vec2(splitX, m_mins.y), m_maxs);
	m_maxs.x = splitX;
}

AABB2 AABB2::MakeBoxCenteredInBox(Vec2 dimensions)
{
	Vec2 center = this->GetCenter();
	float xOffset = dimensions.x * .5f;
	float yOffset = dimensions.y * .5f;
	return AABB2(Vec2(center.x - xOffset, center.y - yOffset), Vec2(center.x + xOffset, center.y + yOffset));
}
