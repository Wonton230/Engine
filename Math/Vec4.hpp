#pragma once

struct Vec4
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

public:
	// Construction/Destruction
	~Vec4() {}												// destructor (do nothing)
	Vec4() {}												// default constructor (do nothing)
	Vec4(Vec4 const& copyFrom);							// copy constructor (from another vec3)
	explicit Vec4(float initialX, float initialY, float initialZ, float initialW);		// explicit constructor (from x, y, z)

	// Operators (const)
	bool		operator==(Vec4 const& compare) const;
	bool		operator!=(Vec4 const& compare) const;
	Vec4 const	operator+(Vec4 const& vecToAdd) const;
	Vec4 const	operator-(Vec4 const& vecToSubtract) const;
	Vec4 const	operator-() const;
	Vec4 const	operator*(float uniformScale) const;
	Vec4 const	operator*(Vec4 const& vecToMultiply) const;
	Vec4 const	operator/(float inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(Vec4 const& vecToAdd);
	void		operator-=(Vec4 const& vecToSubtract);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);
	void		operator=(Vec4 const& copyFrom);
	friend Vec4 const operator*(float uniformScale, Vec4 const& vecToScale);

	void		SetFromText(char const* text);
};