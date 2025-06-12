#include "Vec4.hpp"

Vec4::Vec4(Vec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
{
	x = initialX;
	y = initialY;
	z = initialZ;
	w = initialW;
}

bool Vec4::operator==(Vec4 const& compare) const
{
	return	x == compare.x &&
			y == compare.y &&
			z == compare.z &&
			w == compare.w;
}

bool Vec4::operator!=(Vec4 const& compare) const
{
	return	x != compare.x ||
			y != compare.y ||
			z != compare.z ||
			w != compare.w;
}

Vec4 const Vec4::operator+(Vec4 const& vecToAdd) const
{
	return Vec4(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z, w + vecToAdd.w);
}

Vec4 const Vec4::operator-(Vec4 const& vecToSubtract) const
{
	return Vec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

Vec4 const Vec4::operator-() const
{
	return Vec4(-x, -y, -z, -w);
}

Vec4 const Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}

Vec4 const Vec4::operator*(Vec4 const& vecToMultiply) const
{
	vecToMultiply;
	return Vec4();
}

Vec4 const Vec4::operator/(float inverseScale) const
{
	float divisor = 1 / inverseScale;
	return Vec4(x * divisor, y * divisor, z * divisor, w * divisor);
}

void Vec4::operator+=(Vec4 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

void Vec4::operator-=(Vec4 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

void Vec4::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

void Vec4::operator/=(const float uniformDivisor)
{
	float divisor = 1 / uniformDivisor;
	x *= divisor;
	y *= divisor;
	z *= divisor;
	w *= divisor;
}

void Vec4::operator=(Vec4 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}

Vec4 const operator*(float uniformScale, Vec4 const& vecToScale)
{
	return Vec4(vecToScale.x * uniformScale, vecToScale.y * uniformScale, vecToScale.z * uniformScale, vecToScale.w * uniformScale);
}
