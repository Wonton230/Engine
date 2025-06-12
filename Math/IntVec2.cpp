#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <cmath>

IntVec2::IntVec2(IntVec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

IntVec2::IntVec2(int initialX, int initialY)
{
	x = initialX;
	y = initialY;
}

bool IntVec2::operator==(IntVec2 const& compare) const
{
	if (x == compare.x && y == compare.y)
		return true;
	else
		return false;
}

bool IntVec2::operator!=(IntVec2 const& compare) const
{
	if (x != compare.x || y != compare.y)
		return true;
	else
		return false;
}

IntVec2 const IntVec2::operator+(IntVec2 const& vecToAdd) const
{
	return IntVec2(x + vecToAdd.x, y + vecToAdd.y);
}

IntVec2 const IntVec2::operator-(IntVec2 const& vecToSubtract) const
{
	return IntVec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

IntVec2 const IntVec2::operator-() const
{
	return IntVec2(-x, -y);
}

IntVec2 const IntVec2::operator*(int uniformScale) const
{
	return IntVec2(x * uniformScale, y * uniformScale);
}

IntVec2 const IntVec2::operator*(IntVec2 const& vecToMultiply) const
{
	return vecToMultiply;
}

IntVec2 const IntVec2::operator/(int inverseScale) const
{
	return IntVec2(x / inverseScale, y / inverseScale);
}

IntVec2 const operator*(int uniformScale, IntVec2 const& vecToScale)
{
	return IntVec2(vecToScale.x * uniformScale, vecToScale.y * uniformScale);
}

void IntVec2::operator+=(IntVec2 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}

void IntVec2::operator-=(IntVec2 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}

void IntVec2::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}

void IntVec2::operator/=(const int uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}

void IntVec2::operator=(IntVec2 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

float IntVec2::GetLength() const
{
	float length = sqrtf(((float)x * (float)x) + ((float)y * (float)y));
	return length;
}

int	IntVec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}

int	IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}

float IntVec2::GetOrientationDegrees() const
{
	return ConvertRadiansToDegrees(atan2f((float)y, (float)x));
}

float IntVec2::GetOrientationRadians() const
{
	return atan2f((float)y, (float)x);
}

IntVec2 IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}

IntVec2 IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

void IntVec2::Rotate90Degrees()
{
	int temp = x;
	x = -y;
	y = temp;
}

void IntVec2::RotateMinus90Degrees()
{
	int temp = x;
	x = y;
	y = -temp;
}

void IntVec2::SetFromText(char const* text)
{
	Strings variables = SplitStringOnDelimiter(text, ',');
	if (variables.size() != 2 || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into IntVec2 String");
	}
	x = atoi(variables[0].c_str());
	y = atoi(variables[1].c_str());
}
