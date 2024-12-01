#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleMaskedInt32.h"
#include <cstdint>

namespace brogueHd::simple
{
	struct simplePixel : simpleStruct
	{
		enum colorChannel
		{
			R,
			G,
			B,
			A
		};

		simplePixel()
		{
			red = default_value::value<simpleMaskedInt32>();
			green = default_value::value<simpleMaskedInt32>();
			blue = default_value::value<simpleMaskedInt32>();
			alpha = default_value::value<simpleMaskedInt32>();
			maxValue = 0;
		}
		simplePixel(simpleMaskedInt32 r, simpleMaskedInt32 g, simpleMaskedInt32 b, simpleMaskedInt32 a, int32_t amaxValue)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
			maxValue = amaxValue;
		}
		simplePixel(const simplePixel& copy)
		{
			red = copy.red;
			green = copy.green;
			blue = copy.blue;
			alpha = copy.alpha;
			maxValue = copy.maxValue;
		}
		void operator=(const simplePixel& copy)
		{
			red = copy.red;
			green = copy.green;
			blue = copy.blue;
			alpha = copy.alpha;
			maxValue = copy.maxValue;
		}

		/// <summary>
		/// Returns normalized red channel (0, 1)
		/// </summary>
		float getNormalized(simplePixel::colorChannel channel) const
		{
			switch (channel)
			{
				case simplePixel::colorChannel::R:
					return red.resolve() / (float)maxValue;
				case simplePixel::colorChannel::G:
					return green.resolve() / (float)maxValue;
				case simplePixel::colorChannel::B:
					return blue.resolve() / (float)maxValue;
				case simplePixel::colorChannel::A:
					return alpha.resolve() / (float)maxValue;
				default:
					throw simpleException("Unhandled simplePixel color channel type:  simplePixel.h");
			}

			return 0;
		}

		simpleMaskedInt32 get(simplePixel::colorChannel channel) const
		{
			switch (channel)
			{
				case simplePixel::colorChannel::R:
					return red;
				case simplePixel::colorChannel::G:
					return green;
				case simplePixel::colorChannel::B:
					return blue;
				case simplePixel::colorChannel::A:
					return alpha;
				default:
					throw simpleException("Unhandled simplePixel color channel type:  simplePixel.h");
			}

			return default_value::value<simpleMaskedInt32>();
		}

		/// <summary>
		/// Returns the data properly set for RGBA encoding
		/// </summary>
		/// <returns></returns>
		int32_t getRGBA() const
		{
			return red.getValue() | green.getValue() | blue.getValue() | alpha.getValue();
		}

		/// <summary>
		/// Returns the max integer value of the any of the color channels
		/// </summary>
		int32_t getMaxValue() const
		{
			return maxValue;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(red, green, blue, alpha, maxValue);
		}

	protected:

		simpleMaskedInt32 red;
		simpleMaskedInt32 green;
		simpleMaskedInt32 blue;
		simpleMaskedInt32 alpha;

		int32_t maxValue;
	};
}