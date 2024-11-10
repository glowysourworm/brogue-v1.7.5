#pragma once

#include "color.h"
#include "simple.h"

namespace brogueHd::backend::model
{
	enum brogueGradientType
	{
		Horizontal = 0,
		Vertical = 1,
		Circular = 2
	};

	struct colorGradient : hashable
	{
		colorGradient()
		{
			gradient1 = default_value::value<color>();
			gradient2 = default_value::value<color>();
			gradientType = default_value::value<brogueGradientType>();
		}
		colorGradient(const color& color1, const color& color2, brogueGradientType type)
		{
			gradient1 = color1;
			gradient2 = color2;
			gradientType = type;
		}
		colorGradient(const colorGradient& copy)
		{
			gradient1 = copy.gradient1;
			gradient2 = copy.gradient2;
			gradientType = copy.gradientType;
		}

		color getColor(float interpolationWeight) const
		{
			return color::interpolate(gradient1, gradient2, interpolationWeight);
		}

		color gradient1;
		color gradient2;
		brogueGradientType gradientType;
	};
}