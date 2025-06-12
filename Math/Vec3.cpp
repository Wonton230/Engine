#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
//#include "Engine/Core/EngineCommon.hpp"

const Vec3 Vec3::ZERO = Vec3(0.f, 0.f, 0.f);
const Vec3 Vec3::WORLD_UP = Vec3(0.f, 0.f, 1.f);
const Vec3 Vec3::WORLD_FORWARD = Vec3(1.f, 0.f, 0.f);
const Vec3 Vec3::WORLD_LEFT = Vec3(0.f, 1.f, 0.f);

//-----------------------------------------------------------------------------------------------
Vec3::Vec3(Vec3 const& copy)
{
	x = copy.x;
	y = copy.y;
	z = copy.z;
}

Vec3::Vec3(float initialX, float initialY, float initialZ)
{
	x = initialX;
	y = initialY;
	z = initialZ;
}

//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator + (Vec3 const& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}

//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator-(Vec3 const& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

//------------------------------------------------------------------------------------------------
Vec3 const Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}

//------------------------------------------------------------------------------------------------
Vec3 const Vec3::operator*(Vec3 const& vecToMultiply) const
{
	//help
	float dotProduct = (x * vecToMultiply.x) + (y * vecToMultiply.y);
	return Vec3(dotProduct * vecToMultiply.x, dotProduct * vecToMultiply.y, dotProduct * vecToMultiply.z);
}

//-----------------------------------------------------------------------------------------------
Vec3 const Vec3::operator/(float inverseScale) const
{
	return Vec3(x * (1 / inverseScale), y * (1 / inverseScale), z * (1/inverseScale));
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator+=(Vec3 const& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator-=(Vec3 const& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator/=(const float uniformDivisor)
{
	x /= uniformDivisor;
	y /= uniformDivisor;
	z /= uniformDivisor;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator=(Vec3 const& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

//-----------------------------------------------------------------------------------------------
Vec3 const operator*(float uniformScale, Vec3 const& vecToScale)
{
	return Vec3(uniformScale * vecToScale.x, uniformScale * vecToScale.y, uniformScale * vecToScale.z);
}

//-----------------------------------------------------------------------------------------------
bool Vec3::operator==(Vec3 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z)
		return true;
	return false;
}

//-----------------------------------------------------------------------------------------------
bool Vec3::operator!=(Vec3 const& compare) const
{
	if (x == compare.x && y == compare.y && z == compare.z)
		return false;
	return true;
}

float Vec3::GetLength() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

float Vec3::GetLengthXY() const
{
	return sqrtf((x * x) + (y * y));
}

float Vec3::GetLengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

float Vec3::GetLengthXYSquared() const
{
	return (x * x) + (y * y);
}

float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}

float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}

Vec3 Vec3::GetNormalized() const
{
	float length = GetLength();
	float lengthMult = 1 / length;
	return Vec3(x * lengthMult, y * lengthMult, z * lengthMult);
}

Vec3 Vec3::GetRotatedAboutZDegrees(float addDegrees) const
{
	float angle = Atan2Degrees(y, x) + addDegrees;
	float radius = GetLengthXY();
	return Vec3(radius * CosDegrees(angle), radius * SinDegrees(angle),z);
}

Vec3 Vec3::GetRotatedAboutZRadians(float addRadians) const
{
	float angle = atan2f(y, x) + addRadians;
	float radius = GetLengthXY();
	return Vec3(radius * cosf(angle), radius * sinf(angle), z);
}

Vec3 Vec3::GetFlattenedNormalXY() const
{
	Vec3 flat = Vec3(x, y, 0);
	return flat.GetNormalized();
}

Vec2 Vec3::GetFlattenedXY() const
{
	return Vec2(x,y);
}

void Vec3::SetLength(float length)
{
	Vec3 temp = Vec3(x, y, z);
	temp = temp.GetNormalized();
	temp = temp * length;
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

const Vec3 Vec3::MakeFromPolarRadians(float yawRadians, float pitchRadians, float length)
{
	float tempX = length * cosf(yawRadians) * cosf(pitchRadians);
	float tempY = length * cosf(pitchRadians) * sinf(yawRadians);
	float tempZ = length * sinf(pitchRadians);
	return Vec3(tempX, tempY, tempZ);
}

const Vec3 Vec3::MakeFromPolarDegrees(float yawDegrees, float pitchDegrees, float length)
{
	float tempX = length * CosDegrees(yawDegrees) * CosDegrees(pitchDegrees);
	float tempY = length * CosDegrees(pitchDegrees) * SinDegrees(yawDegrees);
	float tempZ = length * SinDegrees(pitchDegrees);
	return Vec3(tempX, tempY, tempZ);
}

void Vec3::SetFromText(char const* text)
{
	Strings variables = SplitStringOnDelimiter(text, ',');
	if (variables.size() != 3 || variables[0].c_str() == nullptr || variables[1].c_str() == nullptr || variables[2].c_str() == nullptr)
	{
		ERROR_AND_DIE("Invalid arguments passed into Vec3 String");
	}
	x = static_cast<float>(atof(variables[0].c_str()));
	y = static_cast<float>(atof(variables[1].c_str()));
	z = static_cast<float>(atof(variables[2].c_str()));
}