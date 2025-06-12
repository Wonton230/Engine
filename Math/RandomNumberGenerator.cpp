#include "Engine/Math/RandomNumberGenerator.hpp"
#include <stdlib.h>


int RandomNumberGenerator::RollRandomIntLessThan(int const maxNonInclusive)
{
	if (maxNonInclusive == 0)
	{
		return 0;
	}
	return rand() % maxNonInclusive;
}

int RandomNumberGenerator::RollRandomIntInRange(int minInclusive, int maxInclusive)
{
	int range = (maxInclusive - minInclusive) + 1;
	return minInclusive + RollRandomIntLessThan(range);
}

float RandomNumberGenerator::RollRandomFloatZeroToOne()
{
	//Any random number divided by the maximum number rand() can roll. Guaranteed to be equal to or less than 1.
	return (float)rand() / (float)RAND_MAX;
}

float RandomNumberGenerator::RollRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float range = (maxInclusive - minInclusive);
	return minInclusive + range * RollRandomFloatZeroToOne();
}