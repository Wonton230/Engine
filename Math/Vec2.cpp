#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Mathutils.hpp"
#include <cmath>
//#include "Engine/Math/MathUtils.hpp"
//#include "Engine/Core/EngineCommon.hpp"


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( Vec2 const& copy )
	: x( 123.f )
	, y( 456.f )
{
	x = copy.x;
	y = copy.y;
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( float initialX, float initialY )
	: x( 123.f )
	, y( 456.f )
{
	x = initialX;
	y = initialY;
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator + ( Vec2 const& vecToAdd ) const
{
	return Vec2( x + vecToAdd.x, y + vecToAdd.y );
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator-( Vec2 const& vecToSubtract ) const
{
	return Vec2( x - vecToSubtract.x, y - vecToSubtract.y );
}


//------------------------------------------------------------------------------------------------
Vec2 const Vec2::operator-() const
{
	return Vec2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator*( float uniformScale ) const
{
	return Vec2( x * uniformScale, y * uniformScale );
}


//------------------------------------------------------------------------------------------------
Vec2 const Vec2::operator*( Vec2 const& vecToMultiply ) const
{
	//help
	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
Vec2 const Vec2::operator/( float inverseScale ) const
{
	return Vec2( x * (1/inverseScale), y * (1/inverseScale) );
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=( Vec2 const& vecToAdd )
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=( Vec2 const& vecToSubtract )
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=( const float uniformScale )
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=( Vec2 const& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
Vec2 const operator*( float uniformScale, Vec2 const& vecToScale )
{
	return Vec2( uniformScale * vecToScale.x, uniformScale * vecToScale.y );
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==( Vec2 const& compare ) const
{
	if (x == compare.x && y == compare.y)
		return true;
	return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=( Vec2 const& compare ) const
{
	if (x == compare.x && y == compare.y)
		return false;
	return true;
}

Vec2 Vec2::MakeFromPolarDegrees(float angle, float radius)
{
	return Vec2(radius * CosDegrees(angle), radius * SinDegrees(angle));
}

Vec2 Vec2::MakeFromPolarRadians(float angle, float radius)
{
	float degrees = ConvertRadiansToDegrees(angle);
	return Vec2(radius * CosDegrees(degrees), radius * SinDegrees(degrees));
}

float Vec2::GetLength() const
{
	return sqrtf((x*x) + (y*y));
}

float Vec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}

float Vec2::GetOrientationDegrees() const
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}

Vec2 Vec2::GetRotated90Degrees() const
{
	return Vec2(-y, x);
}

Vec2 Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}

Vec2 Vec2::GetRotatedDegrees(float degrees) const
{
	float radius = GetLength();
	float newAngle = GetOrientationDegrees() + degrees;
	return MakeFromPolarDegrees(newAngle,radius);
}

Vec2 Vec2::GetRotatedRadians(float radians) const
{
	float radius = GetLength();
	float newAngle = GetOrientationRadians() + radians;
	return MakeFromPolarRadians(newAngle, radius);
}

void Vec2::SetOrientationDegrees(float degrees)
{
	float radius = GetLength();
	Vec2 temp = MakeFromPolarDegrees(degrees, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::SetOrientationRadians(float radians)
{
	float radius = GetLength();
	Vec2 temp = MakeFromPolarRadians(radians, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::SetPolarDegrees(float degrees, float radius)
{
	Vec2 temp = MakeFromPolarDegrees(degrees, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::SetPolarRadians(float radians, float radius)
{
	Vec2 temp = MakeFromPolarRadians(radians, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::RotateDegrees(float degrees)
{
	float radius = GetLength();
	float newAngle = GetOrientationDegrees() + degrees;
	Vec2 temp = MakeFromPolarDegrees(newAngle, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::RotateRadians(float radians)
{
	float radius = GetLength();
	float newAngle = GetOrientationRadians() + radians;
	Vec2 temp = MakeFromPolarRadians(newAngle, radius);
	x = temp.x;
	y = temp.y;
}

void Vec2::Rotate90Degrees()
{
	float temp = x;
	x = -y;
	y = temp;
}

void Vec2::RotateMinus90Degrees()
{
	float temp = -x;
	x = y;
	y = temp;
}

Vec2 Vec2::GetClamped(float limit) const
{
	if (GetLength() > limit)
	{
		float angleDeg = GetOrientationDegrees();
		return MakeFromPolarDegrees(angleDeg, limit);
	}
	else
	{
		return Vec2(x, y);
	}
}

Vec2 Vec2::GetNormalized() const
{
	float angleDeg = GetOrientationDegrees();
	return MakeFromPolarDegrees(angleDeg, 1.f);
}

Vec2 Vec2::GetReflected(Vec2 const& normalOfReflectionSurface) const
{
	Vec2 currentVector = Vec2(this->x,this->y);
	Vec2 agreeingComponent = normalOfReflectionSurface * DotProduct2D(currentVector, normalOfReflectionSurface);
	Vec2 reflection = currentVector - agreeingComponent * 2;
	return reflection;
}

void Vec2::SetLength(float length)
{
	float angleDeg = GetOrientationDegrees();
	Vec2 temp = MakeFromPolarDegrees(angleDeg, length);
	x = temp.x;
	y = temp.y;
}

void Vec2::ClampLength(float limit)
{
	if (GetLength() > limit)
	{
		float angleDeg = GetOrientationDegrees();
		Vec2 temp = MakeFromPolarDegrees(angleDeg, limit);
		x = temp.x;
		y = temp.y;
	}
}

void Vec2::Normalize()
{
	SetLength(1.f);
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float oldLength = GetLength();
	Normalize();
	return oldLength;
}

Vec2 Vec2::Reflect(Vec2 const& normalOfReflectionSurface)
{
	Vec2 currentVector = Vec2(this->x, this->y);
	Vec2 agreeingComponent = normalOfReflectionSurface * DotProduct2D(currentVector, normalOfReflectionSurface);
	*this = currentVector - agreeingComponent * 2;
	return *this;
}

void Vec2::SetFromText(char const* text)
{
	Strings variables = SplitStringOnDelimiter(text, ',');
	if (variables.size() != 2 || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into Vec2 String");
	}
	x = static_cast<float>(atof(variables[0].c_str()));
	y = static_cast<float>(atof(variables[1].c_str()));
}
