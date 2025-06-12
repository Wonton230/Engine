#pragma once

class RandomNumberGenerator
{
public:
	int RollRandomIntLessThan(int const maxNonInclusive);
	int RollRandomIntInRange(int minInclusive, int maxInclusive);
	float RollRandomFloatZeroToOne();
	float RollRandomFloatInRange(float minInclusive, float maxInclusive);
private:
//	int m_seed = 0;
//	int m_position = 0;
};