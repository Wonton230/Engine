#pragma once


//-----------------------------------------------------------------------------------------------
struct Vec2
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;

public:
	// Construction/Destruction
	~Vec2() {}												// destructor (do nothing)
	Vec2() {}												// default constructor (do nothing)
	Vec2( Vec2 const& copyFrom );							// copy constructor (from another vec2)
	explicit Vec2( float initialX, float initialY );		// explicit constructor (from x, y)

	// Operators (const)
	bool		operator==( Vec2 const& compare ) const;		// vec2 == vec2
	bool		operator!=( Vec2 const& compare ) const;		// vec2 != vec2
	Vec2 const	operator+( Vec2 const& vecToAdd ) const;		// vec2 + vec2
	Vec2 const	operator-( Vec2 const& vecToSubtract ) const;	// vec2 - vec2
	Vec2 const	operator-() const;								// -vec2, i.e. "unary negation"
	Vec2 const	operator*( float uniformScale ) const;			// vec2 * float
	Vec2 const	operator*( Vec2 const& vecToMultiply ) const;	// vec2 * vec2
	Vec2 const	operator/( float inverseScale ) const;			// vec2 / float

	// Operators (self-mutating / non-const)
	void		operator+=( Vec2 const& vecToAdd );				// vec2 += vec2
	void		operator-=( Vec2 const& vecToSubtract );		// vec2 -= vec2
	void		operator*=( const float uniformScale );			// vec2 *= float
	void		operator/=( const float uniformDivisor );		// vec2 /= float
	void		operator=( Vec2 const& copyFrom );				// vec2 = vec2

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend Vec2 const operator*( float uniformScale, Vec2 const& vecToScale );	// float * vec2
	
	//constants
	static Vec2 ZERO;
	static Vec2 ONE;

	//Makers
	static Vec2 MakeFromPolarDegrees(float angle, float length = 1.f);
	static Vec2 MakeFromPolarRadians(float angle, float length = 1.f);

	//Getters
	float		GetLength() const;
	float		GetLengthSquared() const;
	float		GetOrientationDegrees() const;
	float		GetOrientationRadians() const;
	Vec2		GetRotated90Degrees() const;
	Vec2		GetRotatedMinus90Degrees() const;
	Vec2		GetRotatedDegrees(float degrees) const;
	Vec2		GetRotatedRadians(float radians) const;
	Vec2		GetClamped(float limit) const;
	Vec2		GetNormalized() const;
	Vec2		GetReflected(Vec2 const& normalOfReflectionSurface) const;

	//Setters
	void		SetOrientationDegrees(float degrees);
	void		SetOrientationRadians(float radians);
	void		SetPolarDegrees(float degrees, float radius);
	void		SetPolarRadians(float radians, float radius);
	void		SetLength(float length);
	void		ClampLength(float limit);
	void		RotateDegrees(float degrees);
	void		RotateRadians(float radians);
	void		Rotate90Degrees();
	void		RotateMinus90Degrees();
	void		Normalize();
	float		NormalizeAndGetPreviousLength();
	Vec2		Reflect(Vec2 const& normalOfReflectionSurface);
	void		SetFromText(char const* text);
};