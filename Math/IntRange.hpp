#pragma once

struct IntRange
{
public:
	int m_min;
	int m_max;

	IntRange();
	~IntRange();
	IntRange(IntRange const& copyFrom);
	explicit IntRange(int initialMin, int initialMax);

	void operator=(IntRange const& copyFrom);
	bool operator!=(IntRange const& compareTo);
	bool operator==(IntRange const& compareTo);

	bool IsOnRange(float value);
	bool IsOverlappingWith(IntRange range);
};

static const IntRange ZERO = IntRange(0, 0);
static const IntRange ONE = IntRange(1, 1);
static const IntRange ZERO_TO_ONE = IntRange(0, 1);