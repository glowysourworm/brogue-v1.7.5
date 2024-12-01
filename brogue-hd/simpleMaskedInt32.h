#pragma once

#include "simple.h"

#include <cstdint>

namespace brogueHd::simple
{
	struct simpleMaskedInt32 : simpleStruct
	{
		simpleMaskedInt32()
		{
			value = 0;
			mask = 0;
			bitShiftLeft = 0;
		}
		simpleMaskedInt32(int32_t avalue, int32_t amask, int32_t abitShiftLeft)
		{
			value = avalue;
			mask = amask;
			bitShiftLeft = abitShiftLeft;
		}
		simpleMaskedInt32(const simpleMaskedInt32& copy)
		{
			value = copy.value;
			mask = copy.mask;
			bitShiftLeft = copy.bitShiftLeft;
		}
		void operator=(const simpleMaskedInt32& copy)
		{
			value = copy.value;
			mask = copy.mask;
			bitShiftLeft = copy.bitShiftLeft;
		}

		/// <summary>
		/// Applies the mask and bit shift to resolve the masked int
		/// </summary>
		int32_t resolve() const
		{
			// Apply the mask; and reverse the bit shift.
			return (value & mask) >> bitShiftLeft;
		}

		int32_t getValue() const
		{
			return value;
		}
		int32_t getMask() const
		{
			return mask;
		}
		int32_t getBitShiftLeft() const
		{
			return bitShiftLeft;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(value, mask, bitShiftLeft);
		}

	protected:

		int32_t value;
		int32_t mask;
		int32_t bitShiftLeft;

	};
}