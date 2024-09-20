#include "randomGenerator.h"
#include "brogueMath.h"
#include <random>

namespace brogueHd::backend::generator
{
	randomGenerator::randomGenerator(int id)
	{
		brogueAssert(id == RANDOM_GENERATOR_MAIN || id == RANDOM_GENERATOR_COSMETIC)

		_id = id;
		_seed = 0;
		_numbersGenerated = 0;
	}

	randomGenerator::randomGenerator(int id, unsigned long seed)
	{
		brogueAssert(id == RANDOM_GENERATOR_MAIN || id == RANDOM_GENERATOR_COSMETIC)

		_id = id;
		_seed = seed;
		_numbersGenerated = 0;

		// Seed the generator for later use with rand()
		srand(seed);
	}

	randomGenerator::~randomGenerator()
	{

	}

	double randomGenerator::next()
	{
		// Uniform [0,1]
		double nextRand = rand() / (double)RAND_MAX;

		// Goes with rand()
		_numbersGenerated++;

		return nextRand;
	}

	int randomGenerator::range(int upperBound)
	{
		// Expand and truncate
		return (int)(next() * upperBound);
	}

	int randomGenerator::rand_range(int lowerBound, int upperBound)
	{
		brogueAssert(lowerBound <= INT_MAX && upperBound <= INT_MAX);

		// TODO:  Refactor this to validate it using assert. There must have been some
		//		  code path that wasn't using it properly. If we're going to use assert
		//	      here, then use it. Otherwise, let it pass through; and validate pre-emptively.
		//
		if (upperBound <= lowerBound)
		{
			return lowerBound;
		}

		return lowerBound + range(upperBound - lowerBound + 1);
	}

	short randomGenerator::randWeighted(const short weights[])
	{
		int sum = 0;

		for (int index = 0; index < SIZEOF(weights); index++)
		{
			sum += weights[index];
		}

		double nextDouble = next() * sum;

		sum = 0;

		for (int index = 0; index < SIZEOF(weights); index++)
		{
			sum += weights[index];

			if (sum >= nextDouble)
				return index;
		}

		return SIZEOF(weights) - 1;
	}

	short randomGenerator::randClump(randomRange theRange) 
	{
		return randClumpedRange(theRange.lowerBound, theRange.upperBound, theRange.clumpFactor);
	}

	// Get a random int between lowerBound and upperBound, inclusive, with probability distribution
	// affected by clumpFactor.
	short randomGenerator::randClumpedRange(short lowerBound, short upperBound, short clumpFactor)
	{
		if (upperBound <= lowerBound) 
		{
			return lowerBound;
		}
		if (clumpFactor <= 1) 
		{
			return rand_range(lowerBound, upperBound);
		}

		short i, total = 0, numSides = (upperBound - lowerBound) / clumpFactor;

		for (i = 0; i < (upperBound - lowerBound) % clumpFactor; i++) {
			total += rand_range(0, numSides + 1);
		}

		for (; i < clumpFactor; i++) {
			total += rand_range(0, numSides);
		}

		return (total + lowerBound);
	}

	// Get a random int between lowerBound and upperBound, inclusive
	float randomGenerator::uniform()
	{
		

		return (rand_range(0, 99) < clamp(percent, 0, 100));
	}

	void randomGenerator::shuffleList(short* list, short listLength)
	{
		short i, r, buf;
		for (i = 0; i < listLength; i++) 
		{
			r = rand_range(0, listLength - 1);
			if (i != r) {
				buf = list[r];
				list[r] = list[i];
				list[i] = buf;
			}
		}
	}

	void randomGenerator::fillSequentialList(short* list, short listLength)
	{
		short i;
		for (i = 0; i < listLength; i++) 
		{
			list[i] = i;
		}
	}
}