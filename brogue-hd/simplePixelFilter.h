#pragma once

#include "color.h"
#include "simpleMaskedInt32.h"
#include "simplePixel.h"
#include <cstdint>

namespace brogueHd::simple
{
	class simplePixelFilter
	{
	public:

		simplePixelFilter() {};
		~simplePixelFilter() {};

		virtual simplePixel apply(const simplePixel& input) const
		{
			return input;
		}
	};

	class simpleMapColorFilter : simplePixelFilter
	{
	public:

		simpleMapColorFilter() {};
		~simpleMapColorFilter() {};
		simpleMapColorFilter(const color& inputColor, const color& outputColor)
		{
			_inputColor = inputColor;
			_outputColor = outputColor;
		}
		simpleMapColorFilter(const simpleMapColorFilter& copy)
		{
			_inputColor = copy.getInputColor();
			_outputColor = copy.getOutputColor();
		}

		simplePixel apply(const simplePixel& input) const override
		{
			simpleMaskedInt32 red = input.get(simplePixel::colorChannel::R);
			simpleMaskedInt32 green = input.get(simplePixel::colorChannel::G);
			simpleMaskedInt32 blue = input.get(simplePixel::colorChannel::B);
			simpleMaskedInt32 alpha = input.get(simplePixel::colorChannel::A);

			int32_t inputR = _inputColor.red * input.getMaxValue();
			int32_t inputG = _inputColor.green * input.getMaxValue();
			int32_t inputB = _inputColor.blue * input.getMaxValue();
			int32_t inputA = _inputColor.alpha * input.getMaxValue();

			int32_t outputR = _inputColor.red == input.getNormalized(simplePixel::colorChannel::R) ? _outputColor.red * input.getMaxValue() : red.getValue();
			int32_t outputG = _inputColor.green == input.getNormalized(simplePixel::colorChannel::G) ? _outputColor.green * input.getMaxValue() : green.getValue();
			int32_t outputB = _inputColor.blue == input.getNormalized(simplePixel::colorChannel::B) ? _outputColor.blue * input.getMaxValue() : blue.getValue();
			int32_t outputA = _inputColor.alpha == input.getNormalized(simplePixel::colorChannel::A) ? _outputColor.alpha * input.getMaxValue() : alpha.getValue();

			simpleMaskedInt32 outputRed(outputR, red.getMask(), red.getBitShiftLeft());
			simpleMaskedInt32 outputGreen(outputG, green.getMask(), green.getBitShiftLeft());
			simpleMaskedInt32 outputBlue(outputB, blue.getMask(), blue.getBitShiftLeft());
			simpleMaskedInt32 outputAlpha(outputA, alpha.getMask(), alpha.getBitShiftLeft());

			return simplePixel(outputRed, outputGreen, outputBlue, outputAlpha, input.getMaxValue());
		}

	protected:

		color getInputColor() const
		{
			return _inputColor;
		}
		color getOutputColor() const
		{
			return _outputColor;
		}

	private:

		color _inputColor;
		color _outputColor;

	};
}