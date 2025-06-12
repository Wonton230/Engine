#pragma once

struct IntVec2
{
public:
	int x = 0;
	int y = 0;

	//constructors
	~IntVec2() {}
	IntVec2() {}
	IntVec2(IntVec2 const& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	//operators
	bool			operator==(IntVec2 const& compare) const;
	bool			operator!=(IntVec2 const& compare) const;
	IntVec2 const	operator+(IntVec2 const& vecToAdd) const;
	IntVec2 const	operator-(IntVec2 const& vecToSubtract) const;
	IntVec2 const	operator-() const;
	IntVec2 const	operator*(int uniformScale) const;
	IntVec2 const	operator*(IntVec2 const& vecToMultiply) const;
	IntVec2 const	operator/(int inverseScale) const;
	friend IntVec2 const operator*(int uniformScale, IntVec2 const& vecToScale);

	// Operators (self-mutating / non-const)
	void		operator+=(IntVec2 const& vecToAdd);
	void		operator-=(IntVec2 const& vecToSubtract);
	void		operator*=(const int uniformScale);
	void		operator/=(const int uniformDivisor);
	void		operator=(IntVec2 const& copyFrom);

	//methods
	float	GetLength() const;
	int		GetLengthSquared() const;
	int		GetTaxicabLength() const;
	float	GetOrientationDegrees() const;
	float	GetOrientationRadians() const;
	IntVec2	GetRotated90Degrees() const;
	IntVec2	GetRotatedMinus90Degrees() const;

	void	Rotate90Degrees();
	void	RotateMinus90Degrees();
	void	SetFromText(char const* text);
};