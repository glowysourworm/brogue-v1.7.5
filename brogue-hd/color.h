#pragma once

#include "simple.h"
#include "simpleMath.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model::game
{
	struct color : hashable
	{
	public:

		// base RGB components: (0, 1) -> (0, 255)
		float red;
		float green;
		float blue;

		color() 
		{
			red = 0;
			green = 0;
			blue = 0;
		}
		color(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
		}
		color(float ared, float agreen, float ablue)
		{
			red = ared;
			green = agreen;
			blue = ablue;
		}
		void interpolate(const color& other, float weight)
		{
			red = (other.red * weight) + (red * (1 - weight));
			green = (other.green * weight) + (green * (1 - weight));
			blue = (other.blue * weight) + (blue * (1 - weight));
		}

		void operator=(const color& color)
		{
			red = color.red;
			green = color.green;
			blue = color.blue;
		}
		void operator*(const color& color)
		{
			red = red * color.red;
			green = green * color.green;
			blue = blue * color.blue;
		}
		void clamp(color low, color high)
		{
			red = simpleMath::clamp(red, low.red, high.red);
			green = simpleMath::clamp(green, low.green, high.green);
			blue = simpleMath::clamp(blue, low.blue, high.blue);
		}
		void clamp(float low, float high)
		{
			red = simpleMath::clamp(red, low, high);
			green = simpleMath::clamp(green, low, high);
			blue = simpleMath::clamp(blue, low, high);
		}
		bool compare(const color& color) const
		{
			return red == color.red &&
					green == color.green &&
					blue == color.blue;
		}
		bool operator ==(const color& other) const
		{
			return compare(other);
		}
		bool operator !=(const color& other) const
		{
			return !compare(other);
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(red, green, blue);
		}
	};

	struct colors
	{
	public:
		static color black()	{ return color(0, 0, 0); };
		static color red()		{ return color(1, 0, 0); };
		static color orange()	{ return color(1, 0.5, 0); };
		static color yellow()	{ return color(1, 1, 0); };
		static color green()	{ return color(0, 1, 0); };
		static color blue()		{ return color(0, 0, 1); };
		static color purple()	{ return color(1, 0, 1); };
		static color white()	{ return color(1, 1, 1); };

	public:

		/// <summary>
		/// Returns a shade of gray with the specified value
		/// </summary>
		static color getGray(float value)
		{
			return color(value, value, value);
		}
	};
}