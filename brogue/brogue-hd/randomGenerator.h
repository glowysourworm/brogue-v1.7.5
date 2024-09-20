#pragma once

#include <random>
#include "mathdef.h"
#include "broguedef.h"

using namespace brogueHd::backend::math;

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
		short randWeighted(const short weights[]);

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
}

