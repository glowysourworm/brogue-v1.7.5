#pragma once

#include "color.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleMath.h"
#include <random>

using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	enum class randomVariable
	{
		Uniform = 0,
		Exponential,
		Gaussian
	};

	/// <summary>
	/// Generates random draws from several distributions (see randomVariable enum class). These are determined
	/// using normalized mean and standard deviation - which are scaled to whatever the function requires.
	/// </summary>
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
		/// Get a random int between lowerBound and upperBound, inclusive
		/// </summary>
		int randomRangeInclusive(short lowerBound, short upperBound, randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

		/// <summary>
		/// Gets a random integer between lower and upper bounds, exclusive upper bound, with
		/// specified probability distribution.
		/// </summary>
		int randomRangeExclusive(short lowerBound, short upperBound, randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

		/// <summary>
		/// Returns random index from array based on its weights
		/// </summary>
		template<isNumber T>
		int randomIndex(const simpleArray<T>& weights, randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

		/// <summary>
		/// Randomizes the input array using Uniform random draws
		/// </summary>
		template<typename T>
		simpleArray<T> shuffle(const simpleArray<T>& array);

		/// <summary>
		/// Generates next double with the specified parameters
		/// </summary>
		float next(randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

		/// <summary>
		/// Creates a random color with random color channels.
		/// </summary>
		color nextColor(randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

		/// <summary>
		/// Creates a random color with random color channels between the two provided. Makes draws
		/// scaled by the two colors' channels.
		/// </summary>
		color nextColor(color low, color high, randomVariable type = randomVariable::Uniform, float mean = 0.5f, float stdDev = 0.6);

	private:

		int nextInt(int lowerBound, int upperBound, bool exclusiveUpperBound, randomVariable type, float mean, float stdDev);
		float nextFloat(float lowerBound, float upperBound, randomVariable type, float mean, float stdDev);
		float drawUniform(float low, float high);
		float drawExponential(float low, float high, float mean);
		float drawGaussian(float low, float high, float mean, float stdDev);

	private:

		int _id;
		unsigned long _seed;
		unsigned long _numbersGenerated;

		std::mt19937* _engine;				// Mersenne Twister Engine
	};

	randomGenerator::randomGenerator(int id)
	{
		_id = id;
		_seed = 0;
		_numbersGenerated = 0;
		_engine = new std::mt19937(_seed);
	}

	randomGenerator::randomGenerator(int id, unsigned long seed)
	{
		_id = id;
		_engine = new std::mt19937(_seed);

		this->reset(seed);
	}

	randomGenerator::~randomGenerator()
	{
		delete _engine;
	}

	void randomGenerator::reset(unsigned long seed)
	{
		_seed = seed;
		_numbersGenerated = 0;

		_engine->seed(seed);
	}

	float randomGenerator::drawUniform(float low, float high)
	{
		std::uniform_real_distribution<float> distribution(low, high);

		_numbersGenerated++;

		return distribution(*_engine);
	}
	float randomGenerator::drawExponential(float low, float high, float mean)
	{
		std::exponential_distribution<float> distribution(mean);

		_numbersGenerated++;

		return simpleMath::clamp(distribution(*_engine), low, high);
	}
	float randomGenerator::drawGaussian(float low, float high, float mean, float stdDev)
	{
		std::normal_distribution<float> distribution(mean, stdDev);

		_numbersGenerated++;

		return simpleMath::clamp(distribution(*_engine), low, high);
	}

	int randomGenerator::nextInt(int lowerBound, int upperBound, bool exclusiveUpperBound, randomVariable type, float mean, float stdDev)
	{
		float draw = 0.0f;

		switch (type)
		{
			case randomVariable::Exponential:
				draw = drawExponential(0, 1, mean);
				break;
			case randomVariable::Gaussian:
				draw = drawGaussian(0, 1, mean, stdDev);
				break;
			case randomVariable::Uniform:
				draw = drawUniform(0, 1);
				break;
		}

		if (exclusiveUpperBound)
			return lowerBound + simpleMath::floor(draw * (upperBound - lowerBound));

		else
			return lowerBound + simpleMath::ceiling(draw * (upperBound - lowerBound));
	}

	float randomGenerator::nextFloat(float lowerBound, float upperBound, randomVariable type, float mean, float stdDev)
	{
		float draw = 0.0f;

		switch (type)
		{
			case randomVariable::Exponential:
				draw = drawExponential(0, 1, mean);
				break;
			case randomVariable::Gaussian:
				draw = drawGaussian(0, 1, mean, stdDev);
				break;
			case randomVariable::Uniform:
				draw = drawUniform(0, 1);
				break;
		}

		return lowerBound + (draw * (upperBound - lowerBound));
	}

	int randomGenerator::randomRangeInclusive(short lowerBound, short upperBound, randomVariable type, float mean, float stdDev)
	{
		return nextInt(lowerBound, upperBound, true, type, mean, stdDev);
	}

	int randomGenerator::randomRangeExclusive(short lowerBound, short upperBound, randomVariable type, float mean, float stdDev)
	{
		return nextInt(lowerBound, upperBound, false, type, mean, stdDev);
	}

	template<isNumber T>
	int randomGenerator::randomIndex(const simpleArray<T>& weights, randomVariable type, float mean, float stdDev)
	{
		short sum = 0;

		for (int index = 0; index < weights.count(); index++)
		{
			sum += weights.get(index);
		}

		float nextRandom = nextFloat(0, sum, type, mean, stdDev);

		sum = 0;

		for (int index = 0; index < weights.count(); index++)
		{
			sum += weights.get(index);

			if (sum >= nextRandom)
				return index;
		}

		return weights.count() - 1;
	}

	/// <summary>
	/// Randomizes the input array using Uniform random draws
	/// </summary>
	template<typename T>
	simpleArray<T> randomGenerator::shuffle(const simpleArray<T>& array)
	{
		throw simpleException("Not Implemented:  randomGenerator::shuffle");
	}

	/// <summary>
	/// Generates next double with the specified parameters
	/// </summary>
	float randomGenerator::next(randomVariable type, float mean, float stdDev)
	{
		return nextFloat(0, 1, type, mean, stdDev);
	}

	/// <summary>
	/// Creates a random color with random color channels.
	/// </summary>
	color randomGenerator::nextColor(randomVariable type, float mean, float stdDev)
	{
		color result(next(type, mean, stdDev),
					 next(type, mean, stdDev),
					 next(type, mean, stdDev),
					 next(type, mean, stdDev));

		return result;
	}

	/// <summary>
	/// Creates a random color with random color channels between the two provided. Makes draws
	/// scaled by the two colors' channels.
	/// </summary>
	color randomGenerator::nextColor(color low, color high, randomVariable type, float mean, float stdDev)
	{
		color result(nextFloat(low.red, high.red, type, mean, stdDev),
					 nextFloat(low.green, high.green, type, mean, stdDev),
					 nextFloat(low.blue, high.blue, type, mean, stdDev),
					 nextFloat(low.alpha, high.alpha, type, mean, stdDev));

		return result;
	}
}

