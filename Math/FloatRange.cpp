#include "FloatRange.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

extern RandomNumberGenerator* g_rng;

const FloatRange FloatRange::ZERO		 = FloatRange(0.f, 0.f);
const FloatRange FloatRange::ONE		 = FloatRange(1.f, 1.f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.f, 1.f);

FloatRange::FloatRange()
{
	m_min = 0.f;
	m_max = 0.f;
}

FloatRange::~FloatRange()
{
}

FloatRange::FloatRange(FloatRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

FloatRange::FloatRange(float initialMin, float initialMax)
{
	m_min = initialMin;
	m_max = initialMax;
}

void FloatRange::operator=(FloatRange const& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

bool FloatRange::operator!=(FloatRange const& compareTo)
{
	return (m_min != compareTo.m_min || m_max != compareTo.m_max);
}

bool FloatRange::operator==(FloatRange const& compareTo)
{
	return (m_min == compareTo.m_min && m_max == compareTo.m_max);
}

bool FloatRange::IsOnRange(float value) const
{
	return (m_min <= value && value <= m_max);
}

bool FloatRange::IsOverlappingWith(FloatRange range) const
{
	return (range.m_min <= m_max && m_min <= range.m_max);
}

float FloatRange::GetRandomFloatInclusive() const
{
	return g_rng->RollRandomFloatInRange(m_min, m_max);
}

void FloatRange::SetFromText(char const* text)
{
	//VALID: min, max
	Strings variables = SplitStringOnDelimiter(text, '~');
	if (variables.size() != 2 || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into FloatRange String");
	}
	m_min = static_cast<float>(atof(variables[0].c_str()));
	m_max = static_cast<float>(atof(variables[1].c_str()));
}
