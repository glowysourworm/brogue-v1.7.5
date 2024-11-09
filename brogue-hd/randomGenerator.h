#pragma once

#include "color.h"
#include "randomRange.h"
#include "simpleArray.h"
#include "simpleMath.h"
#include <cstdlib>

using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
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
		short randClump(const randomRange& theRange);

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
		/// Returns random integer between bounds, excluding the upper bound
		/// </summary>
		int randomIndex(int lower, int upper);

		/// <summary>
		/// Returns a random inteber between the two bounds, including both.
		/// </summary>
		int randomRange(int lowerBound, int upperBound);

		/// <summary>
		/// Generates next double Uniform[0,1] and keeps private members updated
		/// </summary>
		double next();

		/// <summary>
		/// Scales the next draw from Uniform[0, 1] and keeps private members updated
		/// </summary>
		double next(double low, double high);

		/// <summary>
		/// Generates a gaussian random number with the specified parameters
		/// </summary>
		double gaussian(double mean, double stdDev, double lowLimit, double highLimit);

		/// <summary>
		/// Creates a random color with random color channels between the two provided. Makes U[0,1] draws
		/// scaled by the two colors' channels.
		/// </summary>
		color nextColor(color low, color high);

		/// <summary>
		/// Creates a  random color using color channels near the mean - based on gaussian draws.
		/// </summary>
		color nextColorNear(color mean, float stdDev);

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
		float alpha = next(low.alpha, high.alpha);

		return color(red, green, blue, alpha);
	}
	color randomGenerator::nextColorNear(color mean, float stdDev)
	{
		float red = gaussian(mean.red, stdDev, 0, 1);
		float green = gaussian(mean.green, stdDev, 0, 1);
		float blue = gaussian(mean.blue, stdDev, 0, 1);
		float alpha = gaussian(mean.alpha, stdDev, 0, 1);

		return color(red, green, blue, alpha);
	}
	double randomGenerator::gaussian(double mean, double stdDev, double lowLimit, double highLimit)
	{
		// Normal Distribution
		// 
		// https://www.alanzucconi.com/2015/09/16/how-to-sample-from-a-gaussian-distribution/

		double v1, v2, R;

		do
		{
			// Generate U[-1, 1] Variables: 2 draws
			v1 = (2.0 * next()) - 1.0;
			v2 = (2.0 * next()) - 1.0;

			// Calculate R^2
			R = v1 * v1 + v2 * v2;

			// Reject points outside the unit circle (RARE)
		} while (R >= 1.0f || R == 0.0f);

		// Use inverse CDF methods to calculate the gaussian
		double normalValue = v1 * simpleMath::sqrt((-2.0 * simpleMath::naturalLog(R)) / R);

		// The resulting gaussian draw
		double result = (mean + (normalValue * stdDev));

		// Clip off very rare events
		float resultClipped = simpleMath::clamp<float>(result, lowLimit, highLimit);

		return resultClipped;
	}
	void randomGenerator::reset(unsigned long seed)
	{
		_seed = seed;
		_numbersGenerated = 0;

		// Seed the generator for later use with rand()
		srand(seed);
	}

	int randomGenerator::randomIndex(int lowerBound, int upperBound)
	{
		int result = lowerBound + (int)(next() * (upperBound - lowerBound));

		// Small chance of hitting the upper bound
		return simpleMath::clamp(result, lowerBound, upperBound);
	}

	int randomGenerator::randomRange(int lowerBound, int upperBound)
	{
		return lowerBound + (int)(next() * (upperBound - lowerBound));
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

	//short randomGenerator::randClump(const randomRange& theRange)
	//{
	//	return randClumpedRange(theRange.lowerBound, theRange.upperBound, theRange.clumpFactor);
	//}

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
			return randomRange(lowerBound, upperBound);
		}

		short i, total = 0, numSides = (upperBound - lowerBound) / clumpFactor;

		for (i = 0; i < (upperBound - lowerBound) % clumpFactor; i++) {
			total += randomRange(0, numSides + 1);
		}

		for (; i < clumpFactor; i++) {
			total += randomRange(0, numSides);
		}

		return (total + lowerBound);
	}

	void randomGenerator::shuffleList(short* list, short listLength)
	{
		short i, r, buf;
		for (i = 0; i < listLength; i++)
		{
			r = randomRange(0, listLength - 1);
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

