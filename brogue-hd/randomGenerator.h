#pragma once

#include "simpleArray.h"
#include "randomRange.h"
#include <random>

using namespace brogueHd::simple;

namespace brogueHd::backend::generator
{
	class randomGenerator
	{
	public:

		randomGenerator(int id);
		randomGenerator(int id, unsigned long seed);
		~randomGenerator();

		/// <summary>
		/// Returns true if id matches the provided id
		/// </summary>
		bool checkId(int id) { return _id == id; }

		/// <summary>
		/// Reseeds the random number generator
		/// </summary>
		void reset(unsigned long seed);

		/// <summary>
		/// Returns random index from array based on its weights
		/// </summary>
		short randWeighted(const simpleArray<short>& weights);

		/// <summary>
		/// Returns random from the provided range, using the clumping factor
		/// </summary>
		short randClump(randomRange theRange);

		/// <summary>
		/// Get a random int between lowerBound and upperBound, inclusive, with probability distribution
		/// affected by clumpFactor.
		/// </summary>
		short randClumpedRange(short lowerBound, short upperBound, short clumpFactor);

		/// <summary>
		/// Randomizes the input array
		/// </summary>
		void shuffleList(short* list, short listLength);

		/// <summary>
		/// Creates list of numbers in order (non-random)
		/// </summary>
		void fillSequentialList(short* list, short listLength);

		/// <summary>
		/// Returns random integer between bounds, inclusive.
		/// </summary>
		int rand_range(int lowerBound, int upperBound);

		/// <summary>
		/// Generates next double Uniform[0,1] and keeps private members updated
		/// </summary>
		double next();

		/// <summary>
		/// Scales the next draw from Uniform[0, 1] and keeps private members updated
		/// </summary>
		double next(double low, double high);

		/// <summary>
		/// Creates a random color with random color channels between the two provided. Makes U[0,1] draws
		/// scaled by the two colors' channels.
		/// </summary>
		color nextColor(color low, color high);

	private:

		/// <summary>
		/// Calls next random number [0, RAND_MAX] and scales it to the 
		/// provided range.
		/// </summary>
		int range(int upperBound);

	private:

		int _id;
		unsigned long _seed;
		unsigned long _numbersGenerated;
	};

	randomGenerator::randomGenerator(int id)
	{
		//brogueAssert(id == RANDOM_GENERATOR_MAIN || id == RANDOM_GENERATOR_COSMETIC)

		_id = id;
		_seed = 0;
		_numbersGenerated = 0;
	}

	randomGenerator::randomGenerator(int id, unsigned long seed)
	{
		//brogueAssert(id == RANDOM_GENERATOR_MAIN || id == RANDOM_GENERATOR_COSMETIC)

		_id = id;

		this->reset(seed);
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

	double randomGenerator::next(double low, double high)
	{
		// Uniform [0,1]
		double nextRand = rand() / (double)RAND_MAX;

		// Goes with rand()
		_numbersGenerated++;

		return (nextRand * (high - low)) + low;
	}
	color randomGenerator::nextColor(color low, color high)
	{
		float red = next(low.red, high.red);
		float green = next(low.green, high.green);
		float blue = next(low.blue, high.blue);

		return color(red, green, blue);
	}
	void randomGenerator::reset(unsigned long seed)
	{
		_seed = seed;
		_numbersGenerated = 0;

		// Seed the generator for later use with rand()
		srand(seed);
	}

	int randomGenerator::range(int upperBound)
	{
		// Expand and truncate
		return (int)(next() * upperBound);
	}

	int randomGenerator::rand_range(int lowerBound, int upperBound)
	{
		//brogueAssert(lowerBound <= INT_MAX && upperBound <= INT_MAX);

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

	short randomGenerator::randWeighted(const simpleArray<short>& weights)
	{
		short sum = 0;

		for (int index = 0; index < weights.count(); index++)
		{
			sum += weights.get(index);
		}

		double nextDouble = next() * sum;

		sum = 0;

		for (int index = 0; index < weights.count(); index++)
		{
			sum += weights.get(index);

			if (sum >= nextDouble)
				return index;
		}

		return weights.count() - 1;
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

