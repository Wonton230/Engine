#pragma once

struct Vec2;

struct Vec3
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

public:
	// Construction/Destruction
	~Vec3() {}												// destructor (do nothing)
	Vec3() {}												// default constructor (do nothing)
	Vec3(Vec3 const& copyFrom);							// copy constructor (from another vec3)
	explicit Vec3(float initialX, float initialY, float initialZ);		// explicit constructor (from x, y, z)

	// Operators (const)
	bool		operator==(Vec3 const& compare) const;
	bool		operator!=(Vec3 const& compare) const;
	Vec3 const	operator+(Vec3 const& vecToAdd) const;
	Vec3 const	operator-(Vec3 const& vecToSubtract) const;
	Vec3 const	operator-() const;
	Vec3 const	operator*(float uniformScale) const;
	Vec3 const	operator*(Vec3 const& vecToMultiply) const;
	Vec3 const	operator/(float inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(Vec3 const& vecToAdd);
	void		operator-=(Vec3 const& vecToSubtract);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);	
	void		operator=(Vec3 const& copyFrom);
	friend Vec3 const operator*(float uniformScale, Vec3 const& vecToScale);

	//Methods
	float GetLength() const;
	float GetLengthXY() const;
	float GetLengthSquared() const;
	float GetLengthXYSquared() const;
	float GetAngleAboutZDegrees() const;
	float GetAngleAboutZRadians() const;
	Vec3 GetNormalized() const;
	Vec3 GetRotatedAboutZDegrees(float degrees) const;
	Vec3 GetRotatedAboutZRadians(float radians) const;
	Vec3 GetFlattenedNormalXY() const;
	Vec2 GetFlattenedXY() const;

	void SetLength(float length);

	const static Vec3 MakeFromPolarRadians(float yawRadians, float pitchRadians, float length = 1.0f);
	const static Vec3 MakeFromPolarDegrees(float yawDegrees, float pitchDegrees, float length = 1.0f);
	void SetFromText(char const* text);

	static const Vec3 ZERO;
	static const Vec3 WORLD_UP;
	static const Vec3 WORLD_FORWARD;
	static const Vec3 WORLD_LEFT;
};