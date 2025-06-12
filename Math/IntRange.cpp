#include "IntRange.hpp"

IntRange::IntRange()
{
	m_min = 0;
	m_max = 0;
}

IntRange::~IntRange()
{
}

IntRange::IntRange(IntRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

IntRange::IntRange(int initialMin, int initialMax)
{
	m_min = initialMin;
	m_max = initialMax;
}

void IntRange::operator=(IntRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool IntRange::operator!=(IntRange const& compareTo)
{
	return (m_min != compareTo.m_min || m_max != compareTo.m_max);
}

bool IntRange::operator==(IntRange const& compareTo)
{
	return (m_min == compareTo.m_min && m_max == compareTo.m_max);
}

bool IntRange::IsOnRange(float value)
{
	return (m_min <= value && value <= m_max);
}

bool IntRange::IsOverlappingWith(IntRange range)
{
	return (range.m_min <= m_max && m_min <= range.m_max);
}