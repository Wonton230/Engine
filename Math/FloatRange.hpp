#pragma once

struct FloatRange
{
public:
	float m_min;
	float m_max;

	FloatRange();
	~FloatRange();
	FloatRange(FloatRange const& copyFrom);
	explicit FloatRange(float initialMin, float initialMax);

	void operator=(FloatRange const& copyFrom);
	bool operator!=(FloatRange const& compareTo);
	bool operator==(FloatRange const& compareTo);

	bool IsOnRange(float value) const;
	bool IsOverlappingWith(FloatRange range) const;
	float GetRandomFloatInclusive() const;

	void SetFromText(char const* text);

	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;
};



